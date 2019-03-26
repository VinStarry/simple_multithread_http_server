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

// ���������ܻ��õ���һЩstl���
using std::list;
using std::pair;
using std::map;
using std::cout;
using std::endl;
using std::vector;
using std::queue;

constexpr int SUCCEED = 666;	// �൱��һ���궨��ı�ʶ�������ȷ����SUCCEED

class Server {
public:
	SOCKET listenSocket;	// ���ڼ�����SOCKET
	fd_set readfds;			// ���ļ�������
	fd_set writefds;		// д�ļ�������
	size_t rwCnt;			// ��д������
	queue<string> messageQueue;		// �յ�����Ϣ����
	queue<string> rnrQueue;			// �ظ�����Ϣ����

	bool endSignal = false;			// �������߳̽����ı��
	const string DEFAULT_PORT;		// Ĭ�϶˿�
	const size_t DEFAULT_BUFLEN;	// Ĭ�ϵ�BUF����
	const size_t MAX_CONNECTION;	// �����յ�������
	const string SERVER_ADDR;		// ��������IP��ַ
	const u_long BLOCK_MODE;		// ����ģʽ
	const string HOME_PAGE;			// ��ҳ����Ŀ¼
	virtual SOCKET acceptConnections(QString &info);		// �������Ӻ���
	void receiveMessageFromClient(SOCKET client, size_t bufLen);	// �ӿͻ��˽�����Ϣ����
	string sendMessageToClient(httpRequest *requestFromClient, string defaultPage);		// �����ɵ�HTTP��Ϣ�����ͻ��˺���

public:
	vector<std::thread> *threadList;		// ����̵߳�list
	Server(tempConfig tmp);			// constructor
	~Server();						// destructor

	int initWinsock();				// ��ʼ��Winsock
	int createSocket(int &err);		// ����Socket
	int listenOnSocket(int &err);	// ��������Socket
};

string GetClientAddress(SOCKET s, int &portNumber);		// һ�������ĺ��������ڻ�ÿͻ��˵�Socket��IP��Ϣ

