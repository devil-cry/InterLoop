//
// Created by devilcry on 2023/1/17.
//

#ifndef INTERLOOP_BEDPE_NODE_H
#define INTERLOOP_BEDPE_NODE_H

#include "bedpe/bedpe.h"

class BedpeNode : public Bedpe {
public:
    int id;

    BedpeNode();
    BedpeNode(const Bedpe& bedpe, int id);

    static bool cmp_left_anchor_first(const BedpeNode& lsh, const BedpeNode& rsh);
    static bool cmp_right_anchor_first(const BedpeNode& lsh, const BedpeNode& rsh);
    static bool cmp_initial(const BedpeNode& lsh, const BedpeNode& rsh);

//    bool has_overlap_with(const BedpeNode& node);
    int64_t cal_bias_value_with(const BedpeNode& node);

};

#endif //INTERLOOP_BEDPE_NODE_H
