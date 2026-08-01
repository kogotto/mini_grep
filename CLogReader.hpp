#pragma once

#include <cstdio>

constexpr int MAX_STR_LENGTH = 1024;

class CLogReader {
public:
	CLogReader() = default;
	~CLogReader() = default;

	bool Open(const char* filename);
	void Close();

	bool SetFilter(const char* filter);
	bool GetNextLine(char* buf, const int bufSize);

private:
	std::FILE* file_{ nullptr };
	const char* filter_{ nullptr };
};