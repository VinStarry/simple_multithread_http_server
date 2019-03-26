#include "httpRequest.h"

/* HTTP request area begins */

/* constructor */
httpRequest::httpRequest() {
	this->rHeader = new map<string, string>;	// �洢����ͷ�е���Ϣ
}

/* destructor */
httpRequest::~httpRequest() {
	if (this->rHeader != nullptr) {	// �ͷ�����ͷ�е���Ϣ
		this->rHeader->clear();
		delete this->rHeader;
		this->rHeader = nullptr;
	}
}

/* ����HTTP������ */
bool httpRequest::phraseMessage(string requestMessage, SOCKET socket) {
	size_t currentPos = 0, nextPos = 0;
	/* ͨ��space�ҵ����󷽷� */
	nextPos = requestMessage.find(' ', currentPos);
	this->method = requestMessage.substr(currentPos, nextPos - currentPos);
	currentPos = nextPos + 1;
	/* ͨ��space�ҵ�����URL */
	nextPos = requestMessage.find(' ', currentPos);
	this->url = requestMessage.substr(currentPos, nextPos - currentPos);
	currentPos = nextPos + 1;
	/* ͨ��\r\n�ҵ�HTTP�汾 */
	nextPos = requestMessage.find("\r\n", currentPos);
	this->version = requestMessage.substr(currentPos, nextPos - currentPos);
	currentPos = nextPos + 2;
	/* ��������ͷ���� */
	this->rHeader = new map<string, string>;
	bool end = false;
	size_t emptyLine = requestMessage.find_last_of("\r\n");
	/* �����ǿ���Ҳû�н���ʱ */
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
		/* POST��data�� */
		this->data = requestMessage.substr(emptyLine + 2);
	}
	else {
		this->data = "";
	}
	this->socket = socket;
	return true;
}

/* ���HTTP�����ĵ���Ϣ�� */
void httpRequest::outputParams() {
	cout << "Method: " << this->method << endl;
	cout << "URL: " << this->url << endl;
	cout << "Version: " << this->version << endl;
	if (this->method == string("GET"))
		cout << "No data" << endl;
	else
		cout << "Data: " << this->data << endl;
}

/* �������������SOCKET��Ϣ */
SOCKET httpRequest::getsocket() {
	return this->socket;
}

/* �������� ���URL������ */
string httpRequest::getUrlName() {
	return this->url.substr(1);
}


/* HTTP Request area ends */
