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
	if (filesize_ == 0) {
		printf("File \"%s\" is empty\n", filename);
		Close();
		return false;
	}

	mapping_ = MyCreateFileMapping(file_);
	if (mapping_ == NULL) {
		printf("Can not create mapping with error %d\n", GetLastError());
		Close();
		return false;
	}

	currentOffset_ = 0;
	currentEnd_ = 0;
	currentChunkSize_ = 0;
	if (!LoadNextChunk()) {
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
	UnmapBuffer();
	DestroyFileMapping();
	CloseFile();
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
	len = (pos != 0 && buffer[pos - 1] == '\r')
		? (pos - 1)
		: pos;
	Seek(pos + 1);
	return true;
}

bool MMFile::GetBuffer(const char*& buffer, size_t& len) {
	if (pos_ >= filesize_) {
		return false;
	}

	if (pos_ >= currentEnd_) {
		if (!LoadNextChunk()) {
			return false;
		}
	}

	buffer = buffer_ + (pos_ - currentOffset_);
	len = currentEnd_ - pos_;
	return true;
}

size_t LastNewLine(const char* begin, const char* end) {
	while (true) {
		if (begin == end) {
			return 0;
		}
		if (*--end == '\n') {
			return end - begin + 1;
		}
	}
}

bool MMFile::LoadNextChunk() {
	const size_t rawNewOffset = currentEnd_;
	const size_t newOffset = (rawNewOffset / granularity_) * granularity_;
	const size_t remainingBytes = filesize_ - newOffset;
	const size_t bytesToRead = min(chunkSize_, remainingBytes);
	if (!LoadChunk(newOffset, bytesToRead)) {
		return false;
	}
	const size_t effectiveWindow = (newOffset + bytesToRead < filesize_)
		? LastNewLine(buffer_, buffer_ + bytesToRead)
		: bytesToRead;
	currentEnd_ = currentOffset_ + effectiveWindow;
	return true;
}

bool MMFile::LoadChunk(size_t offset, size_t bytesToRead) {
	UnmapBuffer();

	currentOffset_ = offset;
	currentChunkSize_ = bytesToRead;
	rawBuffer_ = MyMapViewOfFile(mapping_, currentOffset_, currentChunkSize_);
	if (rawBuffer_ == NULL) {
		printf("Can not map view of file with error %d\n", GetLastError());
		Close();
		return false;
	}
	buffer_ = static_cast<const char*>(rawBuffer_);
	return true;
}

void MMFile::Seek(size_t pos) {
	pos_ += pos;
}

void MMFile::UnmapBuffer() {
	if (rawBuffer_ != nullptr) {
		UnmapViewOfFile(rawBuffer_);
		rawBuffer_ = nullptr;
		buffer_ = nullptr;
	}
}

void MMFile::DestroyFileMapping() {
	if (mapping_ != nullptr) {
		CloseHandle(mapping_);
		mapping_ = nullptr;
	}
}

void MMFile::CloseFile() {
	filesize_ = 0;
	if (file_ != INVALID_HANDLE_VALUE) {
		CloseHandle(file_);
	}
}