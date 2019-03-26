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
	QLineEdit *ip1;QLineEdit *ip2;QLineEdit *ip3;QLineEdit *ip4;		// 文本框组建 用于显示4个IP
	QLineEdit *port1;													// 端口号
	QLineEdit *homepage;												// 主目录与主页
	QLabel *outputLabel, *outputLabel2, *outputLabel3, *outputLabel4;	// 输出信息的Label
	QPushButton *OKButton, *Start, *Close;								// 3个按钮 OK Start和Close
	QScrollArea *scrollArea;											// 滚动条
	QWidget *centralWidget;												// 窗口
	vector<std::thread> *main;											// 主函数多线程

public:
	Server *server;			// 有一个服务器类
	bool configOK;			// 是否配置成功

public slots:
	void OKButtonClicked();	// 槽函数,当OK键被按下后启动
	void startServer();		// 槽函数,当START键被按下后启动
	void closeServer();		// 槽函数,当CLOSE键被按下后启动

private:
	Ui::QtGuiApplication1Class ui;	// UI组建
	bool status;					// 系统状态
	bool alreadyStart;				// 是否已经开始了
	bool endsignal;					// 结束的信号
	QString info;					// 一些信息
	QString start_s;				// 开始的提示
	virtual int loopBegin();		// 开始循环
};
