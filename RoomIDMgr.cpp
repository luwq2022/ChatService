#include "RoomIDMgr.h"

const int MIN_ROOMID = 100000;
const int MAX_ROOMID = 999999;

 RoomIDMgr::RoomIDMgr()
 {
    for(int i = MIN_ROOMID; i <= MAX_ROOMID; ++i)
    {
        m_queueIDs.push(i);
    }
 }

int RoomIDMgr::GetID()
{
    AutoLock lock(m_lock);
    int id = m_queueIDs.front();
    m_queueIDs.pop();
    return id;
}

void RoomIDMgr::Recycle(int ID)
{
    AutoLock lock(m_lock);
    m_queueIDs.push(ID);
}