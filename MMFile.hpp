#pragma once

#include <Windows.h>

class MMFile {
public:
	MMFile();
	~MMFile();

	bool Open(const char* filename);
	void Close();

	bool GetNextLine(const char*& str, size_t& len);
private:
	bool GetBuffer(const char*& buffer, size_t& len);
	bool LoadNextChunk();
	bool LoadChunk(size_t offset, size_t bytesToRead);
	void Seek(size_t pos);

	void UnmapBuffer();
	void DestroyFileMapping();
	void CloseFile();

	DWORD granularity_{ 0 };
	size_t chunkSize_{ 0 };

	size_t currentOffset_{ 0 };
	size_t currentEnd_{ 0 };
	size_t currentChunkSize_{ 0 };

	HANDLE file_{ INVALID_HANDLE_VALUE };
	size_t filesize_{ 0 };
	HANDLE mapping_{ nullptr };
	void* rawBuffer_{ nullptr };
	const char* buffer_{ nullptr };
	size_t pos_{ 0 };
};
