#include "ExceptionClass.hpp"

Exceptions::Exceptions(const std::string &message) : message_(message) {}

const char * Exceptions::what() const throw() {
	return this->message_.c_str();
}