#include "RequestClass.hpp"
#include <regex>

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

Request::Request(const std::string &request) {
	std::vector<std::string> vRequest = split(request, "\n");
	std::vector<std::string> utils = split(vRequest[0], " ");
	std::vector<std::string>::iterator it = utils.begin();
	this->setStartLine(it, utils.end());
	it = vRequest.begin() + 1;
	this->setHeader(it, vRequest.end());
	this->setBodyes(it, vRequest.end());
}

void Request::setStartLine(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) {
	if (begin + 2 >= end)
		return;
	this->startLine_["methood"] = *(begin++);
	this->startLine_["URL"] = *(begin++);
	this->startLine_["version"] = *(begin);
}

void Request::setHeader(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) {
	for (;begin < end; ++begin) {
		if ((*begin).find(':') == -1)
			return ;
		std::vector<std::string> header = split(*begin, ":");
		this->headers_[header[0]] = header[1];
	}
}

void Request::setBodyes(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) {
	for (; begin < end; ++begin) {
		this->bodyes_.push_back(*begin);
	}
}