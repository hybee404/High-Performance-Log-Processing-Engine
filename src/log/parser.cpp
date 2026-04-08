#include <iostream>
#include "log/parser.hpp"

LogEvent Parser::parse(std::string_view line){
    LogEvent event;
    unsigned int timestamp_start = line.find('[');
    unsigned int timestamp_end = line.find(']');

    event.TIMESTAMP = line.substr(timestamp_start + 1, timestamp_end - timestamp_start - 1);

    unsigned int level_start = line.find('[',timestamp_end);
    unsigned int level_end = line.find(']', level_start);

    event.LEVEL = line.substr(level_start + 1, level_end - level_start - 1);

    event.CONTENT = line.substr(level_end + 2);

    return event;
}

std::ostream& operator<<(std::ostream& os, const LogEvent& e) {
    os << e.TIMESTAMP << " "
       << e.LEVEL << " "
       << e.CONTENT;
    return os;
}