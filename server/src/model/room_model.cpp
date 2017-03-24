#include "model/room_model.h"

//================== room ================
Room::Room(uint32_t roomId, uint32_t roomType) {
    m_roomId = roomId;
    m_roomType = roomType;
}

Room::~Room() {

}

//================= room model ==============
RoomModel::RoomModel() {

}

RoomModel::~RoomModel() {

}

bool RoomModel::_exist(uint32_t roomId) {
    if (m_rooms.find(roomId) != m_rooms.end()) {
        return true;
    }

    return false;
}

uint32_t RoomModel::_genRoomId() {
    int maxTryCnt = 10;
    int tryCnt = 0;
    while (++tryCnt < maxTryCnt) {
        uint32_t r = (uint32_t)(rand() % 1000000);
        if (!this->_exist(r)) {
            return r; 
        }
    }

    return 0;
}

int RoomModel::createRoom(uint32_t type, uint32_t& roomId) {
    roomId = this->_genRoomId();
    if (roomId == 0) {
        return 1;
    }
    Room* r = new Room(roomId, type);
    m_rooms[roomId] = r;

    return 0;
}



