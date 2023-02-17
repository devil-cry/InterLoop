//
// Created by devilcry on 2022/12/27.
//

#ifndef INTERLOOP_FREELOOP_H
#define INTERLOOP_FREELOOP_H

#include <vector>

#include "bedpe/bedpe.h"
#include "bedpe/bedpeNode.h"

class FreeLoop{
public:
    void set_vec_bedpe(const std::vector<Bedpe>& vec_bedpe);    // step 1
    void set_group(const std::string& group);
    void set_allow_dis_max(int64_t val);
    void preprocess();  // step 2
    void add_edges(bool is_L2R);    // step 3
    void clustering(bool base_on_left);     // step 4
    void merging_loops();
    void run();
    std::vector<Bedpe> get_results();

private:
    std::string group;
    std::vector<BedpeNode> vec_bedpe_nodes;   // input bedpe data
    int64_t allow_dis_max = 10000;

    /**
     * left_anchor_mx_idx, right_anchor_mx_idx
     * Used max index of bedpe to represent the length of the chromosome
     */
    int64_t left_anchor_mx_idx;
    int64_t right_anchor_mx_idx;

//    struct Edge {
//        int to;
//        int64_t bias_value;
//        bool operator < (const Edge &t) const {
//            return to < t.to;
//        }
//    };

    std::vector<std::vector<int>> edges;
    std::vector<Bedpe> vec_res;
};

#endif //INTERLOOP_FREELOOP_H
