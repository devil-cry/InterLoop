//
// Created by devilcry on 2022/12/3.
//

#ifndef INTERLOOP_BEDPE_H
#define INTERLOOP_BEDPE_H

#include <cstring>
#include <vector>
#include <fstream>

class Bedpe{
public:
    static int psm_bedpe_valid_elements;
    static std::vector<std::string> psm_inc_chromes;

    std::string chr_L;
    int64_t st_L, en_L;
    std::string chr_R;
    int64_t st_R, en_R;

    friend std::ostream& operator<<(std::ostream& os, const Bedpe& c);

    friend bool operator<(const Bedpe& lsh, const Bedpe& rsh);

//    static bool cmp_anchor_right_first(const Bedpe& lsh, const Bedpe& rsh);

    Bedpe();

    Bedpe(const std::string& chr_L, int64_t st_L, int64_t en_L, const std::string& chr_R, int64_t st_R, int64_t en_R);

    static bool check_if_vec_convert2_bedpe(const std::vector<std::string>& vec_str);

};

#endif //INTERLOOP_BEDPE_H
