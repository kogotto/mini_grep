#include <stdio.h>

#include "CLogReader.hpp"

namespace {

void ProcessString(const char* str, size_t len) {
	for (int i = 0; i < len; ++i) {
		putchar(str[i]);
	}
	putchar('\n');
}

constexpr int NORMAL_EXIT = 0;
constexpr int WRONG_USAGE = 1;
constexpr int CAN_NOT_OPEN_FILE = 2;
constexpr int CAN_NOT_SET_FILTER = 3;

} // namespace

int main(int argc, const char* argv[])
{
	if (argc != 3) {
		printf("Usage: %s <filename> <filter> \n", argv[0]);
		return WRONG_USAGE;
	}

	CLogReader reader;
	if (!reader.Open(argv[1])) {
		printf("Can't open file \"%s\"\n", argv[1]);
		return CAN_NOT_OPEN_FILE;
	}

	if (!reader.SetFilter(argv[2])) {
		printf("Can't set filter \"%s\"\n", argv[2]);
		return CAN_NOT_SET_FILTER;
	}

	const char* str = nullptr;
	size_t len = 0;
	while (reader.GetNextLine(str, len)) {
		ProcessString(str, len);
	}
	return NORMAL_EXIT;
}
