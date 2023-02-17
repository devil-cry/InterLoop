//
// Created by devilcry on 2022/12/7.
//

#include <sstream>
#include <direct.h>
#include <algorithm>


#include "io/io.h"
#include "share/global_def.h"

using namespace global_def;

IO::IO() = default;

std::set<std::string> IO::get_include_pairs(const std::string &inc_pairs, Logger& logger) {
    std::set<std::string> set_pairs;
    if (inc_pairs == "all") {
        return set_pairs;
    }

    std::vector<std::string> vec_pairs;
    split(inc_pairs, vec_pairs, ",");
    for (auto &pair : vec_pairs) {
        std::vector<std::string> vec_chs;
        split(pair, vec_chs, "-");
        if (vec_chs.size() != 2) {
            logger.ERROR("unsupported format of argument '-inc_pair/--include_pairs' value. Please check and try again.");
            exit(1);
        }
        if (cmp_chrome(vec_chs[0], vec_chs[1]) == 1) {
            std::swap(vec_chs[0], vec_chs[1]);
        }
        set_pairs.insert(vec_chs[0] + "-" + vec_chs[1]);
    }

    return set_pairs;
}

void IO::read_bedpe_file(const std::string& file, const std::set<std::string>& inc_pairs,
                         std::map<std::string, std::vector<Bedpe>>& mp, Logger& logger) {
    logger.INFO("Start reading file : %s.", file.c_str());

    static char line[MAX_READ_LINE_LENGTH];

    std::ifstream ifs = std::ifstream(file.c_str());
    long long int error_lines = 0;
    long long int all_lines = 0;
    std::string str;
    Bedpe tmp_bedpe;
    while (ifs.getline(line, MAX_READ_LINE_LENGTH)) {
        all_lines++;

        std::stringstream ss(line);
        std::vector<std::string> vec_elements;
        while (ss >> str) {
            vec_elements.emplace_back(str);
        }

        if (!Bedpe::check_if_vec_convert2_bedpe(vec_elements)) {
            error_lines++;
            continue;
        }

        tmp_bedpe = Bedpe(vec_elements[0], atoll(vec_elements[1].c_str()), atoll(vec_elements[2].c_str()),
                          vec_elements[3], atoll(vec_elements[4].c_str()), atoll(vec_elements[5].c_str()));

        std::string group = tmp_bedpe.chr_L + "-" + tmp_bedpe.chr_R;
        if (inc_pairs.empty() || inc_pairs.find(group) != inc_pairs.end()) {
            mp[group].emplace_back(tmp_bedpe);
        }
    }

    if (error_lines == all_lines) {
        logger.ERROR("Not found any valid 'bedpe', please check the format of your input file!");
        exit(1);
    }

    if (error_lines > 0 && all_lines > 0) {
        double rate = 1.0 * error_lines / all_lines * 100;
//        std::string msg = "Found " + std::to_string(error_lines) + "/" + std::to_string(all_lines);
        logger.WARN("Found wrong format lines %lld/%lld(%.3lf%%).", error_lines, all_lines, rate);
    }

//    for (auto &[key, value] : mp) {
//        std::sort(value.begin(), value.end());
//    }

    for (const auto &[key, value] : mp) {
        logger.INFO("pair %s found %d 'bedpe' data line.", key.c_str(), value.size());
    }

    logger.INFO("Read file %s finished.", file.c_str());
//    logger.INFO("Read file %s finished, parsed %d bedpe pairs!", file.c_str(), vec_bedpe.size());

}

void IO::output_loop_file(const std::string &file, const std::vector<Bedpe> &vec_loop, Logger &logger) {

    std::string dir = file.substr(0, file.find_first_of('/'));
    if(!dir.empty()) {
        if (_access(dir.c_str(), 0) != -1) {
            _rmdir(dir.c_str());
        }
        _mkdir(dir.c_str());
    }

    std::ofstream ofs = std::ofstream(file.c_str());

    for(const auto &loop : vec_loop) {
        ofs << loop << "\n";
    }

}
