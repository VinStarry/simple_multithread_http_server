#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include "ui_QtGuiApplication1.h"
#include <QLabel>
#include <QLineEdit>
#include <QButtonGroup>
#include "Config.h"
#include "myServer.h"

class QtGuiApplication1 : public QMainWindow
{
	Q_OBJECT 

public:
	QtGuiApplication1(QWidget *parent = Q_NULLPTR);
	QLineEdit *ip1;QLineEdit *ip2;QLineEdit *ip3;QLineEdit *ip4;		// �ı����齨 ������ʾ4��IP
	QLineEdit *port1;													// �˿ں�
	QLineEdit *homepage;												// ��Ŀ¼����ҳ
	QLabel *outputLabel, *outputLabel2, *outputLabel3, *outputLabel4;	// �����Ϣ��Label
	QPushButton *OKButton, *Start, *Close;								// 3����ť OK Start��Close
	QScrollArea *scrollArea;											// ������
	QWidget *centralWidget;												// ����
	vector<std::thread> *main;											// ���������߳�

public:
	Server *server;			// ��һ����������
	bool configOK;			// �Ƿ����óɹ�

public slots:
	void OKButtonClicked();	// �ۺ���,��OK�������º�����
	void startServer();		// �ۺ���,��START�������º�����
	void closeServer();		// �ۺ���,��CLOSE�������º�����

private:
	Ui::QtGuiApplication1Class ui;	// UI�齨
	bool status;					// ϵͳ״̬
	bool alreadyStart;				// �Ƿ��Ѿ���ʼ��
	bool endsignal;					// �������ź�
	QString info;					// һЩ��Ϣ
	QString start_s;				// ��ʼ����ʾ
	virtual int loopBegin();		// ��ʼѭ��
};
