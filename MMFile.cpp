#include "MMFile.hpp"

#include <stdio.h>

namespace {

constexpr DWORD PIECES = 32;

DWORD High(size_t n) {
	return n >> 32;
}

DWORD Low(size_t n) {
	return n & 0xFFFFFFFF;
}

size_t Pack(DWORD high, DWORD low) {
	return (static_cast<size_t>(high) << 32) | static_cast<size_t>(low);
}

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

bool MyGetFileSize(HANDLE file, size_t& filesize) {
	BY_HANDLE_FILE_INFORMATION fileinfo;
	if (!GetFileInformationByHandle(file, &fileinfo)) {
		return false;
	}

	filesize = Pack(fileinfo.nFileSizeHigh, fileinfo.nFileSizeLow);
	return true;
}

HANDLE MyCreateFileMapping(HANDLE file) {
	return CreateFileMapping(
		file,
		NULL,
		PAGE_READONLY,
		0, 0, NULL
	);
}

void* MyMapViewOfFile(HANDLE mapping, size_t offset, size_t chunkSize) {
	return MapViewOfFile(
		mapping,
		FILE_MAP_READ,
		High(offset),
		Low(offset),
		chunkSize
	);
}

}

MMFile::MMFile()
	: granularity_{GetGranularity()}
	, chunkSize_{PIECES * granularity_}
{
}

MMFile::~MMFile() {
	Close();
}

bool MMFile::Open(const char* filename) {
	file_ = MyCreateFile(filename);
	if (file_ == INVALID_HANDLE_VALUE) {
		printf("Can not open file \"%s\" with error %d\n", filename, GetLastError());
		return false;
	}

	if (!MyGetFileSize(file_, filesize_)) {
		printf("Can not get file \"%s\" size with error %d\n", filename, GetLastError());
		Close();
		return false;
	}

	mapping_ = MyCreateFileMapping(file_);
	if (mapping_ == NULL) {
		printf("Can not create mapping with error %d\n", GetLastError());
		Close();
		return false;
	}

	buffer_ = MyMapViewOfFile(mapping_, 0, filesize_);
	if (buffer_ == NULL) {
		printf("Can not map view of file with error %d\n", GetLastError());
		Close();
		return false;
	}

	pos_ = 0;

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
	filesize_ = 0;
	if (file_ != INVALID_HANDLE_VALUE) {
		CloseHandle(file_);
	}
}

bool MMFile::GetNextLine(const char*& str, size_t& len) {
	const char* buffer;
	size_t bufferLength;
	if (!GetBuffer(buffer, bufferLength)) {
		return false;
	}

	size_t pos = 0;
	while (pos < bufferLength && buffer[pos] != '\n') {
		++pos;
	}
	str = buffer;
	len = pos;
	Seek(pos + 1);
	return true;
}

bool MMFile::GetBuffer(const char*& buffer, size_t& len) {
	if (pos_ >= filesize_) {
		return false;
	}

	buffer = static_cast<const char*>(buffer_) + pos_;
	len = filesize_ - pos_;
	return true;
}

void MMFile::Seek(size_t pos) {
	pos_ += pos;
}
