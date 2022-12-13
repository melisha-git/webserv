#include "MessagesClass.hpp"
#include <regex>

// ! Прочие функции
// TODO убрать в другой файл
std::vector<std::string> split(const std::string &s, std::string dec) {
	std::regex reg(dec);
	std::sregex_token_iterator iter(s.begin(), s.end(), reg, -1);
	std::sregex_token_iterator end;
	return {iter, end};
}

template <class U, class T>
void print(std::map<U, T> mp) {
	for (std::pair<U, T> p : mp)
		std::cout << p.first << " " << p.second << std::endl;
}

template <class U>
void print(std::vector<U> vc) {
	int i = 0;
	for (U c : vc)
		std::cout << ++i << c << std::endl;
}

// ! Конец прочих функции
// TODO убрать в другой файл

Messages::Messages(const std::string &request) {
	//std::cout << request << std::endl;
	std::vector<std::string> vRequest = split(request, "\n");
	std::vector<std::string> utils = split(vRequest[0], " ");
	std::vector<std::string>::iterator it = utils.begin();
	this->setStartLine(it, utils.end());
	it = vRequest.begin() + 1;
	this->setHeader(it, vRequest.end());
	this->setBodyes(it, vRequest.end());
	print(this->startLine_);
}

void Messages::setStartLine(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) {
	if (begin + 2 >= end)
		return;
	this->startLine_["methood"] = *(begin++);
	while (begin != end && (*begin).find("HTTP") == -1) {
		this->startLine_["URL"] = *(begin++);
		if (this->startLine_["URL"] == "/")
			this->startLine_["URL"] = "/index.html";
		this->requestFile_.open("./frontend" + this->startLine_["URL"]);
		if (this->requestFile_.is_open())
			break;
	}
	if (!this->requestFile_.is_open())
		this->startLine_["status"] = "404 Not Found";
	else {
		this->startLine_["status"] = "200 OK";
	}
	this->startLine_["version"] = *(begin);
}

void Messages::setHeader(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) {
	for (;begin < end; ++begin) {
		if ((*begin).find(':') == -1)
			return ;
		std::vector<std::string> header = split(*begin, ":");
		this->headers_[header[0]] = header[1];
	}
	if (this->startLine_["status"] == "200 OK") {
		this->headers_["Server"] = "localhost";
		this->headers_["Connection"] = this->startLine_["close"];
	}
}

void Messages::setBodyes(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) {
	for (; this->startLine_["methood"] == "POST" && begin < end; ++begin) {
		this->bodyes_.push_back(*begin);
	}
	std::string line;
	size_t size = 0;
	while (this->startLine_["methood"] == "GET" && std::getline(this->requestFile_, line)) {
		size += line.size();
		this->bodyes_.push_back(line);
	}
	this->headers_["Content-Lenght"] = std::to_string(size);
	this->requestFile_.close();
}

std::string Messages::getResponse() {
	std::string response;
	response = this->startLine_["version"] + " " + this->startLine_["status"] + "\n";
	for (auto header : this->headers_) {
		response = response + (header.first + ": " + header.second + "\n");
	}
	response += "\n\n";
	for (auto bodyLine : this->bodyes_) {
		response = response + (bodyLine + "\n");
	}
	response += "\n\n";
	return response;
}

std::map<std::string, std::string> Messages::startLine() const {
	return this->startLine_;
}

std::map<std::string, std::string> Messages::header() const {
	return this->headers_;
}

std::vector<std::string> Messages::bodyes() const {
	return this->bodyes_;
}
