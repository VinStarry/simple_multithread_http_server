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

/* http回复,收到HTTP请求后生成回复并返回 */
class httpResponse {
public:
	size_t contentLength;		// 内容长度
	string contentType;			// 内容的类型
	string statusCode;			// HTTP状态码
	string statusDescription;	// 状态码描述
	string version;				// HTTP版本

protected:
	string determineSuffix(string filename, bool &noneSuffix);		// 判断文件的后缀名 .txt/.pdf ...没有后缀名会返回空string

public:
	map<string, string> *status;	/* 状态码和状态码描述的一个映射 */
	map<string, string> httpCM;	/* HTTP的文件类型转化成mime type */
	SOCKET clientSocket;		/* 客户端的socket */
	httpResponse();				// constructor
	~httpResponse();			// destructor
	string generateMessage(httpRequest *hrqst, string defaultPage, string &view);	// 生成HTTP信息
	void outputParams(void);	// 输出一些参数 用于debug
};