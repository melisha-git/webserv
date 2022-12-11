#include <string>
#include <vector>

#include <regex>

std::vector<std::string> split(const std::string &s, std::string dec) {
	std::regex reg(dec);
	std::sregex_token_iterator iter(s.begin(), s.end(), reg, -1);
	std::sregex_token_iterator end;
	return {iter, end};
}