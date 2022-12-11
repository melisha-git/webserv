#ifndef REQUEST_CLASS_HPP
#define REQUEST_CLASS_HPP

#include <iostream>
#include "../AMessagesClass.hpp"
//#include "../../Utility/split.cpp"

class Request : public AMessages {
public:
	Request(const std::string &request);

	virtual void setStartLine(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end);
	virtual void setHeader(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end);
	virtual void setBodyes(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end);
};

#endif