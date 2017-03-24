#ifndef ROOM_MODEL_H
#define ROOM_MODEL_H
#include <map>

class Room {
public:
    Room(uint32_t roomId, uint32_t roomType);
    ~Room();

private:
    uint32_t m_roomId;
    uint32_t m_roomType;
};

class RoomModel {
public:
    static RoomModel* instance() {
        static RoomModel self;
        return &self;
    }

    int createRoom(uint32_t type, uint32_t& roomId);

private:
    RoomModel();
    ~RoomModel();
    bool _exist(uint32_t roomId);
    uint32_t _genRoomId();

private:
    std::map<uint32_t, Room*> m_rooms;
};

#endif


