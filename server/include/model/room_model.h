#ifndef ROOM_MODEL_H
#define ROOM_MODEL_H
#include <map>

class Seat {
public:
	Seat(uint32_t uid);
	~Seat();

private:
	uint32_t m_uid;
};

class Room {
public:
    Room(uint32_t roomId, uint32_t roomType);
    ~Room();

    bool hasUser(uint32_t uid);
    bool addUser(uint32_t uid);
    bool removeUser(uint32_t uid);

private:
    uint32_t m_roomId;
    uint32_t m_roomType;
    std::map<uint32_t, Seat*> m_seats;
};

class RoomModel {
public:
    static RoomModel* instance() {
        static RoomModel self;
        return &self;
    }

    int createRoom(uint32_t type, uint32_t& roomId);
    int enterRoom(uint32_t roomId, uint32_t uid);
    Room* getRoom(uint32_t roomId);

private:
    RoomModel();
    ~RoomModel();
    bool _exist(uint32_t roomId);
    uint32_t _genRoomId();

private:
    std::map<uint32_t, Room*> m_rooms;
};

#endif


