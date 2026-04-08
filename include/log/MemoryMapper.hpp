#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

class MemoryMappedFile {
public:
    MemoryMappedFile(const std::string& filepath);
    ~MemoryMappedFile();

    bool open();
    void flush() ;
    void close();
    char* getData();
    size_t getSize();

private:
    std::string filePath;
    char* data;
    size_t size;
#ifdef _WIN32
    HANDLE hFile;
    HANDLE hMap;
#endif
};

