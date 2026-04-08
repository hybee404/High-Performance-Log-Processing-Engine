#include <iostream>
#include <vector>


struct Chunk {
    size_t chunk_start;
    size_t chunk_end;
};

std::vector<Chunk> splitChunks(char* data, size_t size, size_t numChunks);