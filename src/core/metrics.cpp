#include "core/metrics.hpp"

MetricsEngine::MetricsEngine(size_t windowSize)
    : maxSize(windowSize) {}

void MetricsEngine::process(const LogEvent& e) {
    std::lock_guard<std::mutex> lock(mtx);

    window.push_back(e);
    addEvent(e);
    
    if (window.size() > maxSize) {
        auto old = window.front();
        window.pop_front();
        removeEvent(old);
    }
    checkAnomaly();
}

void MetricsEngine::addEvent(const LogEvent& e) {
    if (e.LEVEL == "error" ) errorCount++;
    else if (e.LEVEL == "notice") noticeCount++;
}

void MetricsEngine::removeEvent(const LogEvent& e) {
    if (e.LEVEL == "error") errorCount--;
    else if (e.LEVEL == "notice") noticeCount--;
}

void MetricsEngine::checkAnomaly() {
double errorRate = (double)errorCount / window.size();


    if(errorRate <= 0.2) hasAlerted = false;

    if (errorRate > 0.2 && !hasAlerted) {

        std::cout << "\n ALERT: Error spike detected!\n";
        std::cout << "Error count: " << errorCount << "\n";
        std::cout << "Recent error logs:\n";

        hasAlerted = true;

        for (const auto& e : window) {
          if(e.LEVEL == "error"){
                std::cout << e.TIMESTAMP << " "
                          << e.LEVEL << " "
                          << e.CONTENT << "\n";
          }
        }

        std::cout << "-----------------------------\n";
    }
}

void MetricsEngine::printStats() {
    std::lock_guard<std::mutex> lock(mtx);

    std::cout << "\n--- Metrics ---\n";
    std::cout << "Window size: " << window.size() << "\n";
    std::cout << "Errors: " << errorCount << "\n";
    std::cout << "Notices: " << noticeCount << "\n";
}