#include <deque>
#include <mutex>
#include <iostream>
#include "log/parser.hpp"

class MetricsEngine {
public:
    MetricsEngine(size_t windowSize);

    void process(const LogEvent& e);

    void printStats();

private:
    std::deque<LogEvent> window;
    size_t maxSize;
    bool hasAlerted = false;

    int errorCount = 0;
    int noticeCount = 0;

    std::mutex mtx;

    void addEvent(const LogEvent& e);
    void removeEvent(const LogEvent& e);
    void checkAnomaly();
};