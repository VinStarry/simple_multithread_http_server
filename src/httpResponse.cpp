#include "httpResponse.h"

/* HTTP Response area ends */

/* constructor */
httpResponse::httpResponse() {
	/* 生成各种HTTP状态码和状态码描述文字的映射 */
	this->status = new map<string, string>;
	this->status->insert(std::pair<string, string>("200", "OK"));
	this->status->insert(std::pair<string, string>("201", "Created"));
	this->status->insert(std::pair<string, string>("301", "Moved Permanently"));
	this->status->insert(std::pair<string, string>("302", "Found"));
	this->status->insert(std::pair<string, string>("304", "Modified"));
	this->status->insert(std::pair<string, string>("403", "Forbidden"));
	this->status->insert(std::pair<string, string>("404", "Not Found"));
	this->status->insert(std::pair<string, string>("405", "Method Not Allowed"));
	this->status->insert(std::pair<string, string>("500", "Internal Server Error"));
	this->status->insert(std::pair<string, string>("502", "Bad Gateway"));
	this->status->insert(std::pair<string, string>("503", "Service Unavailable"));
	this->status->insert(std::pair<string, string>("505", "HTTP Version Not Supported"));
	/* HTTP 版本设为1.1 */
	this->version = "HTTP/1.1";
	/* HTTP相应文件后缀名和mime type的映射 */
	this->httpCM[".avi"] = "video/avi";
	this->httpCM[".bmp"] = "application/x-bmp";
	this->httpCM[".css"] = "text/css";
	this->httpCM[".exe"] = "application/x-msdownload";
	this->httpCM[".gif"] = "image/gif";
	this->httpCM[".html"] = "text/html";
	this->httpCM[".htm"] = "text/html";
	this->httpCM[".htt"] = "text/webviewhtml";
	this->httpCM[".htx"] = "text/html";
	this->httpCM[".ico"] = "image/x-icon";
	this->httpCM[".java"] = "java/*";
	this->httpCM[".jpg"] = "image/jpeg";
	this->httpCM[".jpeg"] = "image/jpeg";
	this->httpCM[".latex"] = "application/x-latex";
	this->httpCM[".mml"] = "text/xml";
	this->httpCM[".mp1"] = "audio/mp1";
	this->httpCM[".mp2"] = "audio/mp2";
	this->httpCM[".mp2v"] = "video/mpeg";
	this->httpCM[".mp3"] = "audio/mp3";
	this->httpCM[".mp4"] = "video/mpeg4";
	this->httpCM[".pdf"] = "application/pdf";
	this->httpCM[".png"] = "image/png";
	this->httpCM[".rtf"] = "application/x-rtf";
	this->httpCM[".tif"] = "image/tiff";
	this->httpCM[".txt"] = "text/plain";
	this->httpCM[".xhtml"] = "text/html";
	this->httpCM[".xml"] = "text/xml";
}

/* destructor */
httpResponse::~httpResponse() {
	/* 把映射全部释放 */
	if (this->status != nullptr) {
		this->status->clear();
		delete this->status;
		this->status = nullptr;
	}
}

/* 生成HTTP相应信息 */
string httpResponse::generateMessage(httpRequest* hrqst, string defaultPage, string &view) {
	this->clientSocket = hrqst->getsocket();
	bool validSuffix = true;
	bool noneSuffix = false;
	bool defaultRequest = false;

	/* 首先获得请求URL的后缀信息 */
	string requstURL = hrqst->getUrlName();
	/* 如果请求URL为空,返回默认的主页 */
	if (requstURL == "") {
		requstURL = defaultPage + "\\wiki.htm";
		defaultRequest = true;
	}

	fstream fin;
	string fileSuffix = determineSuffix(requstURL, noneSuffix);
	string data;

	if (defaultRequest == true) {
		// 默认主页显示
		fin.open(defaultPage + "\\wiki.htm", ios::in | ios::binary);
		this->statusCode = "200";
		auto filetype = this->httpCM.find(fileSuffix);
		auto statusDp = this->status->find(string("200"));
		this->statusDescription = statusDp->second;
		this->contentType = filetype->second;
	}
	else {
		// 打开文件
		if (noneSuffix == true) {
			/* 找不到文件或没有后缀，404 NOT FOUND */
			this->statusCode = "404";
			auto statusDp = this->status->find(string("404"));
			this->statusDescription = statusDp->second;
		}
		else {
			/* 在mime-type映射中查找该后缀 */
			auto filetype = this->httpCM.find(fileSuffix);
			if (filetype == httpCM.end()) {
				// 找不到,不合法后缀
				validSuffix = false;
			}
			else {
				// 找到了
				validSuffix = true;
			}
			if (validSuffix) {
				// 打开文件成功 200 OK
				fin.open(defaultPage +"\\" + requstURL, ios::in | ios::binary);
				this->statusCode = "200";
				auto statusDp = this->status->find(string("200"));
				this->statusDescription = statusDp->second;
				this->contentType = filetype->second;
			}
			else {
				// 找不到文件 404 NOT FOUND
				this->statusCode = "404";
				auto statusDp = this->status->find(string("404"));
				this->statusDescription = statusDp->second;
				this->contentType = "image/jpeg";
				fin.open(defaultPage + "\\" + string("pagenotfound.jpg"), ios::in | ios::binary);
			}
		}
	}

	if (!fin.is_open()) {
		// 找不到文件发送pagenotfound.jpg并标识404
		this->statusCode = "404";
		auto statusDp = this->status->find(string("404"));
		this->statusDescription = statusDp->second;
		std::cout << "\n\nCouldn't find " << requstURL << " for reading" << std::endl;
		fin.close();
		fin.open(defaultPage + "\\" + string("pagenotfound.jpg"), ios::in | ios::binary);
		/* couldn't open file because suffix is invalid or file doesn't exist */
		auto itorjpg = this->httpCM.find(".jpg");
		this->contentType = itorjpg->second;
	}

	/* 获得文件的长度 */
	fin.seekg(0, ios::end);
	size_t totalLength = fin.tellg();

	/* 用buffer存储整个文件，二进制存储 */
	fin.seekg(0, ios::beg);
	char* inBuffer = new char[totalLength];
	fin.read(inBuffer, totalLength);
	fin.close();
	data = string(inBuffer, totalLength);

	delete inBuffer;
	this->contentLength = data.size();
	string msg = this->version + " " + this->statusCode + " " + this->statusDescription + "\r\n";
	/* 填充Content-Type:和Content-Length:信息 */
	msg = msg + "Content-Type: " + this->contentType + "\r\nContent-Length: " + to_string(data.size()) + "\r\n\r\n";
	view = "\n\n\n  Response Params: \n\n" + msg;
	msg += data;
	
	return msg;
}

/* 输出该响应报文的一些参数 */
void httpResponse::outputParams() {
	cout << this->version << " " << this->statusCode << " " << this->statusDescription << endl;
	cout << "Content-Length: " << this->contentLength << endl;
	cout << "Content-Type: " << this->contentType << endl;
}

/* 获得后缀名 */
string httpResponse::determineSuffix(string filename, bool &noneSuffix) {
	auto dosPos = filename.find_last_of('.');
	if (dosPos == std::string::npos) {
		noneSuffix = true;
		return "";
	}
	string rslt = filename.substr(dosPos);
	cout << "suffix: " << rslt << endl;
	return filename.substr(dosPos);
}

/* HTTP Response area ends */