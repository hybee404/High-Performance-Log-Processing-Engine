#include <iostream>
#include <string>
#include "log/MemoryMapper.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

MemoryMappedFile::MemoryMappedFile(const std::string& filepath) {

     filePath = filepath;
     data=nullptr;
     size= 0;

#ifdef _WIN32
     hFile = NULL;
     hMap = NULL;
#endif 
}

MemoryMappedFile::~MemoryMappedFile() {
    close();
}

    bool MemoryMappedFile::open() {
#ifdef _WIN32
        hFile = CreateFileA(filePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) return false;

        LARGE_INTEGER filesize;
        if (!GetFileSizeEx(hFile, &filesize)) return false;
        size = static_cast<size_t>(filesize.QuadPart);

        hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
        if (!hMap) return false;

        data = (char*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (!data) return false;

#else
        int fd = ::open(filepath.c_str(), O_RDWR);
        if (fd == -1) return false;

        struct stat st;
        if (fstat(fd, &st) == -1) return false;
        size = st.st_size;

        data = (char*)mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) return nullptr;

        ::close(fd);
#endif
        return true;
    }

    void MemoryMappedFile::flush() {
#ifdef _WIN32
        if (data) FlushViewOfFile(data, 0);
#else
        if (data) msync(data, size, MS_SYNC);
#endif
    }

    void MemoryMappedFile::close() {
#ifdef _WIN32
        if (data) UnmapViewOfFile(data); data = nullptr;
        if (hMap) CloseHandle(hMap); hMap = NULL;
        if (hFile) CloseHandle(hFile); hFile = NULL;
#else
        if (data) { munmap(data, size); data = nullptr; }
#endif
    }

    char* MemoryMappedFile::getData() { return data; }
    size_t MemoryMappedFile::getSize() { return size; }


