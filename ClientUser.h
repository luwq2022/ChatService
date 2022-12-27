#pragma once
#include "LinkUser.h"
#include <string>
#include "ChatProtocol.h"

class ClientUser : public CLinkUser
{
public:
    ClientUser();

public:
    virtual bool OnMsg(char* msg, int nLen);

    virtual bool OnBreak();
    
public:
    bool OnRequestSessionID();

    bool OnCreateRoom();

    bool OnJoinRoom(PHYHEADER pHeader);

    bool OnChatMsg(PHYHEADER pHeader);

    int64_t GetSesID(){return m_sesID;}

public:
    std::string m_strNickName;
};