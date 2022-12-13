#ifndef MESSAGES_CLASS_HPP
#define MESSAGES_CLASS_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class Messages {
protected:
	std::string message_;
public:
	Messages(const std::string &);

	void message(const std::vector<std::string> &headers, const std::vector<std::string> &bodyes);

	std::string response();
};

#endif