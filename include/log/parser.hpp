#include <string_view>

struct LogEvent {
    std::string_view TIMESTAMP;
    std::string_view LEVEL;
    std::string_view CONTENT;
};

class Parser{
    public:
    static LogEvent parse(std::string_view line);
};

std::ostream& operator<<(std::ostream& os, const LogEvent& e);