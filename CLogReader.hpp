#pragma once

#include <cstdio>

#include "MMFile.hpp"

class CLogReader {
public:
	CLogReader() = default;
	~CLogReader() = default;

	bool Open(const char* filename);
	void Close();

	bool SetFilter(const char* filter);
	bool GetNextLine(const char*& str, int& len);

private:
	MMFile mmfile_;
	const char* filter_{ nullptr };
	size_t filterLength_{ 0 };
};
