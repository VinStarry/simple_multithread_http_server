#pragma once

#include <string>
#include <map>
#include <http.h>
#include <iostream>
#include <winsock2.h>
#include <algorithm>

using std::string;
using std::map;
using std::cout;
using std::endl;

/* HTTP�����󣬽���һ����������ͨ��phraseMessage�õ�����ÿ����Ŀ */
class httpRequest {
public:
	SOCKET socket;			// �����Ӧ��socket
	string method;			// ����ķ��� Get Post�ȵ�
	string url;				// �����URL
	string version;			// HTTP�汾
	map<string, string> *rHeader;	// ����ͷ����
	string data;			// POST�����ݶ�

	string getUrlName();	// ���URL���������
	SOCKET getsocket();		// ��ÿͻ���Socket
	httpRequest();			// constructor
	~httpRequest();			// destructor
	bool phraseMessage(string requestMessage, SOCKET socket);	// ��������
	void outputParams(void);	// ���һЩ���� ����debug
};