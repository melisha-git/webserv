#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "StatusStruct.hpp"

class Response {
private:
	std::string startLine_;
	std::map<std::string, std::string> headers_;
	std::vector<std::string> body_;
public:
	Response(const std::string &request) {
		this->setBody("./index.html");
		this->setHeader("Server", "bebrochka");
		this->setHeader("Content-Length", std::to_string(this->getBodySize()));
		this->setHeader("Content-Type", "text/html");
		this->setHeader("Connection", "close");
		this->setStartLine(200);
	}

	Response(const Response &) = delete;

	Response &operator=(const Response &) = delete;

	void setStartLine(const int &status, const std::string &version = "HTTP/1.1") {
		startLine_ = version + " " + Status::getStatus(status);
	}

	void setHeader(const std::string &header, const std::string &params) {
		headers_[header] = params;
	}

	void setBody(std::string fileName) {
		std::ifstream file(fileName);
		this->setBody(file);
		file.close();
	}

	void setBody(std::ifstream &file) {
		if (!file.is_open())
			throw ("File not open\n");
		std::string line;
		while (std::getline(file, line))
			this->body_.push_back(line);
	}

	size_t getBodySize() const {
		size_t size = 0;
		for (std::string s : this->body_)
			size += s.size();
		return size;
	}


	operator std::string() {
		std::string response = startLine_ + "\n";
		for (std::pair<std::string, std::string> p : this->headers_)
			response += p.first + ": " + p.second + "\n"; 
		response += "\n";
		for (std::string line : this->body_)
			response += line + "\n";
		return response;
	}

	~Response() {}
};