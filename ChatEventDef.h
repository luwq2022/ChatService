#pragma once
#include "HYEvent.h"

const int EVENT_TYPE_DESTORY_ROOM = 101;

struct DestroyRoomEvent : public CHYEvent
{
    int nRoomID;
};
