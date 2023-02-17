//
// Created by devilcry on 2022/12/17.
//

#include <sstream>

#include "view/view.h"
#include "share/global_def.h"

template<typename T> T RandT(T _min, T _max)
{
    T temp;
    if (_min > _max)
    {
        temp = _max;
        _max = _min;
        _min = temp;
    }
    return rand() / (double)RAND_MAX *(_max - _min) + _min;
}

void View::show_golden_in_valid_bedpe(const std::string& file_name, const std::string& group,
                                      const std::vector<Bedpe>& vec_bedpe, Logger& logger)
{
    char line[MAX_READ_LINE_LENGTH];

    std::ifstream ifs = std::ifstream(file_name.c_str());
    long long int error_lines = 0;
    long long int all_lines = 0;
    std::string str;
    Bedpe tmp_bedpe;

    long long int score = 0;

    int width = 5000000;
    std::vector<int> l_score(100000, 0), r_score(100000, 0);
    std::vector<int> l_cnt(100000, 0), r_cnt(100000, 0);
    int min_pos = std::numeric_limits<int>::max(), max_pos = std::numeric_limits<int>::min();
    for(auto &bedpe : vec_bedpe) {
        min_pos = std::min(min_pos, std::min(int(bedpe.st_L), int(bedpe.st_R)));
        max_pos = std::max(max_pos, std::max(int(bedpe.en_L), int(bedpe.en_R)));
        int l_pos = (bedpe.st_L / width + bedpe.en_L / width) / 2;
        int r_pos = (bedpe.st_R / width + bedpe.en_R / width) / 2;
        l_cnt[l_pos]++;
        r_cnt[r_pos]++;
    }

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

        std::string cur_group = tmp_bedpe.chr_L + "-" + tmp_bedpe.chr_R;
        if (cur_group == group) {
            int l_pos = (tmp_bedpe.st_L / width + tmp_bedpe.en_L / width) / 2;
            int r_pos = (tmp_bedpe.st_R / width + tmp_bedpe.en_R / width) / 2;
#ifdef _DEBUG
            l_pos = RandT(min_pos / width, max_pos / width);
            r_pos = RandT(min_pos / width, max_pos / width);
#endif
            int s = l_cnt[l_pos] * r_cnt[r_pos] + std::max(l_cnt[l_pos], r_cnt[r_pos]) * std::max(l_cnt[l_pos],
                                                                                                   r_cnt[r_pos]);
            score += s;
        }
    }
    logger.INFO("group %s score : %lld .", group.c_str(), score);
}

void View::show_golden_cover_valid_bedpe_cnt(const std::string &file_name, const std::string &group,
                                             const std::vector<Bedpe> &vec_bedpe, Logger &logger) {
    char line[MAX_READ_LINE_LENGTH];

    std::ifstream ifs = std::ifstream(file_name.c_str());
    int64_t error_lines = 0;
    int64_t all_lines = 0;
    std::string str;
    Bedpe tmp_bedpe;

    int64_t score = 0;

    int width = 5000000;
    std::vector<int> l_score(100000, 0), r_score(100000, 0);
    std::vector<int> l_cnt(100000, 0), r_cnt(100000, 0);
    int min_pos = std::numeric_limits<int>::max(), max_pos = std::numeric_limits<int>::min();
    for(auto &bedpe : vec_bedpe) {
        min_pos = std::min(min_pos, std::min(int(bedpe.st_L), int(bedpe.st_R)));
        max_pos = std::max(max_pos, std::max(int(bedpe.en_L), int(bedpe.en_R)));
        int l_pos = (bedpe.st_L / width + bedpe.en_L / width) / 2;
        int r_pos = (bedpe.st_R / width + bedpe.en_R / width) / 2;
        l_cnt[l_pos]++;
        r_cnt[r_pos]++;
    }

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

        std::string cur_group = tmp_bedpe.chr_L + "-" + tmp_bedpe.chr_R;
        if (cur_group == group) {
            int l_pos = (tmp_bedpe.st_L / width + tmp_bedpe.en_L / width) / 2;
            int r_pos = (tmp_bedpe.st_R / width + tmp_bedpe.en_R / width) / 2;
#ifdef _DEBUG
            l_pos = RandT(min_pos / width, max_pos / width);
            r_pos = RandT(min_pos / width, max_pos / width);
#endif
            int s = l_cnt[l_pos] * r_cnt[r_pos] + std::max(l_cnt[l_pos], r_cnt[r_pos]) * std::max(l_cnt[l_pos],
                                                                                                  r_cnt[r_pos]);
            score += s;
        }
    }
    logger.INFO("group %s score : %lld .", group.c_str(), score);
}
