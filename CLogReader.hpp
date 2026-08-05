#pragma once

#include "MMFile.hpp"
#include "Utils.hpp"

class CLogReader {
public:
	CLogReader();
	~CLogReader();

	bool Open(const char* filename);
	void Close();

	bool SetFilter(const char* filter);
	bool GetNextLine(const char*& str, size_t& len);

private:
	MMFile mmfile_;
	char filter_[MAX_FILTER_LENGTH + 1];
	size_t filterLength_{ 0 };
	Storage storage_;
};
