#include "CLogReader.hpp"

#include <cstring>
#include <Windows.h>

namespace {

bool row1[MAX_FILTER_LENGTH + 1];
bool row2[MAX_FILTER_LENGTH + 1];

struct Matrix {
	Matrix(size_t colCount)
		: currentRow_{ row1 }
		, previousRow_{ row2 } {
		memset(previousRow_, 0, sizeof(row2));
	}

	bool* currentRow() {
		return currentRow_;
	}

	bool* previousRow() {
		return previousRow_;
	}

	void swapRows() {
		bool* tmp = currentRow_;
		currentRow_ = previousRow_;
		previousRow_ = tmp;
	}

private:
	bool* currentRow_;
	bool* previousRow_;
};

bool match(const char* str, size_t strSize, const char* filter, size_t filterSize) {
	Matrix matrix{ filterSize + 1};

	auto innerLoop = [&] (int i) {
		for (int j = 1; j <= filterSize; ++j) {
			switch (filter[j - 1]) {
			case '*':
				matrix.currentRow()[j] = matrix.previousRow()[j] || matrix.currentRow()[j - 1];
				break;
			case '?':
				matrix.currentRow()[j] = matrix.previousRow()[j - 1];
				break;
			default:
				if (i == 0) {
					matrix.currentRow()[j] = false;
				}
				else {
					matrix.currentRow()[j] = matrix.previousRow()[j - 1] && (filter[j - 1] == str[i - 1]);
				}
				break;
			}
		}
	};

	matrix.currentRow()[0] = true;
	innerLoop(0);
	matrix.swapRows();
	for (int i = 1; i <= strSize; ++i) {
		matrix.currentRow()[0] = false;
		innerLoop(i);
		matrix.swapRows();
	}

	return matrix.previousRow()[filterSize];
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
	const auto len = strlen(filter);
	if (len > MAX_FILTER_LENGTH) {
		return false;
	}

	filter_ = filter;
	filterLength_ = len;
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
