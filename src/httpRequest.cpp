#include "httpRequest.h"

/* HTTP request area begins */

/* constructor */
httpRequest::httpRequest() {
	this->rHeader = new map<string, string>;	// 存储请求头中的信息
}

/* destructor */
httpRequest::~httpRequest() {
	if (this->rHeader != nullptr) {	// 释放请求头中的信息
		this->rHeader->clear();
		delete this->rHeader;
		this->rHeader = nullptr;
	}
}

/* 解析HTTP请求报文 */
bool httpRequest::phraseMessage(string requestMessage, SOCKET socket) {
	size_t currentPos = 0, nextPos = 0;
	/* 通过space找到请求方法 */
	nextPos = requestMessage.find(' ', currentPos);
	this->method = requestMessage.substr(currentPos, nextPos - currentPos);
	currentPos = nextPos + 1;
	/* 通过space找到请求URL */
	nextPos = requestMessage.find(' ', currentPos);
	this->url = requestMessage.substr(currentPos, nextPos - currentPos);
	currentPos = nextPos + 1;
	/* 通过\r\n找到HTTP版本 */
	nextPos = requestMessage.find("\r\n", currentPos);
	this->version = requestMessage.substr(currentPos, nextPos - currentPos);
	currentPos = nextPos + 2;
	/* 解析请求头内容 */
	this->rHeader = new map<string, string>;
	bool end = false;
	size_t emptyLine = requestMessage.find_last_of("\r\n");
	/* 当不是空行也没有结束时 */
	while (nextPos + 1 != emptyLine) {
		nextPos = requestMessage.find("\r\n", currentPos);
		string tmp = requestMessage.substr(currentPos, nextPos - currentPos);
		currentPos = nextPos + 2;
		size_t cpos = 0, npos = 0;
		npos = tmp.find_first_of(":");
		if (npos != std::string::npos) {
			string key = tmp.substr(cpos, npos);
			string value = tmp.substr(npos + 2);
			this->rHeader->insert(std::pair<string, string>(key, value));
		}
	}
	if (this->method == "POST") {
		/* POST的data段 */
		this->data = requestMessage.substr(emptyLine + 2);
	}
	else {
		this->data = "";
	}
	this->socket = socket;
	return true;
}

/* 输出HTTP请求报文的信息项 */
void httpRequest::outputParams() {
	cout << "Method: " << this->method << endl;
	cout << "URL: " << this->url << endl;
	cout << "Version: " << this->version << endl;
	if (this->method == string("GET"))
		cout << "No data" << endl;
	else
		cout << "Data: " << this->data << endl;
}

/* 辅助函数，获得SOCKET信息 */
SOCKET httpRequest::getsocket() {
	return this->socket;
}

/* 辅助函数 获得URL的名字 */
string httpRequest::getUrlName() {
	return this->url.substr(1);
}


/* HTTP Request area ends */
