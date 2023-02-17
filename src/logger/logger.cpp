//
// Created by devilcry on 2022/12/3.
//

#include <algorithm>
#include <fstream>
#include <map>
#include <vector>
#include <cstdarg>
#include <ctime>
#include <mutex>

#include "share/global_def.h"
#include "logger/logger.h"

enum LOG_LEVEL
{
    DEBUG,
    INFO,
    WARN,
    ERROR
};

std::mutex mtx_logger;

static std::map<std::string, int> s_level_map = {
        {"DEBUG", 0},
        {"INFO", 1},
        {"WARN", 2},
        {"ERROR", 3}
};
static std::string sep = "\t";
static std::vector<std::string> LOG_LEVEL_NAME = {"DEBUG", "INFO", "WARN", "ERROR"};

int check_level(const std::string& level) {
    std::string ts = level;
    std::transform(ts.begin(), ts.end(), ts.begin(), ::toupper);
    switch (s_level_map[ts]) {
        case DEBUG:
            return 0;
        case INFO:
            return 1;
        case WARN:
            return 2;
        case ERROR:
            return 3;
    }
    return 1;
}

/**
 * @param log_file
 * @param level
 */
Logger::Logger(const std::string& log_file, const std::string& level) {
    pm_ofs = std::ofstream(log_file.c_str(), std::ios::out | std::ios::app);
    pm_level = check_level(level);
}

inline void Logger::print_log_head(const std::string &file_name, int line) {
    auto t = time(nullptr);
    auto date = localtime(&t);
    static char ch_head[MAX_LOGGER_HEAD_LENGTH];
    sprintf(ch_head, "%04d-%02d-%02d%s%02d:%02d:%02d%s%s:%d%s",
            1900 + date->tm_year, 1 + date->tm_mon, date->tm_mday, sep.c_str(),
            date->tm_hour, date->tm_min, date->tm_sec, sep.c_str(),
            file_name.c_str(), line, sep.c_str());
    pm_ofs << ch_head;
}

inline void Logger::print_log_msg_level(int msg_level) {
    pm_ofs << "[" << LOG_LEVEL_NAME[msg_level] << "]" << sep;
}

inline void Logger::print_log_msg(const char* fmt, va_list arg_list) {
    static char buf[MAX_LOGGER_MSG_LENGTH];
    std::string s;
    vsprintf(buf, fmt, arg_list);
    pm_ofs << buf << "\n" << std::flush;
    puts(buf);
}

void Logger::debug(const std::string& file_name, int line, const char *fmt, ...) {
    if (pm_level > DEBUG) {
        return;
    }
    mtx_logger.lock();

    std::string file_name_relative = file_name.substr(file_name.find(PROGRAM_NAME));
    print_log_head(file_name_relative, line);
    print_log_msg_level(s_level_map[LOG_LEVEL_NAME[DEBUG]]);
    va_list argptr;
    va_start(argptr, fmt);
    va_end(argptr);
    print_log_msg(fmt, argptr);

    mtx_logger.unlock();
}

void Logger::info(const std::string& file_name, int line, const char *fmt, ...) {
    if (pm_level > INFO) {
        return;
    }
    mtx_logger.lock();

    std::string file_name_relative = file_name.substr(file_name.find(PROGRAM_NAME));
    print_log_head(file_name_relative, line);
    print_log_msg_level(s_level_map[LOG_LEVEL_NAME[INFO]]);
    va_list argptr;
    va_start(argptr, fmt);
    va_end(argptr);
    print_log_msg(fmt, argptr);

    mtx_logger.unlock();
}

void Logger::warn(const std::string& file_name, int line, const char *fmt, ...) {
    if (pm_level > WARN) {
        return;
    }
    mtx_logger.lock();

    std::string file_name_relative = file_name.substr(file_name.find(PROGRAM_NAME));
    print_log_head(file_name_relative, line);
    print_log_msg_level(s_level_map[LOG_LEVEL_NAME[WARN]]);
    va_list argptr;
    va_start(argptr, fmt);
    va_end(argptr);
    print_log_msg(fmt, argptr);

    mtx_logger.unlock();
}

void Logger::error(const std::string& file_name, int line, const char *fmt, ...) {
    if (pm_level > ERROR) {
        return;
    }
    mtx_logger.lock();

    std::string file_name_relative = file_name.substr(file_name.find(PROGRAM_NAME));
    print_log_head(file_name_relative, line);
    print_log_msg_level(s_level_map[LOG_LEVEL_NAME[ERROR]]);
    va_list argptr;
    va_start(argptr, fmt);
    va_end(argptr);
    print_log_msg(fmt, argptr);

    mtx_logger.unlock();
}