#include "myServer.h"

std::mutex mtx;

string homepage;

enum err { wsaerr, vererr, listenerr, binderr, ioct };	// һЩ�������ĺ궨��

/*
*	Server's constructor
*	���������ļ������ã���ʼ��һЩ��������������IP,�˿ں�,��Ŀ¼��
*/
Server::Server(tempConfig tmp) :DEFAULT_BUFLEN(1024),
MAX_CONNECTION(500),
SERVER_ADDR(tmp.serverAddress),
BLOCK_MODE(1),
HOME_PAGE(tmp.homePage),
DEFAULT_PORT(tmp.defaultPort)
{
	threadList = new vector<std::thread>;
	homepage = this->HOME_PAGE;
	this->endSignal = false;
}

/*
*	Server's destructor
*	�ͷŷ����������һЩ��Դ 
*/
Server::~Server() {
	/* ��Ҫ���ͷ��߳�list */
	if (threadList) {
		threadList->clear();
		delete threadList;
		threadList = nullptr;
	}
}

/*
*	��ʼ��Winsock����
*/
int Server::initWinsock() {
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		/* Winsock������������ */
		return 1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		/* ȱ��Winsock�Ŀ� */
		WSACleanup();
		return 1;
	}
	return SUCCEED;
}

/*
*	����һ�����ڷ�������Socket
*	����֮���
*/
int Server::createSocket(int &err) {
	int iResult = 0;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;	// IPV4
	hints.ai_socktype = SOCK_STREAM;	// TCP��
	hints.ai_protocol = IPPROTO_TCP;	// TCP
	hints.ai_flags = AI_PASSIVE;

	// �������ص�IP��ַ��TCP���ȵ�
	iResult = getaddrinfo(NULL, this->DEFAULT_PORT.c_str(), &hints, &result);
	if (iResult != 0) {
		/* �����Ϣʧ�� */
		err = iResult;
		WSACleanup();
		return 1;
	}

	// ����һ�����ڼ�����Socket
	this->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (this->listenSocket == INVALID_SOCKET) {
		/* ��������Socketʧ�� */
		err = WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		return 2;
	}
	// ����TCP���ӵ�Socket
	iResult = bind(this->listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		/* ����TCP����ʧ�� */
		err = WSAGetLastError();
		freeaddrinfo(result);
		closesocket(this->listenSocket);
		WSACleanup();
		return 3;
	}
	else {
		cout << "Listen socket is set and bind ok for server!" << endl;
	}
	freeaddrinfo(result);
	return SUCCEED;
}

/*
*	�����ڼ�����Socket
*/
int Server::listenOnSocket(int &err) {
	// listen�������ڼ���
	if (listen(this->listenSocket, MAX_CONNECTION) == SOCKET_ERROR) {
		// ����ʧ��
		closesocket(this->listenSocket);
		WSACleanup();
		return 1;
	}
	return SUCCEED;
}

/*
*	��ÿͻ��˵�Socket��Ϣ
*	��Ҫ��������Ļ���Debug
*/
string GetClientAddress(SOCKET s, int &portNumber) {
	string clientAddress;
	sockaddr_in clientAddr;
	int nameLen, rtn;

	nameLen = sizeof(clientAddr);
	rtn = getpeername(s, (LPSOCKADDR)&clientAddr, &nameLen);	// ��ȡClient��IP��ַ��Ϣ
	char str[INET_ADDRSTRLEN];
	if (rtn != SOCKET_ERROR) {
		clientAddress += inet_ntop(AF_INET, &clientAddr.sin_addr, str, sizeof(str));	// IP��Ϣ�޸ĸ�ʽ
	}
	portNumber = ntohs(clientAddr.sin_port);	// ��ö˿ں�

	return clientAddress;
}


/*
*	���տͻ��˵�����
*	���߳�ʹ�ã�ÿ�����յ�һ�������Ӿʹ���һ�����߳�
*/
SOCKET Server::acceptConnections(QString &info) {
	SOCKET clientSocket;

	u_long blockMode = BLOCK_MODE;

	clientSocket = INVALID_SOCKET;

	/* �յ��ͻ��˵��������� */
	if (FD_ISSET(this->listenSocket, &this->readfds)) {
		/* �������� */
		clientSocket = accept(this->listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			// �ͻ���Socket���Ϸ�
			closesocket(clientSocket);
			WSACleanup();
		}

		/* ���÷�����ģʽ */
		else {
			if (ioctlsocket(clientSocket, FIONBIO, &blockMode) == SOCKET_ERROR)
				;//����������ʧ��
			int portNumber = 0;
			mtx.lock();
			string clientAddress = GetClientAddress(clientSocket, portNumber);
			clientAddress += (":" + to_string(portNumber) + " has come to service\n" ); // �����Ϣ
			info += QString::fromStdString(clientAddress);
			mtx.unlock();
		}
	}

	return clientSocket;
}

/*
*	�ӿͻ��˽�����Ϣ 
*	���̴߳�����һ����ѭ��
*	�Ƴ�������2��1.ʮ����û�����ӣ�2.�յ������̵߳�end�ź�
*/
void Server::receiveMessageFromClient(SOCKET client, size_t bufLen)

{
	while (true) {

		fd_set wfds;
		fd_set rfds;

		// �����ļ�������
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);

		// �ͻ��˿ɶ���д
		FD_SET(client, &rfds);
		FD_SET(client, &wfds);

		const timeval timeOut = { 10, 0 };	// ��ʱ����Ϊ10s
		int portNumber = 0;

		select(0, &rfds, nullptr, nullptr, &timeOut);	// 10���ڲ��ɶ�
		select(0, nullptr, &wfds, nullptr, &timeOut);	// 10���ڲ���й

		if (endSignal == true) {
			// ����յ���end���ź�
			mtx.lock();
			string s = GetClientAddress(client, portNumber);
			string t = ("Close connection with " + s + ":" + to_string(portNumber) + ".\n");
			// �ر�������ʾ
			this->messageQueue.push(t);
			mtx.unlock();
			closesocket(client);
			break;
		}

		if (!(FD_ISSET(client, &rfds) && FD_ISSET(client, &wfds))) {
			/* �ļ�������ʧЧ�������ɶ�Ҳ����д */
			mtx.lock();
			string s = GetClientAddress(client, portNumber);
			// ��Ļ�������Ϣ
			if (endSignal == true) {
				string t = ("Close connection with " + s + ":" + to_string(portNumber) + ".\n");
				this->messageQueue.push(t);
			}
			else {
				string t = ("Close connection with " + s + ":" + to_string(portNumber) + " because of timeout.\n");
				this->messageQueue.push(t);
			}
			mtx.unlock();
			closesocket(client);
			break;
		}
		else {
			char *currentRecvbuf = new char[bufLen];
			auto rcvlen = recv(client, currentRecvbuf, bufLen, 0);

			if (rcvlen > 0) {
				/* �ͻ��˷�����Ϣ */
				currentRecvbuf[rcvlen] = '\0';
				httpRequest* hr = new httpRequest;
				hr->phraseMessage(string(currentRecvbuf), client);	// �½�һ��Http������Ϣ�࣬������
				mtx.lock();
				// ���һЩ�������
				string tmp = "\n\n\n  Requst Params: ";
				tmp += ("Method: " + hr->method  + "\n");
				tmp += ("URL: " + hr->url + "\n");
				tmp += ("Version: " + hr->version + "\n");
				for (auto itor = hr->rHeader->begin(); itor != hr->rHeader->end(); itor++) {
					tmp += itor->first + ": " + itor->second + "\n";
				}
				if (hr->method == string("GET"))	//	�����GET��û�����ݶ�
					tmp += "No data\n";
				else
					tmp += "Data: " + hr->data + "\n";
				this->rnrQueue.push(tmp);
				mtx.unlock();
				sendMessageToClient(hr, homepage);	// ������Ľ�������ظ�,�������ͻ���
				delete hr;
				delete currentRecvbuf;
			}
			else if (rcvlen == SOCKET_ERROR) {
				// ���ܴ���
				int portNumber = 0;
				string s = GetClientAddress(client, portNumber);
				closesocket(client);
				break;
			}
		}

	}
	return;
}

/*
*	���ݽ�����Ľ�������ظ�,�������ͻ���
*	�����������ʾ
*/
string Server::sendMessageToClient(httpRequest *requestFromClient, string defaultPage) {
	httpResponse *hrsps = new httpResponse;
	mtx.lock();
	string tmp;
	string msg = hrsps->generateMessage(requestFromClient, defaultPage, tmp);
	this->rnrQueue.push(tmp);
	mtx.unlock();
	send(hrsps->clientSocket, msg.c_str(), msg.length(), 0);	// �������ɵ���Ϣ

	delete hrsps;
	
	std::this_thread::sleep_for(std::chrono::milliseconds(10));	// ����10����
	return tmp;
}