//
// Created by devilcry on 2022/12/16.
//

#ifndef INTERLOOP_VIEW_H
#define INTERLOOP_VIEW_H

#include <string>
#include <vector>

#include "bedpe/bedpe.h"
#include "logger/logger.h"

class View {
public:
    static void show_golden_in_valid_bedpe(const std::string& file_name, const std::string& group,
                                           const std::vector<Bedpe>& vec_bedpe, Logger& logger);
    static void show_golden_cover_valid_bedpe_cnt(const std::string& file_name, const std::string& group,
                                                  const std::vector<Bedpe>& vec_bedpe, Logger& logger);
};

#endif //INTERLOOP_VIEW_H
