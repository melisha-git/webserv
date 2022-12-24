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
	Response(const std::string &version = "HTTP/1.1") {
		startLine_ = version + Status(404).getStatus();
	}

	// INT STATUS
	void setStartLine(const Status &status, const std::string &version = "HTTP/1.1") {
		startLine_ = version + " " + status.getStatus();
	}

	void setHeader(const std::string &header, const std::string &params) {
		headers_[header] = params;
	}

	void setBody(std::string fileName) {
		std::ifstream file(fileName);
		this->setBody(file);
		file.close();
	}

	size_t getBodySize() const {
		size_t size = 0;
		for (std::string s : this->body_)
			size += s.size();
		return size;
	}

	void setBody(std::ifstream &file) {
		if (!file.is_open())
			throw ("File not open\n");
		std::string line;
		while (std::getline(file, line))
			this->body_.push_back(line);
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
};