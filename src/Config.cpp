#include "Config.h"

/**
*	从文件中读取配置文件
*	配置完成之后输出
*/
QString tempConfig::readConfigFile(QString IP, QString PORT, QString HP) {
	this->defaultPort = PORT.toStdString();	// 读入指定端口
	this->serverAddress = IP.toStdString();	// 读入服务器IP
	this->homePage = HP.toStdString();		// 读入主页
	QString rtn = "IP address: \n" + QString::fromStdString(this->serverAddress) + "\n" + "Port Number: \n" 
		+ QString::fromStdString(this->defaultPort) + "\n"
		+ "Default Page: \n" + QString::fromStdString(this->homePage) + "\n";
	return rtn;	//返回信息可以输出在屏幕上
}