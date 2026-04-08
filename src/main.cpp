#include <iostream>
#include "log/MemoryMapper.hpp"
#include <chrono>
#include <thread>
#include "core/threadpool.hpp"
#include "core/chunk.hpp"
#include "core/metrics.hpp"


int main(int argc, char* argv[]){

      if (argc < 2) {
        std::cerr << "Usage: ./log_engine <logfile>\n";
        return 1;
    }


    MemoryMappedFile mmf(argv[1]);
    if(!mmf.open()){
        std::cerr<<"Error: Cannot open file\n";
    }

    char* data = mmf.getData();
    size_t file_size = mmf.getSize();

    auto start_time = std::chrono::high_resolution_clock::now();
    size_t numThreads = std::thread::hardware_concurrency();
    auto chunks = splitChunks(data, file_size, numThreads);
    ThreadPool pool(numThreads);


    std::vector<std::vector<LogEvent>> outputs(chunks.size());
    MetricsEngine metrics(100); 

    for (size_t i = 0; i < chunks.size(); ++i) {
        pool.enqueue([&, i]() {

            size_t start = chunks[i].chunk_start;
            size_t end = chunks[i].chunk_end;

            size_t lineStart = start;
            for (size_t j = start; j < end; ++j) {

                if (data[j] == '\n') {

                    std::string_view line(&data[lineStart], j - lineStart);
                    auto event = Parser::parse(line);
                   
                    outputs[i].push_back(event);
                           lineStart = j + 1;
                }
            
        }});
    }
    
    pool.wait();
      for (auto& chunk : outputs) {
    for (auto& event : chunk) {
        metrics.process(event);
      
    }
    }

auto end_time = std::chrono::high_resolution_clock::now();
 std::chrono::duration<double> elapsed = end_time - start_time;
metrics.printStats();
  std::cout << "Time taken: " << elapsed.count() << " seconds" << std::endl;

mmf.close();
    return 0;
}

