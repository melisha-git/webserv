#include "MessagesClass.hpp"
#include "../Utils/Utils.hpp"

Messages::Messages(const std::string &request) : message_(request) {
	std::vector<std::string> parts = utils::split(request, "\n\n");
	std::vector<std::string> headers = utils::split(parts[0], "\n");
	std::vector<std::string> bodyes;
	if (parts.size() == 2)
		bodyes = utils::split(parts[1], "\n");
	this->message(headers, bodyes);
}

void Messages::message(const std::vector<std::string> &headers, const std::vector<std::string> &bodyes) {
	std::vector<std::string> startLine = utils::split(headers[0], " ");
	
	std::string method = startLine[0];
	if (method != "GET")
		return;

	this->message_ = startLine[2];
	if (startLine[1] == "/")
		startLine[1] = "/index.html";
	std::ifstream file(startLine[1]);
	if (!file.is_open()) {
		this->message_ += "404 Not Found\n";
		this->message_ += "Connection: close\n";
		this->message_ += "Content-Length: \n\n";
		message_ += "<html><body><h1>404 NOT FOUND</h1></body></html>\n";
		return;
	}
	this->message_ += " 200 OK\n";
	std::pair<size_t, std::vector<std::string> > vcFile = utils::fileReader(file);
	file.close();
	if (std::find(headers.begin(), headers.end(), "Connection: close") != headers.end())
		this->message_ += "Connection: close\n";
	this->message_ += "Content-Length: " + std::to_string(vcFile.first) + "\n\n";
	for (std::string line : vcFile.second)
		message_ += line + "\n";
	message_ += "\n";
}

std::string Messages::response() {
	return this->message_;
}
