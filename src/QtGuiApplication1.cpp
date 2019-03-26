#include "QtGuiApplication1.h"

std::mutex mtx2;

/*constructor
*	���ڹ���һЩQT���齨
*/
QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);	// ����ui
	this->outputLabel = this->findChild<QLabel *>("outputLabel");	// ��һ������� config��Ϣ
	this->outputLabel2 = this->findChild<QLabel *>("outputLabel2");	// �ڶ�������� ���һЩ������Ϣ
	this->outputLabel3 = this->findChild<QLabel *>("outputLabel3");	// �����������	http����ʵʱ��Ϣ
	this->outputLabel4 = this->findChild<QLabel *>("outputLabel4");	// ���ĸ������	http�ظ�ʵʱ��Ϣ
	this->OKButton = this->findChild<QPushButton *>("OKbutton");	// OK��ť
	this->Start = this->findChild<QPushButton *>("Start");			// Start��ť
	this->Close = this->findChild<QPushButton *>("Close");			// Close��ť
	this->ip1 = this->findChild<QLineEdit *>("ip1");				// һ��ΪIP��ַ���Ķ� ip1.ip2.ip3.ip4
	this->ip2 = this->findChild<QLineEdit *>("ip2");
	this->ip3 = this->findChild<QLineEdit *>("ip3");
	this->ip4 = this->findChild<QLineEdit *>("ip4");
	this->port1 = this->findChild<QLineEdit *>("port1");			// �˿�
	this->homepage = this->findChild<QLineEdit *>("homepage");		// ��Ŀ¼��ҳ
	this->scrollArea = this->findChild<QScrollArea *>("scrollArea");	// ��Ͳ��
	this->centralWidget = this->findChild<QScrollArea *>("centralWidget");	// ����

	QObject::connect(OKButton, SIGNAL(clicked()), this, SLOT(OKButtonClicked()));	// 3���ۺ�����ӦOK��Start��Close���º������¼�
	QObject::connect(Start, SIGNAL(clicked()), this, SLOT(startServer()));
	QObject::connect(Close, SIGNAL(clicked()), this, SLOT(closeServer()));

	//scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	//scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	this->status = true;		// ��ʼ��һЩ״̬�ֶ�
	this->configOK = false;
	this->endsignal = false;
	this->alreadyStart = false;

	main = new vector<std::thread>;	// �߳���

}

/*
*	�ۺ��� ��OK��ť�����º���������
*	Config���OK������Ѿ�Config���ˣ���ʾAlready Config
*/
void QtGuiApplication1::OKButtonClicked() {
	// config file
	if (this->configOK == true) {
		/* Config���OK������Ѿ�Config���ˣ���ʾAlready Config */
		info += "Already configed\n";
		return;
	}
	/* ���� ���óɹ� */
	QString addr = ip1->text() + "." + ip2->text() + "." + ip3->text() + "." + ip4->text();
	tempConfig tmp;
	/* ���һЩ��Ϣ */
	this->info = "Config OK!\n";
	info += tmp.readConfigFile(addr, port1->text(), homepage->text());
	this->server = new Server(tmp);
	/* ��ʼ��winsock */
	if (this->server->initWinsock() != SUCCEED) {
		info += "WSAStartup failed with error!\n";
		this->status = false;
		return;
	}
	else {
		info += "The Winsock 2.2 dll was found okay!\n";
	}
	int err = 0;	// �����ֶ�
	int rslt = this->server->createSocket(err); // ����socket
	if (rslt == 1) {
		info += "Getaddrinfo failed with: ";
		info += err;
		info += "\n";
		this->status = false;
		return;
	}
	else if (rslt == 2) {
		info += "Error at create socket for server with: "; // ����socket����
		info += err;
		info += "\n";
		this->status = false;
		return;
	}
	else if (rslt == 3) {	// bindһ���˿�ʧ��
		info += "Bind failed with error: ";
		info += err;
		info += "\n";
		this->status = false;
		return;
	}
	else {	// ���ɹ�
		info += "Listen socket is set and bind ok for server!";
	}
	this->configOK = true;	// ���óɹ�
	this->outputLabel->setText(info);	// �����Ϣ
}

/*
*	���������֮����԰�Start��Server
*   ��Server���������Ҫ��ɼ����Ȳ���
*/
void QtGuiApplication1::startServer() {
	if (!this->configOK) {
		/* ��û�н������� */
		start_s = "Config first!\n";
		outputLabel2->setText(start_s);
		return;
	}
	else if (!this->status) {
		/* ����ʧ�� */
		start_s = "Config first!\n";
		outputLabel2->setText(start_s);
		return;
	}
	else if (this->alreadyStart) {
		/* ��ǰ�������Ѿ����ڴ򿪵�״̬ */
		start_s = "Already Start!\n";
		outputLabel2->setText(start_s);
		return;
	}
	else {
		/* ���������� */
		start_s = "Starting Server...\n";
		int err = 0;
		int rslt = this->server->listenOnSocket(err); // ��֮ǰ���ɵĶ˿ڼ���
		if (rslt == 1) {
			/* ����ʧ�� */
			info += "Listen failed with error: ";
			info += err;
			info += "\n";
			this->status = false;
			outputLabel2->setText(start_s);
			return;
		}
		else {
			start_s += "Listen sockets set up succeed!\n"; // �����ɹ�
		}
		u_long blockMode = this->server->BLOCK_MODE;//none-block mode�����÷�����
		int rtn;

		if ((rtn = ioctlsocket(this->server->listenSocket, FIONBIO, &blockMode) == SOCKET_ERROR)) {
			// ���÷�����ģʽʧ��
			start_s += "ioctlsocket() failed with error: ";
			start_s += WSAGetLastError();
			start_s += "\n";
			outputLabel2->setText(start_s);
			return;
		}
		start_s += "ioctlsocket() for server socket ok! Waiting for client connection and data...\n"; // �ɹ�
		outputLabel2->setText(start_s);
		alreadyStart = true;
		main->push_back(std::thread(&QtGuiApplication1::loopBegin, this));	// ���߳̿�ʼLoop
	}
}

/* 
*	�رշ�����
*	�����������ڿ���״̬��ʱ��رշ�����
*/
void QtGuiApplication1::closeServer() {
	if (alreadyStart == false && configOK == true) {
		/*���û�д򿪷��������Ըð�ť*/
		return;
	}
	QString close_s = "Server Closed\n";
	this->start_s += close_s;
	outputLabel2->setText(start_s);
	this->server->endSignal = true;	// �����̷߳���endSignal
	for (auto itor = this->main->begin(); itor != this->main->end(); itor++)
		itor->join();
	// ���������߳̽���
	alreadyStart = false;	// ������״̬�ر�
	this->configOK = false;	// ��Ҫ�������ò��ܴ�
	server->~Server();		// ����
	delete server;
	server = nullptr;
	main->clear();			// �߳�list����
}

/*
*	�������������߳̽��е�loopBegin()
*	ÿ���յ�һ������,�ͽ���һ���߳�
*	ÿһ�����̸߳����messageQueue��ȡ����Ϣ
*	���н��������������Ժ��͸��ͻ���
*	���������е�һЩ��¼��Ϣ�������out3��out4��
*	����յ�endSignal�Ժ�������߳�
*/
int QtGuiApplication1::loopBegin() {
	QString out3 = "";
	QString out4 = "";
	while (!this->server->endSignal) {

		mtx2.lock();	// ������
		while (!this->server->messageQueue.empty()) {
			/* ÿ�δ���Ϣ������ȡ��һ����Ϣ */
			string tmp = this->server->messageQueue.front();
			out3 += QString::fromStdString(tmp);
			outputLabel3->setText(out3);	// �����Ϣ
			this->server->messageQueue.pop();
		}
		while (!this->server->rnrQueue.empty()) {
			/* ��һЩ�ظ���Ϣ�������Ļ�� */
			string tmp = this->server->rnrQueue.front();
			out4 += QString::fromStdString(tmp);
			outputLabel4->setText(out4);
			this->server->rnrQueue.pop();
		}
		mtx2.unlock();

		FD_ZERO(&server->readfds);		// �ļ����������
		FD_SET(server->listenSocket, &server->readfds);

		const timeval zero = { 0, 0 };

		server->rwCnt = select(0, &server->readfds, nullptr, nullptr, &zero);
		if (server->rwCnt == SOCKET_ERROR) {
			cout << "select() failed with error: " << WSAGetLastError() << endl;
		}

		/* ����Client������ */
		SOCKET client = server->acceptConnections(out3);
		outputLabel3->setText(out3);

		/* �ӿͻ����յ�һ��raw����Ϣ,�ʹ���һ�����߳� */
		if (client != INVALID_SOCKET) {
			server->threadList->push_back(std::thread(&Server::receiveMessageFromClient, server, client, server->DEFAULT_BUFLEN));
		}

		/* ����յ���endSignal ���˳��߳� */
		if (this->server->endSignal)
			break;
	}
	/* �߳�join */
	for (auto itor = server->threadList->begin(); itor != server->threadList->end(); itor++) {
		(*itor).join();
	}
	mtx2.lock();
	/* ����Ϣ�������Ļ�� */
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
	closesocket(server->listenSocket);		// �˳��߳�ǰ�����ر�����
	return SUCCEED;
}