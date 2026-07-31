#include <iostream>

#include "CLogReader.hpp"

constexpr int BUF_SIZE = 1024;

int main()
{
	CLogReader reader;
	const char* const filename = "input";
	if (!reader.Open(filename)) {
		std::cerr << "Can't open file \"" << filename << "\"\n";
		return 1;
	}

	char buf[BUF_SIZE];
	while (reader.GetNextLine(buf, BUF_SIZE)) {
		std::cout << buf;
	}
	return 0;
}
