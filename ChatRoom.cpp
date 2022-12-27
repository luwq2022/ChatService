#include "ChatRoom.h"
#include "HYBuffer.h"
#include "Protocol.h"
#include "HYLog.h"
#include "HYFunc.h"
#include "ErrCodeDef.h"
#include "ChatEventDef.h"

const int TIME_LIFE_ROOM = 30 * 60 * 1000 * 1000; //30分钟
const int MAX_PLAYER_IN_ROOM_DEFAULT = 10;

const int SEND_MSG_USER_NOT_FIND = 2;

ChatRoom::ChatRoom()
{
    
}

void ChatRoom::Init(int nRoomID, int64_t nCreaterID)
{
    m_nCreaterID =  nCreaterID;
    m_id = nRoomID;
    m_timeCreate = GetTimeStamp();
    m_nLiftTime = TIME_LIFE_ROOM;
    m_nMaxPlayerCount = MAX_PLAYER_IN_ROOM_DEFAULT;
    m_eStatus = RS_NORMAL;
}

void ChatRoom::Check()
{
    AutoLock lk(m_lock);
    if (m_users.size() > 0)
    {
        // 房间有人，不用管
        return;
    }

    if (GetTimeStamp() - m_timeCreate > m_nLiftTime)
    {
        // 禁止进人，通知上层可以销毁房间
        if (m_eStatus != RS_DESTROY)
        {
            m_eStatus = RS_DESTROY;
            DestroyRoomEvent event;
            event.nType = EVENT_TYPE_DESTORY_ROOM;
            event.nDataLen = sizeof(DestroyRoomEvent) - sizeof(CHYEvent);
            event.nRoomID = m_id;
            EventMgr::Instance()->PostEvent(&event);
        }
    }
}

bool ChatRoom::HasUser(int64_t userid)
{
    AutoLock lk(m_lock);
    if (m_users.count(userid) > 0)
    {
        return true;
    }
    
    return false;
}

int ChatRoom::JoinRoom(int64_t id, ClientUser* user)
{
    AutoLock lk(m_lock);
    if (m_users.size() >= m_nMaxPlayerCount)
    {
        return JOIN_ROOM_MAX_USER;
    }

    if (m_eStatus == RS_DESTROY)
    {
        return JOIN_ROOM_DESTORY;
    }
    
    m_users[id] = user;

    return JOIN_ROOM_SUCC;
}

int ChatRoom::SendRoomMsg(int64_t sesID, std::string& msg)
{
    AutoLock lk(m_lock);
    CHYBuffer buf;
    if (m_users.count(sesID) == 0)
    {
        LogInfo("SendRoomMsg can not find user %lld %d %s", sesID, m_id, msg.c_str());
        return SEND_MSG_USER_NOT_FIND;
    }

    ClientUser* pUser = m_users[sesID];
    buf.SetHeaderLen(HYHEADERSIZE);
    buf.AppendFormatString("{\"sender\":\"%s\", \"msg\":\"%s\"}", pUser->m_strNickName.c_str(), msg.c_str());
    
    PHYHEADER pHeader = (PHYHEADER)buf.GetBufPtr();
    pHeader->dwLength = buf.GetDataLen();
    pHeader->wOrigine = ORIGINE_CHATSVR;
    pHeader->wType = MSG_TYPE_BROADCAST_MSG | HY_ACK;
    
    for (auto itr : m_users)
    {
        LogInfo("Room %d Send Room msg to %lld, %s", m_id, itr.first, msg.c_str());
        itr.second->SendMsg(buf.GetBufPtr(), buf.GetBufLen());
    }
    
    return 0;
}

int ChatRoom::LeaveRoom(int64_t sesid)
{
    LogInfo("LeaveRoom %d : %lld",m_id, sesid);
    AutoLock lk(m_lock);
    m_users.erase(sesid);
    return 0;
}

int ChatRoom::DestoryRoom()
{
    AutoLock lk(m_lock);
    m_users.clear();
    return 0;
}