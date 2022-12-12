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
	std::vector<std::string> vRequest = split(request, "\n");
	std::vector<std::string> utils = split(vRequest[0], " ");
	std::vector<std::string>::iterator it = utils.begin();
	this->setStartLine(it, utils.end());
	it = vRequest.begin() + 1;
	this->setHeader(it, vRequest.end());
	this->setBodyes(it, vRequest.end());
}

void Messages::setStartLine(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) {
	if (begin + 2 >= end)
		return;
	this->startLine_["methood"] = *(begin++);
	this->startLine_["URL"] = *(begin++);
	this->startLine_["version"] = *(begin);
}

void Messages::setHeader(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) {
	for (;begin < end; ++begin) {
		if ((*begin).find(':') == -1)
			return ;
		std::vector<std::string> header = split(*begin, ":");
		this->headers_[header[0]] = header[1];
	}
}

void Messages::setBodyes(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) {
	for (; begin < end; ++begin) {
		this->bodyes_.push_back(*begin);
	}
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
