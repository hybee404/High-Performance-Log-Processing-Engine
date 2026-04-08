#include "core/chunk.hpp"
#include <vector>

std::vector<Chunk> splitChunks(char* data, size_t size, size_t numThread) {
    std::vector<Chunk> chunks;
    size_t chunkSize = size / numThread;
    size_t start = 0;

    for (size_t i = 0; i < numThread; ++i) {
        size_t end = (i == numThread - 1) ? size : start + chunkSize;
    
        while (end < size && data[end] != '\n') {
            end++;
        }

        chunks.push_back({start, end});
        start = end + 1;
    }

    return chunks;
}
