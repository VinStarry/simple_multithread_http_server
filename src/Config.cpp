#include "Config.h"

/**
*	���ļ��ж�ȡ�����ļ�
*	�������֮�����
*/
QString tempConfig::readConfigFile(QString IP, QString PORT, QString HP) {
	this->defaultPort = PORT.toStdString();	// ����ָ���˿�
	this->serverAddress = IP.toStdString();	// ���������IP
	this->homePage = HP.toStdString();		// ������ҳ
	QString rtn = "IP address: \n" + QString::fromStdString(this->serverAddress) + "\n" + "Port Number: \n" 
		+ QString::fromStdString(this->defaultPort) + "\n"
		+ "Default Page: \n" + QString::fromStdString(this->homePage) + "\n";
	return rtn;	//������Ϣ�����������Ļ��
}