#include "RoomMgr.h"
#include "HYLog.h"
#include "ErrCodeDef.h"
#include "ChatEventDef.h"

RoomMgr::RoomMgr()
{
    EventMgr::Instance()->RegisterEvent(EVENT_TYPE_DESTORY_ROOM, this, [&](CHYEvent* event){
		OnDestroyRoomEvent(event);
	});
}

void RoomMgr::OnDestroyRoomEvent(CHYEvent* pEvent)
{
    DestroyRoomEvent* e = (DestroyRoomEvent*)pEvent;
    int nRoomID = e->nRoomID;

    AutoLock lock(m_lock);
    if (m_mapRoom.count(nRoomID) == 0)
    {
        LogWarn("OnDestroyRoomEvent Failed! room %d is not exist!", nRoomID);
        return;
    }
    LogInfo("OnDestroyRoomEvent room %d", nRoomID);
    ChatRoom* pRoom = m_mapRoom[nRoomID];
    pRoom->DestoryRoom();
    m_mapRoom.erase(nRoomID);
    m_roomIDMgr.Recycle(nRoomID);
    delete pRoom;
}

int RoomMgr::CreateRoom(int64_t sesID, int& roomID)
{
    AutoLock lock(m_lock);
    // 检查是否已经创建过，创建过直接返回已创建的房间
    for (auto itr : m_mapRoom)
    {
        if (itr.second->m_nCreaterID == sesID)
        {
            roomID = itr.second->m_id;
            return ROOM_AREADY_EXIST;
        }   
    }
    
    int nID = m_roomIDMgr.GetID();
    ChatRoom*pRoom = new ChatRoom();
    pRoom->Init(nID, sesID);
    m_mapRoom[nID] = pRoom;
    roomID = nID;

    return ROOM_CREATE_SUCC;
}

int RoomMgr::JoinRoom(int64_t sesID, int& roomID, ClientUser* user)
{
    AutoLock lock(m_lock);

    // 检查是否已经在某个房间中，如果在某个房间，直接返回该房间号
    for (auto itr : m_mapRoom)
    {
        if (itr.second->HasUser(sesID))
        {
            roomID = itr.second->m_id;
            return JOIN_ROOM_AREADY_IN;
        }   
    }

    if (m_mapRoom.count(roomID) == 0)
    {
        return JOIN_ROOM_NOT_FIND;
    }

    return m_mapRoom[roomID]->JoinRoom(sesID,user);
}

int RoomMgr::OnClientBreak(int64_t sesID)
{
    AutoLock lock(m_lock);

    // 如果在某个房间，直接离开
    for (auto itr : m_mapRoom)
    {
        itr.second->LeaveRoom(sesID);
    }
    return 0;
}

int RoomMgr::OnSendRoomMsg(int64_t sesID, int nRoomID, std::string& msg)
{
    LogInfo("OnSendRoomMsg %lld %d %s", sesID, nRoomID, msg.c_str());
    AutoLock lock(m_lock);
    if (m_mapRoom.count(nRoomID) == 0)
    {
        LogInfo("OnSendRoomMsg can not find room %lld %d %s", sesID, nRoomID, msg.c_str());
        return SEND_MSG_ROOM_NOT_FIND;
    }

    return m_mapRoom[nRoomID]->SendRoomMsg(sesID, msg);
}

bool RoomMgr::IsRoomAlive(int roomID)
{
    return false;
}

void RoomMgr::Check()
{
    AutoLock lock(m_lock);
    for (auto itr : m_mapRoom)
    {
        itr.second->Check();
    }   
}
