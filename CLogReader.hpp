#pragma once

#include <cstdio>

#include "MMFile.hpp"

constexpr int MAX_FILTER_LENGTH = 1024;

namespace detail {

struct Storage {
	bool row1[MAX_FILTER_LENGTH + 1];
	bool row2[MAX_FILTER_LENGTH + 1];
};

} // namespace detail

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
	char filter_[MAX_FILTER_LENGTH + 1];
	size_t filterLength_{ 0 };
	detail::Storage storage_;
};
