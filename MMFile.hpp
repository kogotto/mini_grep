#pragma once

#include <Windows.h>

class MMFile {
public:
	MMFile();
	~MMFile();

	bool Open(const char* filename);
	bool Opened() const;	
	void Close();

	bool GetNextLine(const char*& str, int& len);
private:
	bool GetBuffer(const char*& buffer, int& len);
	void Seek(int pos);

	DWORD granularity_;
	DWORD chunkSize_;

	HANDLE file_{ INVALID_HANDLE_VALUE };
	HANDLE mapping_{ nullptr };
	void* buffer_{ nullptr };
	int pos_{ 0 };
};
