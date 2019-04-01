#include "myServer.h"

std::mutex mtx;

string homepage;

enum err { wsaerr, vererr, listenerr, binderr, ioct };	// 一些错误号码的宏定义

/*
*	Server's constructor
*	根据配置文件的配置，初始化一些基本参数，包括IP,端口号,主目录等
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
*	释放服务器申请的一些资源 
*/
Server::~Server() {
	/* 主要是释放线程list */
	if (threadList) {
		threadList->clear();
		delete threadList;
		threadList = nullptr;
	}
}

/*
*	初始化Winsock环境
*/
int Server::initWinsock() {
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		/* Winsock环境创建错误 */
		return 1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		/* 缺少Winsock的库 */
		WSACleanup();
		return 1;
	}
	return SUCCEED;
}

/*
*	创建一个属于服务器的Socket
*	用于之后绑定
*/
int Server::createSocket(int &err) {
	int iResult = 0;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;	// IPV4
	hints.ai_socktype = SOCK_STREAM;	// TCP流
	hints.ai_protocol = IPPROTO_TCP;	// TCP
	hints.ai_flags = AI_PASSIVE;

	// 解析本地的IP地址，TCP流等等
	iResult = getaddrinfo(NULL, this->DEFAULT_PORT.c_str(), &hints, &result);
	if (iResult != 0) {
		/* 获得信息失败 */
		err = iResult;
		WSACleanup();
		return 1;
	}

	// 创建一个用于监听的Socket
	this->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (this->listenSocket == INVALID_SOCKET) {
		/* 创建监听Socket失败 */
		err = WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		return 2;
	}
	// 建立TCP连接的Socket
	iResult = bind(this->listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		/* 建立TCP连接失败 */
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
*	绑定用于监听的Socket
*/
int Server::listenOnSocket(int &err) {
	// listen函数用于监听
	if (listen(this->listenSocket, MAX_CONNECTION) == SOCKET_ERROR) {
		// 监听失败
		closesocket(this->listenSocket);
		WSACleanup();
		return 1;
	}
	return SUCCEED;
}

/*
*	获得客户端的Socket信息
*	主要用于在屏幕输出Debug
*/
string GetClientAddress(SOCKET s, int &portNumber) {
	string clientAddress;
	sockaddr_in clientAddr;
	int nameLen, rtn;

	nameLen = sizeof(clientAddr);
	rtn = getpeername(s, (LPSOCKADDR)&clientAddr, &nameLen);	// 获取Client的IP地址信息
	char str[INET_ADDRSTRLEN];
	if (rtn != SOCKET_ERROR) {
		clientAddress += inet_ntop(AF_INET, &clientAddr.sin_addr, str, sizeof(str));	// IP信息修改格式
	}
	portNumber = ntohs(clientAddr.sin_port);	// 获得端口号

	return clientAddress;
}


/*
*	接收客户端的连接
*	主线程使用，每当接收到一个新连接就创建一个子线程
*/
SOCKET Server::acceptConnections(QString &info) {
	SOCKET clientSocket;

	u_long blockMode = BLOCK_MODE;

	clientSocket = INVALID_SOCKET;

	/* 收到客户端的连接请求 */
	if (FD_ISSET(this->listenSocket, &this->readfds)) {
		/* 接收请求 */
		clientSocket = accept(this->listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			// 客户端Socket不合法
			closesocket(clientSocket);
			WSACleanup();
		}

		/* 设置非阻塞模式 */
		else {
			if (ioctlsocket(clientSocket, FIONBIO, &blockMode) == SOCKET_ERROR)
				;//非阻塞设置失败
			int portNumber = 0;
			mtx.lock();
			string clientAddress = GetClientAddress(clientSocket, portNumber);
			clientAddress += (":" + to_string(portNumber) + " has come to service\n" ); // 输出信息
			info += QString::fromStdString(clientAddress);
			mtx.unlock();
		}
	}

	return clientSocket;
}

/*
*	从客户端接收信息 
*	子线程处理，是一个死循环
*	推出条件有2，1.十秒内没有连接，2.收到了主线程的end信号
*/
void Server::receiveMessageFromClient(SOCKET client, size_t bufLen)

{
	while (true) {

		fd_set wfds;
		fd_set rfds;

		// 清零文件描述符
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);

		// 客户端可读可写
		FD_SET(client, &rfds);
		FD_SET(client, &wfds);

		const timeval timeOut = { 10, 0 };	// 超时设置为10s
		int portNumber = 0;

		select(0, &rfds, nullptr, nullptr, &timeOut);	// 10秒内不可读
		select(0, nullptr, &wfds, nullptr, &timeOut);	// 10秒内不可写

		if (endSignal == true) {
			// 如果收到了end的信号
			mtx.lock();
			string s = GetClientAddress(client, portNumber);
			string t = ("Close connection with " + s + ":" + to_string(portNumber) + ".\n");
			// 关闭连接提示
			this->messageQueue.push(t);
			mtx.unlock();
			closesocket(client);
			break;
		}

		if (!(FD_ISSET(client, &rfds) && FD_ISSET(client, &wfds))) {
			/* 文件描述符失效，即不可读也不可写 */
			mtx.lock();
			string s = GetClientAddress(client, portNumber);
			// 屏幕上输出信息
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
				/* 客户端发来信息 */
				currentRecvbuf[rcvlen] = '\0';
				httpRequest* hr = new httpRequest;
				hr->phraseMessage(string(currentRecvbuf), client);	// 新建一个Http请求消息类，解析它
				mtx.lock();
				// 输出一些请求参数
				string tmp = "\n\n\n  Requst Params: ";
				tmp += ("Method: " + hr->method  + "\n");
				tmp += ("URL: " + hr->url + "\n");
				tmp += ("Version: " + hr->version + "\n");
				for (auto itor = hr->rHeader->begin(); itor != hr->rHeader->end(); itor++) {
					tmp += itor->first + ": " + itor->second + "\n";
				}
				if (hr->method == string("GET"))	//	如果是GET，没有数据段
					tmp += "No data\n";
				else
					tmp += "Data: " + hr->data + "\n";
				this->rnrQueue.push(tmp);
				mtx.unlock();
				sendMessageToClient(hr, homepage);	// 解析后的结果产生回复,返还给客户端
				delete hr;
				delete currentRecvbuf;
			}
			else if (rcvlen == SOCKET_ERROR) {
				// 接受错误
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
*	根据解析后的结果产生回复,返还给客户端
*	加入队列中显示
*/
string Server::sendMessageToClient(httpRequest *requestFromClient, string defaultPage) {
	httpResponse *hrsps = new httpResponse;
	mtx.lock();
	string tmp;
	string msg = hrsps->generateMessage(requestFromClient, defaultPage, tmp);
	this->rnrQueue.push(tmp);
	mtx.unlock();
	send(hrsps->clientSocket, msg.c_str(), msg.length(), 0);	// 发送生成的信息

	delete hrsps;
	
	std::this_thread::sleep_for(std::chrono::milliseconds(10));	// 休眠10毫秒
	return tmp;
}
