#include "CLogReader.hpp"

#include <cstring>
#include <Windows.h>

namespace {

struct Matrix {
	Matrix(detail::Storage& storage)
		: currentRow_{ storage.row1 }
		, previousRow_{ storage.row2 } {
		memset(previousRow_, 0, sizeof(storage.row2));
	}

	bool* CurrentRow() {
		return currentRow_;
	}

	bool* PreviousRow() {
		return previousRow_;
	}

	void SwapRows() {
		bool* tmp = currentRow_;
		currentRow_ = previousRow_;
		previousRow_ = tmp;
	}

private:
	bool* currentRow_;
	bool* previousRow_;
};

bool match(const char* str, size_t strSize, const char* filter, size_t filterSize, detail::Storage& storage) {
	Matrix matrix{ storage };

	auto innerLoop = [&] (int i) {
		for (int j = 1; j <= filterSize; ++j) {
			switch (filter[j - 1]) {
			case '*':
				matrix.CurrentRow()[j] = matrix.PreviousRow()[j] || matrix.CurrentRow()[j - 1];
				break;
			case '?':
				matrix.CurrentRow()[j] = matrix.PreviousRow()[j - 1];
				break;
			default:
				if (i == 0) {
					matrix.CurrentRow()[j] = false;
				}
				else {
					matrix.CurrentRow()[j] = matrix.PreviousRow()[j - 1] && (filter[j - 1] == str[i - 1]);
				}
				break;
			}
		}
	};

	matrix.CurrentRow()[0] = true;
	innerLoop(0);
	matrix.SwapRows();
	for (int i = 1; i <= strSize; ++i) {
		matrix.CurrentRow()[0] = false;
		innerLoop(i);
		matrix.SwapRows();
	}

	return matrix.PreviousRow()[filterSize];
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
		if (match(str, len, filter_, filterLength_, storage_)) {
			return true;
		}
	}
	return false;
}
