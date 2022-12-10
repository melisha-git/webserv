#ifndef EXCEPTION_CLASS_HPP
#define EXCEPTION_CLASS_HPP

#include <string>

class Exceptions : public std::exception {
private:
	std::string message_;
public:
	virtual const char * what() const throw();
	Exceptions(const std::string &);
};

#endif