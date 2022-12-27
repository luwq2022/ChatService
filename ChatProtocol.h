#pragma once
#include "Protocol.h"

const int ORIGINE_CLIENT = 1;

const int ORIGINE_CHATSVR = 2001;

// 请求会话id
// REQ(Client->Server)：RequestSessionID 消息ID 101，客户端连接到服务后发送，无需其他参数
// ACK(Server->Client)：消息为json字符串{"sesID":"1000012345"}
const int MSG_TYPE_REQUEST_SESSIONID = 101;

// 创建聊天室：
// REQ：CreateRoom 消息ID 102，客户端获取会话id后发送，无需其他参数
// ACK：成功返回房间ID {"status":0, "roomID":100001}，失败返回对应错误码{"status":1}
// status 状态定义：
// 0，成功，非0为创建失败；
// 1，未找到会话id，需要先申请会话id；
const int MSG_TYPE_CREATE_ROOM = 102;

// 加入聊天室：
// REQ：JoinRoom 消息ID 103，需要指定要加入的房间id {"roomID":100001}
// ACK：返回加入状态，由消息头中wParam字段携带
// 0，加入成功
// 1，加入失败，聊天室未找到
// 2，加入失败，聊天室人数已达上限
const int MSG_TYPE_JOIN_ROOM = 103;

// 获取我的聊天室：
// REQ：GetMyRoom 消息ID 104，无需其他参数
// ACK：返回聊天室列表{"Create":100001,"his":[100002,100001],"in":100001}
const int MSG_TYPE_GET_MY_ROOM = 104;

// 聊天室内发送消息：
// REQ: SendMsg 消息ID 105 ，参数为聊天室id和消息内容{"roomID":100001, "msg":"this is the first msg"}
// ACK: wParam返回发送状态，0成功，1失败，不在聊天室内
const int MSG_TYPE_SEND_CHAT_MSG = 105;

// 聊天室内获取房间内用户名单：
// REQ：GetRoomUserList 消息ID 106，无需参数，如果没在聊天室内，则返回错误
// ACK：wParam返回获取状态，0成功，消息体为用户昵称列表 {"users":["张三","李四"]}
// 1 失败，不在聊天室内
const int MSG_TYPE_GET_ROOM_USER_LIST = 106;

// 广播消息：
// ACK： BroadcastMsg 消息ID 107， {"msg":"this is the first msg","sender":"张三"}
const int MSG_TYPE_BROADCAST_MSG = 107;