#ifndef USER_MODEL_H
#define USER_MODEL_H
#include <map>

class User {
public:
    User(uint32_t uid);
    ~User();
    uint32_t getId();

private:
    uint32_t m_uid;
};

class UserModel {
public:
    static UserModel* instance() {
        static UserModel self;
        return &self;
    }

    int add(uint32_t uid);
    int remove(uint32_t uid);

private:
    UserModel();
    ~UserModel();
    bool _exist(uint32_t uid);

private:
    std::map<uint32_t, User*> m_users;

};

#endif

