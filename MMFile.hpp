#pragma once

#include <Windows.h>

class MMFile {
public:
	MMFile();
	~MMFile();

	bool Open(const char* filename);
	bool Opened() const;	
	void Close();

	bool GetNextLine(const char*& str, size_t& len);
private:
	bool GetBuffer(const char*& buffer, size_t& len);
	void Seek(size_t pos);

	DWORD granularity_;
	DWORD chunkSize_;

	HANDLE file_{ INVALID_HANDLE_VALUE };
	size_t filesize_{ 0 };
	HANDLE mapping_{ nullptr };
	void* buffer_{ nullptr };
	size_t pos_{ 0 };
};
