#pragma once
#include <winsock2.h>
#include <iostream>
#include <string>
#include <fstream>
#include <QString>

using std::string;

struct tempConfig {
	
	string serverAddress;	/* 服务器的IP地址 */
	string defaultPort;		/* 服务器默认绑定的端口 */
	string homePage;		/* 设置主目录和主页 */

	QString readConfigFile(QString IP, QString PORT, QString HP);	// 从文件中读取配置文件
};