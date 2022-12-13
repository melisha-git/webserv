#ifndef MESSAGES_CLASS_HPP
#define MESSAGES_CLASS_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class Messages {
protected:
	std::ifstream requestFile_;
	std::map<std::string, std::string> startLine_;
	std::map<std::string, std::string> headers_;
	std::vector<std::string> bodyes_;
public:
	Messages(const std::string &);

	void setStartLine(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end);
	void setHeader(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end);
	void setBodyes(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end);

	std::string getResponse();
	std::map<std::string, std::string> startLine() const;
	std::map<std::string, std::string> header() const;
	std::vector<std::string> bodyes() const;
};

#endif