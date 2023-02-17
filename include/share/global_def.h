//
// Created by devilcry on 2022/12/7.
//

#ifndef INTERLOOP_GLOBAL_DEF_H
#define INTERLOOP_GLOBAL_DEF_H

#include <algorithm>

namespace global_def{

#define PROGRAM_NAME "InterLoop"
#define PROGRAM_VERSION "1.0.0"

#define LOGGER_SUP __FILE__, __LINE__
#define DEBUG(msg, args...) debug(LOGGER_SUP, msg, ##args)
#define INFO(msg, args...) info(LOGGER_SUP, msg, ##args)
#define WARN(msg, args...) warn(LOGGER_SUP, msg, ##args)
#define ERROR(msg, args...) error(LOGGER_SUP, msg, ##args)

#define MAX_LOGGER_HEAD_LENGTH 200
#define MAX_LOGGER_MSG_LENGTH 1000

#define MAX_READ_LINE_LENGTH 1000


    /**
     * split string s in vector v with chars from string c
     *
     * @param s
     * @param v
     * @param c
     */
    static void split(const std::string &s, std::vector<std::string> &v, const std::string &c) {
        auto len = s.length();
        size_t pos1 = 0, pos2 = s.find(c);
        while (std::string::npos != pos2) {
            v.emplace_back(s.substr(pos1, pos2 - pos1));
            pos1 = pos2 + c.size();
            pos2 = s.find(c, pos1);
        }
        if (pos1 != len) {
            v.emplace_back(s.substr(pos1));
        }
    }

    /**
     * used to sort chromes by sequence of number
     *
     * @param const std::string& l
     * @param const std::string& r
     * @return int
     */
    static int cmp_chrome(const std::string &l, const std::string &r) {
        int lv = 0, rv = 0;
        for (auto ch : l) {
            if (std::isdigit(ch)) {
                lv = lv * 10 + ch - '0';
            }
        }
        for (auto ch : r) {
            if (std::isdigit(ch)) {
                rv = rv * 10 + ch - '0';
            }
        }
        if (lv == rv)
            return 0;
        return lv < rv ? -1 : 1;
    }

    /**
     * check if the string only contains 10 types of digits.
     *
     * @param const std::string& str
     * @return bool
     */
    static bool parse_int(const std::string &str) {
        auto it = std::find_if(str.begin(), str.end(), [](char const &c) {
            return !std::isdigit(c);
        });
        return !str.empty() && it == str.end();
    }

}

#endif //INTERLOOP_GLOBAL_DEF_H
