#include "ClientUser.h"
#include "HYLog.h"
#include "HYBuffer.h"
#include <iostream>
#include "ChatService.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

ClientUser::ClientUser()
{
    LogInfo("create a ClientUser ");
    
    if (m_strNickName.empty())
    {
        char szName[64];
        sprintf(szName, "%lld", m_sesID);
        m_strNickName = szName;
    }
}

bool ClientUser::OnBreak()
{
    CLinkUser::OnBreak();
    LogInfo("Client User break %d", m_sesID);
    g_pService->OnClientBreak(m_sesID);
    return true;
}

bool ClientUser::OnMsg(char* msg, int nLen)
{
    //LogInfo("Client User Recv msg %d", nLen);
    PHYHEADER pHeader = (PHYHEADER)msg;
    switch (pHeader->wType){
        case MSG_TYPE_REQUEST_SESSIONID:
            OnRequestSessionID();
            break;
        case MSG_TYPE_CREATE_ROOM:
            OnCreateRoom();
            break;
        case MSG_TYPE_JOIN_ROOM:
            OnJoinRoom(pHeader);
            break;
        case MSG_TYPE_SEND_CHAT_MSG:
            OnChatMsg(pHeader);
            break;
    }
    return true;
}

bool ClientUser::OnCreateRoom()
{
    int nRoomID = 0;
    int nRet = g_pService->OnCreateRoom(m_sesID, nRoomID);
    
    LogInfo("ClientUser::OnCreateRoom: %lld, room %d", m_sesID, nRoomID);
    
    CHYBuffer buf;
    buf.SetHeaderLen(HYHEADERSIZE);
    buf.AppendFormatString("{\"status\":\"%d\", \"roomid\":%d}", nRet, nRoomID);

    PHYHEADER pHeader = (PHYHEADER)buf.GetBufPtr();
    pHeader->dwLength = buf.GetDataLen();
    pHeader->wOrigine = ORIGINE_CHATSVR;
    pHeader->wType = MSG_TYPE_CREATE_ROOM | HY_ACK;

    SendMsg(buf.GetBufPtr(), buf.GetBufLen());
    return true;
}

bool ClientUser::OnJoinRoom(PHYHEADER pHeader)
{
    char* pData = (char*)(pHeader + 1);
    std::cout << pData << std::endl;
    json j = json::parse(pData);
    int nRoomID = 0;
    if (j["roomID"].is_number_integer())
    {
        nRoomID = j["roomID"].get<int>();
    }
    
    LogInfo("ClientUser::OnJoinRoom %lld, %s", m_sesID, pData);
    
    CHYBuffer buf;
    buf.SetHeaderLen(HYHEADERSIZE);
    
    if (nRoomID <= 0)
    {
        buf.AppendFormatString("{\"status\":1}");
    }

    int nRet = g_pService->JoinRoom(m_sesID, nRoomID, this);

    buf.AppendFormatString("{\"status\":%d, \"roomID\":%d}", nRet, nRoomID);
    PHYHEADER pHdr = (PHYHEADER)buf.GetBufPtr();
    pHdr->dwLength = buf.GetDataLen();
    pHdr->wOrigine = ORIGINE_CHATSVR;
    pHdr->wType = MSG_TYPE_JOIN_ROOM | HY_ACK;

    SendMsg(buf.GetBufPtr(), buf.GetBufLen());

    return true;
}

 bool ClientUser::OnChatMsg(PHYHEADER pHeader)
 {
    char* pData = (char*)(pHeader + 1);
    std::cout << pData << std::endl;
    json j = json::parse(pData);
    
    int nRoomID = 0;
    std::string msg;
   
    if (j["msg"].is_string())
    {
        LogInfo("recv client msg :%s", pData);
        msg = j["msg"];
    }
    if (j["roomID"].is_number_integer())
    {
        nRoomID = j["roomID"];     
    }
    
    int nRet = g_pService->OnSendRoomMsg(m_sesID, nRoomID, msg);

    CHYBuffer buf;
    buf.SetHeaderLen(HYHEADERSIZE);

    buf.AppendFormatString("{\"status\":%d}", nRet);
    PHYHEADER pHdr = (PHYHEADER)buf.GetBufPtr();
    pHdr->dwLength = buf.GetDataLen();
    pHdr->wOrigine = ORIGINE_CHATSVR;
    pHdr->wType = MSG_TYPE_SEND_CHAT_MSG | HY_ACK;

    SendMsg(buf.GetBufPtr(), buf.GetBufLen());
    return true;
 }

bool ClientUser::OnRequestSessionID()
{
    CHYBuffer buf;
    buf.SetHeaderLen(HYHEADERSIZE);
    buf.AppendFormatString("{\"sesID\":\"%lld\"}", m_sesID);
 
    LogInfo("ClientUser::OnRequestSessionID: %lld", m_sesID);
    
    PHYHEADER pHeader = (PHYHEADER)buf.GetBufPtr();
    pHeader->dwLength = buf.GetDataLen();
    pHeader->wOrigine = ORIGINE_CHATSVR;
    pHeader->wType = MSG_TYPE_REQUEST_SESSIONID | HY_ACK;

    SendMsg(buf.GetBufPtr(), buf.GetBufLen());

    return true;
}

