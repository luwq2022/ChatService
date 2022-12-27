#include "ChatService.h"
#include "ClientUser.h"
#include "Protocol.h"
#include <iostream>
#include "HYLog.h"
#include "HYFunc.h"


ChatService* g_pService = nullptr;

ChatService::ChatService()
{
    g_pService = this;
    m_nPort = 21100;
    SetLogName("./ChatService.log");
}


CLinkUser* ChatService::CreateUser(int nOrigin)
{
    switch(nOrigin)
    {
        case ORIGINE_CLIENT:
        return new ClientUser();
    }
    return nullptr;
}

void ChatService::OnTimeOut(time_t tNow)
{
   m_roomMgr.Check();
}

int ChatService::OnCreateRoom(int64_t sesID, int& nRoomID)
{
    return m_roomMgr.CreateRoom(sesID, nRoomID);
}

 int ChatService::JoinRoom(int64_t sesID, int& nRoomID, ClientUser* pUser)
 {
    return m_roomMgr.JoinRoom(sesID, nRoomID, pUser);
 }

 int ChatService::OnSendRoomMsg(int64_t sesID, int nRoomID, std::string& msg)
 {
    return m_roomMgr.OnSendRoomMsg(sesID, nRoomID, msg);
 }

 int ChatService::OnClientBreak(int64_t sesID)
 {
    return m_roomMgr.OnClientBreak(sesID);
 }