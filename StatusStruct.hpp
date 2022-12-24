#pragma once

#include <string>

struct Status {
private:
	std::string status_;
public:
	explicit Status(int status) : status_(std::to_string(status) + _getString(status)) {}

	Status() = delete;
	Status(const Status &) = delete;

	Status &operator=(const Status &) = delete;

	std::string getStatus() const {
		return this->status_;
	}

	~Status() {}
private:
	std::string _getString(int status) {
		std::string result;
		switch (status) {
		case 200:
			result = "OK";
			break;
		case 404:
			result = "NOT FOUND";
			break;
		}
		return result;
	}
};