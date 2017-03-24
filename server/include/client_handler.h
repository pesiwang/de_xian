#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H


#include "vcs/event_base/event_base.h"
#include "vcs/util/vc_codec.h"

class ClientHandler : public vcs::EventBase::Handler {
public:
    void onConnected();
    void onData();
    void onClosed();

private:
    void _process(VCCodec::Package* p);
    void _processLogin(VCCodec::Package* p);
    void _processCreateRoom(VCCodec::Package* p);
    void _processEnterRoom(VCCodec::Package* p);
    void _processQuitRoom(VCCodec::Package* p);

private:
    uint32_t m_uid = 0;
};

#endif

