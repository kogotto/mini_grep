#include "CLogReader.hpp"

#include <cstring>
#include <Windows.h>

namespace {

constexpr int MAX_STR_LENGTH = 1024;
constexpr int rawStorageSize = MAX_STR_LENGTH * MAX_STR_LENGTH;
bool rawStorage[rawStorageSize] = {};

struct Matrix {
	Matrix(size_t rowCount, size_t colCount)
		: rowCount{ rowCount }
		, colCount{ colCount } {
	}

	bool* operator[](size_t row) {
		return rawStorage + row * colCount;
	}
private:
	size_t rowCount;
	size_t colCount;
};

bool match(const char* str, size_t strSize, const char* filter, size_t filterSize) {
	Matrix matrix{ strSize, filterSize };

	matrix[0][0] = true;
	for (int i = 1; i <= strSize; ++i) {
		matrix[i][0] = false;
	}
	for (int i = 0; i <= strSize; ++i) {
		for (int j = 1; j <= filterSize; ++j) {
			switch (filter[j - 1]) {
			case '*':
				if (i == 0) {
					matrix[i][j] = matrix[i][j - 1];
				}
				else {
					matrix[i][j] = matrix[i - 1][j] || matrix[i][j - 1];
				}
				break;
			case '?':
				if (i == 0) {
					matrix[i][j] = false;
				}
				else {
					matrix[i][j] = matrix[i - 1][j - 1];
				}
				break;
			default:
				if (i == 0) {
					matrix[i][j] = false;
				}
				else {
					matrix[i][j] = matrix[i - 1][j - 1] && filter[j - 1] == str[i - 1];
				}
				break;
			}
		}
	}

	return matrix[strSize][filterSize];
}

} // namespace

bool CLogReader::Open(const char* filename) {
	if (mmfile_.Opened()) {
		mmfile_.Close();
	}
	return mmfile_.Open(filename);
}

void CLogReader::Close() {
	mmfile_.Close();
}

bool CLogReader::SetFilter(const char* filter) {
	filter_ = filter;
	filterLength_ = strlen(filter);

	return true;
}

bool CLogReader::GetNextLine(const char*& str, size_t& len) {
	while (mmfile_.GetNextLine(str, len)) {
		if (match(str, len, filter_, filterLength_)) {
			return true;
		}
	}
	return false;
}
