#pragma once

#include <string>

class Exception : std::exception {
private:
	std::string exception_;
public:
	Exception(std::string exception) : exception_(exception) {}

	virtual const char *what() const throw() {
		return this->exception_.c_str();
	}

	Exception() = delete;
	Exception operator=(const Exception &) = delete;

	virtual ~Exception() throw() {

	}
};