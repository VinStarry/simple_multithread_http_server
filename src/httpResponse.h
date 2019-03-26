#pragma once

#include "winsock2.h"
#include <iostream>
#include <string>
#include <map>
#include "httpRequest.h"
#include <fstream>

using std::fstream;
using std::ios;
using std::to_string;

/* http�ظ�,�յ�HTTP��������ɻظ������� */
class httpResponse {
public:
	size_t contentLength;		// ���ݳ���
	string contentType;			// ���ݵ�����
	string statusCode;			// HTTP״̬��
	string statusDescription;	// ״̬������
	string version;				// HTTP�汾

protected:
	string determineSuffix(string filename, bool &noneSuffix);		// �ж��ļ��ĺ�׺�� .txt/.pdf ...û�к�׺���᷵�ؿ�string

public:
	map<string, string> *status;	/* ״̬���״̬��������һ��ӳ�� */
	map<string, string> httpCM;	/* HTTP���ļ�����ת����mime type */
	SOCKET clientSocket;		/* �ͻ��˵�socket */
	httpResponse();				// constructor
	~httpResponse();			// destructor
	string generateMessage(httpRequest *hrqst, string defaultPage, string &view);	// ����HTTP��Ϣ
	void outputParams(void);	// ���һЩ���� ����debug
};