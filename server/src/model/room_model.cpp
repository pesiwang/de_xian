#include "model/room_model.h"

//================== seat ================
Seat::Seat(uint32_t uid) {
    m_uid = uid;
}

Seat::~Seat() {

}

//================== room ================
Room::Room(uint32_t roomId, uint32_t roomType) {
    m_roomId = roomId;
    m_roomType = roomType;
}

Room::~Room() {

}

bool Room::hasUser(uint32_t uid) {
	if (m_seats.find(uid) == m_seats.end()) {
		return false;
	}

	return true;
}

bool Room::addUser(uint32_t uid) {
	if (!this->hasUser(uid)) {
		return false;
	}

	m_seats[uid] = new Seat(uid);

	return true;
}

bool Room::removeUser(uint32_t uid) {
	if (!this->hasUser(uid)) {
		return false;
	}

	Seat* s = m_seats[uid];
	m_seats.erase(uid);
	delete s;

	return true;
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

Room* RoomModel::getRoom(uint32_t roomId) {
	if (!this->_exist(roomId)) {
		return nullptr;
	}

	return m_rooms[roomId];
}

int RoomModel::enterRoom(uint32_t roomId, uint32_t uid) {
	Room* room = this->getRoom(roomId);
	if (!room) {
		return 1;
	}

	if (!room->addUser(uid)) {
		return 2;
	}

	return 0;
}

