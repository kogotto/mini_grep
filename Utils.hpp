#pragma once

#include <string.h>

size_t CopyAndRemoveDublicates(const char* rawFilter, char* outFilter);

constexpr int MAX_FILTER_LENGTH = 1024;

struct Storage {
	bool row1[MAX_FILTER_LENGTH + 1];
	bool row2[MAX_FILTER_LENGTH + 1];
};

struct Matrix {
	Matrix(Storage& storage)
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

bool match(const char* str, size_t strSize, const char* filter, size_t filterSize, Storage& storage);