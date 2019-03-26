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

/* HTTP的请求，接收一个请求后可以通过phraseMessage得到请求每个条目 */
class httpRequest {
public:
	SOCKET socket;			// 请求对应的socket
	string method;			// 请求的方法 Get Post等等
	string url;				// 请求的URL
	string version;			// HTTP版本
	map<string, string> *rHeader;	// 请求头部分
	string data;			// POST的数据段

	string getUrlName();	// 获得URL请求的内容
	SOCKET getsocket();		// 获得客户端Socket
	httpRequest();			// constructor
	~httpRequest();			// destructor
	bool phraseMessage(string requestMessage, SOCKET socket);	// 解析请求
	void outputParams(void);	// 输出一些参数 用于debug
};