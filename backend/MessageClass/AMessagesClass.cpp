#include "AMessagesClass.hpp"

std::map<std::string, std::string> AMessages::startLine() const {
	return this->startLine_;
}

std::map<std::string, std::string> AMessages::header() const {
	return this->headers_;
}

std::vector<std::string> AMessages::bodyes() const {
	return this->bodyes_;
}