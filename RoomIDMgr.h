#pragma once
#include <queue>
#include <mutex>

typedef std::mutex Mutex;
typedef std::lock_guard<Mutex> AutoLock;

class RoomIDMgr
{
public:
    RoomIDMgr();

    int GetID();

    void Recycle(int ID);

public:
    std::queue<int> m_queueIDs;
    Mutex m_lock;
};