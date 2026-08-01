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

#if 1
int main(int argc, const char* argv[])
{
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <filename> <filter>\n";
		return 0;
	}

	CLogReader reader;
	if (!reader.Open(argv[1])) {
		std::cerr << "Can't open file \"" << argv[1] << "\"\n";
		return 1;
	}

	if (!reader.SetFilter(argv[2])) {
		std::cerr << "Can't set filter \"" << argv[2] << "\"\n";
		return false;
	}

	char buf[BUF_SIZE];
	while (reader.GetNextLine(buf, BUF_SIZE)) {
		std::cout << buf << "\n";
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
	filterFile("input", "?");
	filterFile("input", "??");
	filterFile("input", "???");
	filterFile("input", "?d");
	filterFile("input", "a?");
	filterFile("input", "?d?");
	filterFile("input", "order ? opened");
	filterFile("input", "order ?* closed");
	filterFile("input", "order ?*? *");
}
#endif
