#include "Utils.hpp"

size_t CopyAndRemoveDublicates(const char* rawFilter, char* outFilter) {
	*outFilter = *rawFilter;
	if (*outFilter == '\0') {
		return 1;
	}
	size_t outPos = 1;
	for (int inPos = 1; rawFilter[inPos] != '\0'; ++inPos) {
		if (rawFilter[inPos] == '*' && outFilter[outPos - 1] == '*') {
			continue;
		}
		outFilter[outPos] = rawFilter[inPos];
		++outPos;
	}
	outFilter[outPos] = '\0';
	return outPos;
}

bool match(const char* str, size_t strSize, const char* filter, size_t filterSize, Storage& storage) {
	Matrix matrix{ storage };

	auto innerLoop = [&](int i) {
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
		matrix.SwapRows();
		};

	matrix.CurrentRow()[0] = true;
	innerLoop(0);
	for (int i = 1; i <= strSize; ++i) {
		matrix.CurrentRow()[0] = false;
		innerLoop(i);
	}

	return matrix.PreviousRow()[filterSize];
}