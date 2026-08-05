#include "CLogReader.hpp"

CLogReader::CLogReader() {
	filter_[0] = '*';
	filter_[1] = '\0';
	filterLength_ = 1;
}

CLogReader::~CLogReader() {
	Close();
}

bool CLogReader::Open(const char* filename) {
	return mmfile_.Open(filename);
}

void CLogReader::Close() {
	mmfile_.Close();
}

bool CLogReader::SetFilter(const char* filter) {
	const auto len = strlen(filter);
	if (len > MAX_FILTER_LENGTH) {
		return false;
	}

	filterLength_ = CopyAndRemoveDublicates(filter, filter_);
	return true;
}

bool CLogReader::GetNextLine(const char*& str, size_t& len) {
	while (mmfile_.GetNextLine(str, len)) {
		if (match(str, len, filter_, filterLength_, storage_)) {
			return true;
		}
	}
	return false;
}
