#include "CLogReader.hpp"

#include <vector>

namespace {

using Row = std::vector<bool>;
using Matrix = std::vector<Row>;

Row emptyRow(size_t size) {
	return Row(size, false);
}

Matrix emptyMatrix(size_t rows, size_t cols) {
	Matrix result;
	result.reserve(rows);
	for (int i = 0; i < rows; ++i) {
		result.emplace_back(emptyRow(cols));
	}
	return result;
}

bool match(const char* str, const char* filter) {
	const auto strSize = strlen(str);
	const auto filterSize = strlen(filter);
	auto matrix = emptyMatrix(strSize + 1, filterSize + 1);

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

bool readLine(char* buf, int bufSize, FILE* file) {
	if (feof(file)) {
		return false;
	}

	int nextPlace = 0;
	for (; nextPlace < bufSize - 1;) {
		const int c = fgetc(file);
		if (c == EOF || c == '\n') {
			break;
		}
		buf[nextPlace++] = static_cast<char>(c);
	}
	buf[nextPlace] = '\0';
	return true;
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
	while (readLine(buf, bufSize, file_)) {
		if (match(buf, filter_)) {
			return true;
		}
	}
	return false;
}