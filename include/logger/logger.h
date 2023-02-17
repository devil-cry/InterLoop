//
// Created by devilcry on 2022/12/2.
//

#ifndef INTERLOOP_LOGGER_H
#define INTERLOOP_LOGGER_H

#include <cstring>
#include <fstream>

class Logger{
public:
    Logger(const std::string& log_file, const std::string& level);

    void debug(const std::string& file_name, int line, const char *fmt, ...);
    void info(const std::string& file_name, int line, const char *fmt, ...);
    void warn(const std::string& file_name, int line, const char *fmt, ...);
    void error(const std::string& file_name, int line, const char *fmt, ...);

private:
    std::ofstream pm_ofs;
    int pm_level;
    inline void print_log_head(const std::string& file_name, int line);
    inline void print_log_msg_level(int msg_level);
    inline void print_log_msg(const char *fmt, va_list list);
};


#endif //INTERLOOP_LOGGER_H
