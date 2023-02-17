//
// Created by devilcry on 2022/12/7.
//

#include <iostream>

#include "bedpe/bedpe.h"
#include "share/global_def.h"

using namespace global_def;

Bedpe::Bedpe(const std::string& chr_L, int64_t st_L, int64_t en_L,
             const std::string& chr_R, int64_t st_R, int64_t en_R) {
    this->chr_L = chr_L;
    this->st_L = st_L;
    this->en_L = en_L;
    this->chr_R = chr_R;
    this->st_R = st_R;
    this->en_R = en_R;
    if (chr_L > chr_R) {
        std::swap(this->chr_L, this->chr_R);
        std::swap(this->st_L, this->st_R);
        std::swap(this->en_L, this->en_R);
    }
}

Bedpe::Bedpe() = default;

std::ostream & operator<<(std::ostream& os,const Bedpe& bedpe) {
    std::string seq = "\t";
    os << bedpe.chr_L << seq << bedpe.st_L << seq << bedpe.en_L << seq;
    os << bedpe.chr_R << seq << bedpe.st_R << seq << bedpe.en_R;
    return os;
}

bool operator<(const Bedpe &lsh, const Bedpe &rsh) {
    if (lsh.st_L != rsh.st_L) {
        return lsh.st_L < rsh.st_L;
    }
    return lsh.en_L < rsh.en_L;
}

//bool Bedpe::cmp_anchor_right_first(const Bedpe &lsh, const Bedpe &rsh) {
//    if (lsh.st_R != rsh.st_R) {
//        return lsh.st_R < rsh.st_R;
//    }
//    return lsh.en_R < rsh.en_R;
//}

/**
 * check if str array can convert to class 'Bedpe'
 *
 * @param const std::vector<std::string>
 * @return bool
 */
bool Bedpe::check_if_vec_convert2_bedpe(const std::vector<std::string> &vec_str) {
    if (vec_str.size() < psm_bedpe_valid_elements)
        return false;
    if (!parse_int(vec_str[1]) || !parse_int(vec_str[2]))
        return false;
    if (!parse_int(vec_str[4]) || !parse_int(vec_str[5]))
        return false;
    return true;
}
