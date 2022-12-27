#pragma once
#include "NetService.h"
#include "RoomMgr.h"

class ClientUser;
class ChatService :
    public CNetService
{
public:
    ChatService();

public:
    virtual CLinkUser* CreateUser(int nOrigin);

public:
    virtual void OnTimeOut(time_t tNow);
    
public:
    int OnCreateRoom(int64_t sesID, int& nRoomID);

    int JoinRoom(int64_t sesID, int& nRoomID, ClientUser* pUser);

    int OnSendRoomMsg(int64_t sesID, int nRoomID, std::string& msg);

    int OnClientBreak(int64_t sesID);

    void OnGetMyRoom();

    void OnGetRoomUserList();

private:
    RoomMgr m_roomMgr;
};

extern ChatService* g_pService;