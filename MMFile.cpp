#include "MMFile.hpp"

#include <iostream>

namespace {

constexpr DWORD PIECES = 32;

DWORD GetGranularity() {
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	return systemInfo.dwAllocationGranularity;
}

HANDLE MyCreateFile(const char* filename) {
	return CreateFile(
		filename,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY,
		NULL
	);
}

HANDLE MyCreateFileMapping(HANDLE file) {
	return CreateFileMapping(
		file,
		NULL,
		PAGE_READONLY,
		0, 0, NULL
	);
}

}

MMFile::MMFile()
	: granularity_{GetGranularity()}
	, chunkSize_{PIECES * granularity_}
{
	std::cout << "allocation granularity " << granularity_ << '\n';
}

MMFile::~MMFile() {
	Close();
}

bool MMFile::Open(const char* filename) {
	file_ = MyCreateFile(filename);
	if (file_ == INVALID_HANDLE_VALUE) {
		std::cerr << "Can not open file " << filename << " with error " << GetLastError() << '\n';
		return false;
	}

	mapping_ = MyCreateFileMapping(file_);
	if (mapping_ == NULL) {
		std::cerr << "Can not create mapping with error " << GetLastError() << '\n';
		Close();
		return false;
	}

	buffer_ = MapViewOfFile(
		mapping_,
		FILE_MAP_READ,
		0, 0,
		0
	);
	if (buffer_ == NULL) {
		std::cout << "Can not map view of file with error " << GetLastError() << '\n';
		Close();
		return false;
	}

	return true;
}

bool MMFile::Opened() const {
	return file_ != INVALID_HANDLE_VALUE;
}

void MMFile::Close() {
	if (buffer_ != nullptr) {
		UnmapViewOfFile(buffer_);
		buffer_ = nullptr;
	}
	if (mapping_ != nullptr) {
		CloseHandle(mapping_);
		mapping_ = nullptr;
	}
	if (file_ != INVALID_HANDLE_VALUE) {
		CloseHandle(file_);
	}
}

bool MMFile::GetNextLine(const char*& str, int& len) {
	const char* buffer;
	int bufferLength;
	if (!GetBuffer(buffer, bufferLength)) {
		return false;
	}

	int pos = 0;
	while (pos < bufferLength && buffer[pos] != '\n') {
		++pos;
	}
	str = buffer;
	len = pos;
	Seek(pos);
	return true;
}

bool MMFile::GetBuffer(const char*& buffer, int& len) {
	return false;
}

void MMFile::Seek(int pos) {
	pos_ += pos;
}
