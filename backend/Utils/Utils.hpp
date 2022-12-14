#ifndef UTILS_HPP
#define UTILS_HPP
#include <regex>
#include <fstream>
#include <vector>

namespace utils {
	std::vector<std::string> split(const std::string &s, std::string dec) {
		std::regex reg(dec);
		if (s.begin() == s.end())
			return {};
		std::sregex_token_iterator iter(s.begin(), s.end(), reg, -1);
		std::sregex_token_iterator end;
		return {iter, end};
	}

	std::pair<size_t, std::vector<std::string> > fileReader(std::ifstream &file) {
		std::vector<std::string> result;
		std::string line;
		size_t size = 0;
		while (std::getline(file, line)) {
			size += line.size();
			result.push_back(line);
		}
		return {size, result};
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
}

#endif