#pragma once

#include <cstdio>

#include "MMFile.hpp"

constexpr int MAX_FILTER_LENGTH = 1024;

class CLogReader {
public:
	CLogReader() = default;
	~CLogReader() = default;

	bool Open(const char* filename);
	void Close();

	bool SetFilter(const char* filter);
	bool GetNextLine(const char*& str, size_t& len);

private:
	MMFile mmfile_;
	const char* filter_{ nullptr };
	size_t filterLength_{ 0 };
};
