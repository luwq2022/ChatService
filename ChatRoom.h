#pragma once
#include <unordered_map>
#include <string>
#include "ClientUser.h"
#include <mutex>

typedef std::mutex Mutex;
typedef std::lock_guard<Mutex> AutoLock;

enum ROOM_STATUS
{
    RS_NORMAL = 1,
    RS_DESTROY,
};

class ChatRoom
{
public:
    ChatRoom();

    void Init(int nRoomID, int64_t nCreaterID);

public:
    int JoinRoom(int64_t id, ClientUser* user);

    int LeaveRoom(int64_t id);

    int DestoryRoom();

    void Check();

    int SendRoomMsg(int64_t sesID, std::string& msg);

    bool HasUser(int64_t userid);

public:
    int m_id;
    int64_t m_nCreaterID;

private:    
    std::unordered_map<int64_t, ClientUser*>m_users;
    Mutex m_lock;
    
private:
    time_t m_timeCreate;
    int m_nLiftTime;
    int m_nMaxPlayerCount;
    ROOM_STATUS m_eStatus;
};