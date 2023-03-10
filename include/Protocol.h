#pragma once

using DWORD = unsigned int;
using WORD = unsigned short;

typedef struct tagHEADER
{
    WORD	wOrigine;			//消息来源
    WORD	wType;				//消息类型
    DWORD	dwLength;			//消息体长度
}HYHEADER,*PHYHEADER;

const int HYHEADERSIZE = sizeof(HYHEADER);

#define HY_ACK  0x8000