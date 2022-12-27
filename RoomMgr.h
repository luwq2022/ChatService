#pragma once
#include <unordered_map>
#include "stdint.h"
#include <mutex>
#include "ChatRoom.h"
#include "RoomIDMgr.h"
#include "ClientUser.h"

typedef std::mutex Mutex;
typedef std::lock_guard<Mutex> AutoLock;
class CHYEvent;

class RoomMgr
{
public:
    RoomMgr();

public:
    int CreateRoom(int64_t sesID, int& roomID);
    
    int JoinRoom(int64_t sesID, int& roomID, ClientUser* user);

    int OnSendRoomMsg(int64_t sesID, int nRoomID, std::string& msg);

    int OnClientBreak(int64_t sesID);

    bool IsRoomAlive(int roomID);

    void Check();

    void OnDestroyRoomEvent(CHYEvent* pEvent);

public:
    std::unordered_map<int, ChatRoom*> m_mapRoom; // <roomid, roomPtr>
    Mutex m_lock;

private:
    RoomIDMgr m_roomIDMgr;
};