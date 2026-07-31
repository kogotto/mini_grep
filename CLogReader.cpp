#include "CLogReader.hpp"

#include <cctype>

namespace {

bool accepted(const char* str, const char* filter) {
	return std::isalpha(str[0]);
}

} // namespace

bool CLogReader::Open(const char* filename) {
	if (file_) {
		Close();
	}
	file_ = fopen(filename, "r");
	return file_ != nullptr;
}

void CLogReader::Close() {
	fclose(file_);
	file_ = nullptr;
}

bool CLogReader::SetFilter(const char* filter) {
	filter_ = filter;
	return true;
}

bool CLogReader::GetNextLine(char* buf, const int bufSize) {
	while (fgets(buf, bufSize, file_) != nullptr) {
		if (accepted(buf, filter_)) {
			return true;
		}
	}
	return false;
}