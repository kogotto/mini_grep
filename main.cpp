#include <iostream>

#include "CLogReader.hpp"

constexpr int BUF_SIZE = 1024;

bool filterFile(const char* filename, const char* filter) {
	CLogReader reader;
	if (!reader.Open(filename)) {
		std::cerr << "Can't open file \"" << filename << "\"\n";
		return false;
	}

	if (!reader.SetFilter(filter)) {
		std::cerr << "Can't set filter \"" << filter << "\"\n";
		return false;
	}

	std::cout << ":::: filter \"" << filter << "\"\n";
	char buf[BUF_SIZE];
	while (reader.GetNextLine(buf, BUF_SIZE)) {
		std::cout << buf << "\n";
	}
	return true;
}

#if 0
int main()
{
	CLogReader reader;
	const char* const filename = "input";
	if (!reader.Open(filename)) {
		std::cerr << "Can't open file \"" << filename << "\"\n";
		return 1;
	}

	const char* const filter = "*";
	if (!reader.SetFilter(filter)) {
		std::cerr << "Can't set filter \"" << filter << "\"\n";
		return false;
	}

	char buf[BUF_SIZE];
	while (reader.GetNextLine(buf, BUF_SIZE)) {
		std::cout << buf;
	}
	return 0;
}
#else
int main() {
	filterFile("input", "*");
	filterFile("input", "qwer");
	filterFile("input", "q*");
	filterFile("input", "*r");
	filterFile("input", "q*r");
	filterFile("input", "qw*r");
	filterFile("input", "q*er");
	filterFile("input", "*r*");
}
#endif
