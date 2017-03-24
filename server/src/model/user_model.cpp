#include "model/user_model.h"

//====================user=====================

User::User(uint32_t uid) {
    m_uid = uid;
}

User::~User() {

}

uint32_t User::getId() {
    return m_uid;
}

//====================user model=====================
UserModel::UserModel() {

}

UserModel::~UserModel() {

}

bool UserModel::_exist(uint32_t uid) {
    if (m_users.find(uid) != m_users.end()) {
        return true;
    }

    return false;
}

int UserModel::add(uint32_t uid) {
    if (this->_exist(uid)) {
        return 1;
    }

    User* u = new User(uid);
    m_users[uid] = u;

    return 0;
}

int UserModel::remove(uint32_t uid) {
    if (!this->_exist(uid)) {
        return 1;
    }

    User* u = m_users[uid];
    m_users.erase(uid);
    delete u;
}



