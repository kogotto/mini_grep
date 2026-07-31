#pragma once

#include <cstdio>

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