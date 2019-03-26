#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <list>
#include <map>
#include "httpRequest.h"
#include <QString>
#include "httpResponse.h"
#include "Config.h"
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <queue>
#include <conio.h>

#pragma comment(lib, "ws2_32.lib")

// 服务器可能会用到的一些stl组件
using std::list;
using std::pair;
using std::map;
using std::cout;
using std::endl;
using std::vector;
using std::queue;

constexpr int SUCCEED = 666;	// 相当于一个宏定义的标识，如果正确返回SUCCEED

class Server {
public:
	SOCKET listenSocket;	// 用于监听的SOCKET
	fd_set readfds;			// 读文件描述符
	fd_set writefds;		// 写文件描述符
	size_t rwCnt;			// 读写计数器
	queue<string> messageQueue;		// 收到的消息队列
	queue<string> rnrQueue;			// 回复的消息队列

	bool endSignal = false;			// 用于子线程结束的标记
	const string DEFAULT_PORT;		// 默认端口
	const size_t DEFAULT_BUFLEN;	// 默认的BUF长度
	const size_t MAX_CONNECTION;	// 最多接收的连接数
	const string SERVER_ADDR;		// 服务器的IP地址
	const u_long BLOCK_MODE;		// 阻塞模式
	const string HOME_PAGE;			// 主页和主目录
	virtual SOCKET acceptConnections(QString &info);		// 接受连接函数
	void receiveMessageFromClient(SOCKET client, size_t bufLen);	// 从客户端接收信息函数
	string sendMessageToClient(httpRequest *requestFromClient, string defaultPage);		// 把生成的HTTP信息发给客户端函数

public:
	vector<std::thread> *threadList;		// 存放线程的list
	Server(tempConfig tmp);			// constructor
	~Server();						// destructor

	int initWinsock();				// 初始化Winsock
	int createSocket(int &err);		// 创建Socket
	int listenOnSocket(int &err);	// 创建监听Socket
};

string GetClientAddress(SOCKET s, int &portNumber);		// 一个辅助的函数，用于获得客户端的Socket和IP信息

