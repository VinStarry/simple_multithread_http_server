#include "QtGuiApplication1.h"

std::mutex mtx2;

/*constructor
*	用于构造一些QT的组建
*/
QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);	// 建立ui
	this->outputLabel = this->findChild<QLabel *>("outputLabel");	// 第一个输出框 config信息
	this->outputLabel2 = this->findChild<QLabel *>("outputLabel2");	// 第二个输出框 输出一些监听信息
	this->outputLabel3 = this->findChild<QLabel *>("outputLabel3");	// 第三个输出框	http请求实时信息
	this->outputLabel4 = this->findChild<QLabel *>("outputLabel4");	// 第四个输出框	http回复实时信息
	this->OKButton = this->findChild<QPushButton *>("OKbutton");	// OK按钮
	this->Start = this->findChild<QPushButton *>("Start");			// Start按钮
	this->Close = this->findChild<QPushButton *>("Close");			// Close按钮
	this->ip1 = this->findChild<QLineEdit *>("ip1");				// 一下为IP地址的四段 ip1.ip2.ip3.ip4
	this->ip2 = this->findChild<QLineEdit *>("ip2");
	this->ip3 = this->findChild<QLineEdit *>("ip3");
	this->ip4 = this->findChild<QLineEdit *>("ip4");
	this->port1 = this->findChild<QLineEdit *>("port1");			// 端口
	this->homepage = this->findChild<QLineEdit *>("homepage");		// 主目录主页
	this->scrollArea = this->findChild<QScrollArea *>("scrollArea");	// 滚筒框
	this->centralWidget = this->findChild<QScrollArea *>("centralWidget");	// 中心

	QObject::connect(OKButton, SIGNAL(clicked()), this, SLOT(OKButtonClicked()));	// 3个槽函数对应OK、Start、Close按下后发生的事件
	QObject::connect(Start, SIGNAL(clicked()), this, SLOT(startServer()));
	QObject::connect(Close, SIGNAL(clicked()), this, SLOT(closeServer()));

	//scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	//scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	this->status = true;		// 初始化一些状态字段
	this->configOK = false;
	this->endsignal = false;
	this->alreadyStart = false;

	main = new vector<std::thread>;	// 线程区

}

/*
*	槽函数 当OK按钮被按下后发生的事情
*	Config后才OK，如果已经Config过了，提示Already Config
*/
void QtGuiApplication1::OKButtonClicked() {
	// config file
	if (this->configOK == true) {
		/* Config后才OK，如果已经Config过了，提示Already Config */
		info += "Already configed\n";
		return;
	}
	/* 否则 配置成功 */
	QString addr = ip1->text() + "." + ip2->text() + "." + ip3->text() + "." + ip4->text();
	tempConfig tmp;
	/* 填充一些信息 */
	this->info = "Config OK!\n";
	info += tmp.readConfigFile(addr, port1->text(), homepage->text());
	this->server = new Server(tmp);
	/* 初始化winsock */
	if (this->server->initWinsock() != SUCCEED) {
		info += "WSAStartup failed with error!\n";
		this->status = false;
		return;
	}
	else {
		info += "The Winsock 2.2 dll was found okay!\n";
	}
	int err = 0;	// 错误字段
	int rslt = this->server->createSocket(err); // 建立socket
	if (rslt == 1) {
		info += "Getaddrinfo failed with: ";
		info += err;
		info += "\n";
		this->status = false;
		return;
	}
	else if (rslt == 2) {
		info += "Error at create socket for server with: "; // 建立socket错误
		info += err;
		info += "\n";
		this->status = false;
		return;
	}
	else if (rslt == 3) {	// bind一个端口失败
		info += "Bind failed with error: ";
		info += err;
		info += "\n";
		this->status = false;
		return;
	}
	else {	// 都成功
		info += "Listen socket is set and bind ok for server!";
	}
	this->configOK = true;	// 配置成功
	this->outputLabel->setText(info);	// 输出信息
}

/*
*	配置完成了之后可以按Start打开Server
*   打开Server后服务器需要完成监听等操作
*/
void QtGuiApplication1::startServer() {
	if (!this->configOK) {
		/* 还没有进行配置 */
		start_s = "Config first!\n";
		outputLabel2->setText(start_s);
		return;
	}
	else if (!this->status) {
		/* 配置失败 */
		start_s = "Config first!\n";
		outputLabel2->setText(start_s);
		return;
	}
	else if (this->alreadyStart) {
		/* 当前服务器已经处于打开的状态 */
		start_s = "Already Start!\n";
		outputLabel2->setText(start_s);
		return;
	}
	else {
		/* 开启服务器 */
		start_s = "Starting Server...\n";
		int err = 0;
		int rslt = this->server->listenOnSocket(err); // 让之前生成的端口监听
		if (rslt == 1) {
			/* 监听失败 */
			info += "Listen failed with error: ";
			info += err;
			info += "\n";
			this->status = false;
			outputLabel2->setText(start_s);
			return;
		}
		else {
			start_s += "Listen sockets set up succeed!\n"; // 监听成功
		}
		u_long blockMode = this->server->BLOCK_MODE;//none-block mode，设置非阻塞
		int rtn;

		if ((rtn = ioctlsocket(this->server->listenSocket, FIONBIO, &blockMode) == SOCKET_ERROR)) {
			// 设置非阻塞模式失败
			start_s += "ioctlsocket() failed with error: ";
			start_s += WSAGetLastError();
			start_s += "\n";
			outputLabel2->setText(start_s);
			return;
		}
		start_s += "ioctlsocket() for server socket ok! Waiting for client connection and data...\n"; // 成功
		outputLabel2->setText(start_s);
		alreadyStart = true;
		main->push_back(std::thread(&QtGuiApplication1::loopBegin, this));	// 主线程开始Loop
	}
}

/* 
*	关闭服务器
*	当服务器处于开启状态的时候关闭服务器
*/
void QtGuiApplication1::closeServer() {
	if (alreadyStart == false && configOK == true) {
		/*如果没有打开服务器忽略该按钮*/
		return;
	}
	QString close_s = "Server Closed\n";
	this->start_s += close_s;
	outputLabel2->setText(start_s);
	this->server->endSignal = true;	// 给子线程发送endSignal
	for (auto itor = this->main->begin(); itor != this->main->end(); itor++)
		itor->join();
	// 等所有子线程结束
	alreadyStart = false;	// 将开启状态关闭
	this->configOK = false;	// 需要重新配置才能打开
	server->~Server();		// 析构
	delete server;
	server = nullptr;
	main->clear();			// 线程list析构
}

/*
*	主函数是由主线程进行的loopBegin()
*	每接收到一个连接,就建立一个线程
*	每一个子线程负责从messageQueue中取出信息
*	进行解析，解析完了以后发送给客户端
*	解析过程中的一些记录信息输出在了out3和out4中
*	最后收到endSignal以后结束子线程
*/
int QtGuiApplication1::loopBegin() {
	QString out3 = "";
	QString out4 = "";
	while (!this->server->endSignal) {

		mtx2.lock();	// 互斥锁
		while (!this->server->messageQueue.empty()) {
			/* 每次从消息队列中取出一条消息 */
			string tmp = this->server->messageQueue.front();
			out3 += QString::fromStdString(tmp);
			outputLabel3->setText(out3);	// 输出信息
			this->server->messageQueue.pop();
		}
		while (!this->server->rnrQueue.empty()) {
			/* 将一些回复信息输出在屏幕上 */
			string tmp = this->server->rnrQueue.front();
			out4 += QString::fromStdString(tmp);
			outputLabel4->setText(out4);
			this->server->rnrQueue.pop();
		}
		mtx2.unlock();

		FD_ZERO(&server->readfds);		// 文件描述符清空
		FD_SET(server->listenSocket, &server->readfds);

		const timeval zero = { 0, 0 };

		server->rwCnt = select(0, &server->readfds, nullptr, nullptr, &zero);
		if (server->rwCnt == SOCKET_ERROR) {
			cout << "select() failed with error: " << WSAGetLastError() << endl;
		}

		/* 接收Client的链接 */
		SOCKET client = server->acceptConnections(out3);
		outputLabel3->setText(out3);

		/* 从客户端收到一个raw的信息,就创建一个子线程 */
		if (client != INVALID_SOCKET) {
			server->threadList->push_back(std::thread(&Server::receiveMessageFromClient, server, client, server->DEFAULT_BUFLEN));
		}

		/* 如果收到了endSignal 就退出线程 */
		if (this->server->endSignal)
			break;
	}
	/* 线程join */
	for (auto itor = server->threadList->begin(); itor != server->threadList->end(); itor++) {
		(*itor).join();
	}
	mtx2.lock();
	/* 把消息输出到屏幕上 */
	while (!this->server->messageQueue.empty()) {
		string tmp = this->server->messageQueue.front();
		out3 += QString::fromStdString(tmp);
		outputLabel3->setText(out3);
		this->server->messageQueue.pop();
	}
	while (!this->server->rnrQueue.empty()) {
		string tmp = this->server->rnrQueue.front();
		out4 += QString::fromStdString(tmp);
		outputLabel4->setText(out4);
		this->server->rnrQueue.pop();
	}
	mtx2.unlock();
	closesocket(server->listenSocket);		// 退出线程前主动关闭连接
	return SUCCEED;
}