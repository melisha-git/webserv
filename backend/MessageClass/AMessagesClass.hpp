#ifndef AMESSAGES_CLASS_HPP
#define AMESSAGES_CLASS_HPP

#include <string>
#include <vector>
#include <map>

class AMessages {
protected:
	std::map<std::string, std::string> startLine_;
	std::map<std::string, std::string> headers_;
	std::vector<std::string> bodyes_;
public:
	
	virtual void setStartLine(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) = 0;
	virtual void setHeader(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) = 0;
	virtual void setBodyes(std::vector<std::string>::iterator &begin, std::vector<std::string>::iterator end) = 0;

	std::map<std::string, std::string> startLine() const;
	std::map<std::string, std::string> header() const;
	std::vector<std::string> bodyes() const;
};

#endif