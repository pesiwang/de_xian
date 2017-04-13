#include "client_handler.h"
#include "vcs/util/vc_codec.h"
#include "common.h"
#include "protobuf/package.pb.h"
#include <string>
#include "model/user_model.h"
#include "model/room_model.h"

using namespace vcs;
using namespace std;

void ClientHandler::onConnected(){
    log_debug("client connected, fd=%d", this->fd);
}

void ClientHandler::onClosed(){
    log_debug("client closed, fd=%d", this->fd);
    UserModel::instance()->remove(this->m_uid);

    delete this;
}

void ClientHandler::onData(){
    while(this->connected){
        VCCodec::Package* p = VCCodec::decode(this->input);
        if(p->status == VCCodec::PACKAGE_STATUS_PARTIAL){
            delete p;
            return ;
        } else if(p->status == VCCodec::PACKAGE_STATUS_CORRUPT){
            log_error("decode fail, data corrupt");
            EventBase::instance()->remove(this);
            delete p;
            return ;
        }

        this->_process(p);
        delete p;
    }
}

void ClientHandler::_process(VCCodec::Package* p) {
    log_debug("recv cmd from client, cmd=%d", p->head.cmd);
    switch(p->head.cmd){
        case C2S_LOGIN:
            this->_processLogin(p);
            break;

        case C2S_CREATE_ROOM:
            this->_processCreateRoom(p);
            break;

        case C2S_QUIT_ROOM:
            this->_processQuitRoom(p);
            break;

        case C2S_ENTER_ROOM:
            this->_processEnterRoom(p);
            break;

        default:
            log_error("recv unknown cmd from client, cmd=%d", p->head.cmd);
            break;
    }
}

void ClientHandler::_processLogin(VCCodec::Package* p) {
    pb::c2s_login loginRequest;
    if (!loginRequest.ParseFromString(p->body)) {
        log_error("login fail, bad c2s_login package");
        return;
    }

    uint32_t uid = loginRequest.uid();
    log_debug("user %u login", uid);

    int ret = UserModel::instance()->add(uid);
    if (ret != 0) {
        log_error("login fail, add user fail, ret=%d", ret);
        goto fail;
    }
    
    m_uid = uid;
fail:
    pb::s2c_login resp;
    resp.set_result(ret);
    string body;
    resp.SerializeToString(&body);
    Buffer* buffer  = VCCodec::encode(SERVER_VERSION, S2C_LOGIN, body);
    this->output->append(buffer);
    delete buffer;

    EventBase::instance()->write(this);
}

void ClientHandler::_processCreateRoom(VCCodec::Package* p) {
    log_debug("create room, uid=%u", this->m_uid);

    pb::c2s_create_room createRoomRequest;
    if (!createRoomRequest.ParseFromString(p->body)) {
        log_error("create room fail, bad c2s_create_room package");
        return;
    }
    
    uint32_t type = createRoomRequest.type();
    uint32_t roomId = 0;
    int ret = 0;

    ret = RoomModel::instance()->createRoom(type, roomId);
    if (ret != 0) {
    	log_error("create room fail, uid=%u, ret=%d", this->m_uid, ret);
        goto fail;
    }

    ret = RoomModel::instance()->enterRoom(roomId, this->m_uid);
    if (ret != 0) {
    	log_error("enter room fail, uid=%u, ret=%d", this->m_uid, ret);
        goto fail;
    }
fail:
    pb::s2c_create_room resp;
    resp.set_result(ret);
    resp.set_room_id(roomId);
    string body;
    resp.SerializeToString(&body);
    Buffer* buffer  = VCCodec::encode(SERVER_VERSION, S2C_CREATE_ROOM, body);
    this->output->append(buffer);
    delete buffer;

    EventBase::instance()->write(this);
}

void ClientHandler::_processEnterRoom(VCCodec::Package* p) {

}

void ClientHandler::_processQuitRoom(VCCodec::Package* p) {

}
