#pragma once
#include <winsock2.h>
#include <iostream>
#include <string>
#include <fstream>
#include <QString>

using std::string;

struct tempConfig {
	
	string serverAddress;	/* ��������IP��ַ */
	string defaultPort;		/* ������Ĭ�ϰ󶨵Ķ˿� */
	string homePage;		/* ������Ŀ¼����ҳ */

	QString readConfigFile(QString IP, QString PORT, QString HP);	// ���ļ��ж�ȡ�����ļ�
};