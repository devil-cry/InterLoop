//
// Created by devilcry on 2022/12/3.
//

#ifndef INTERLOOP_IO_H
#define INTERLOOP_IO_H

#include <vector>
#include <string>
#include <map>
#include <set>

#include "bedpe/bedpe.h"
#include "logger/logger.h"

class IO{
public:
    IO();
    static std::set<std::string> get_include_pairs(const std::string& inc_pairs, Logger& logger);
    static void read_bedpe_file(const std::string& file, const std::set<std::string>& inc_pairs,
                                std::map<std::string, std::vector<Bedpe>>& mp, Logger& logger);
    static void output_loop_file(const std::string& file, const std::vector<Bedpe>& loop, Logger& logger);
};


#endif //INTERLOOP_IO_H
