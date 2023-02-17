//
// Created by devilcry on 2023/1/17.
//

#include "bedpe/bedpeNode.h"

BedpeNode::BedpeNode() = default;

BedpeNode::BedpeNode(const Bedpe &bedpe, int id) {
    this->st_L = bedpe.st_L;
    this->en_L = bedpe.en_L;
    this->st_R = bedpe.st_R;
    this->en_R = bedpe.en_R;
    this->id = id;
}

bool BedpeNode::cmp_left_anchor_first(const BedpeNode &lsh, const BedpeNode &rsh) {
    if (lsh.st_L < rsh.st_L) {
        return lsh.st_L < rsh.st_L;
    }
    return lsh.en_L < rsh.en_L;
}

bool BedpeNode::cmp_right_anchor_first(const BedpeNode &lsh, const BedpeNode &rsh) {
    if (lsh.st_R < rsh.st_R) {
        return lsh.st_R < rsh.st_R;
    }
    return lsh.en_R < rsh.en_R;
}

bool BedpeNode::cmp_initial(const BedpeNode &lsh, const BedpeNode &rsh) {
    return lsh.id < rsh.id;
}

//bool BedpeNode::has_overlap_with(const BedpeNode &node) {
//    bool ok_left, ok_right;
//    ok_left = ( (st_L <= node.st_L && node.st_L <= en_L) || (st_L > node.st_L && st_L <= node.en_L) );
//    ok_right = ( (st_R <= node.st_R && node.st_R <= en_R) || (st_R > node.st_R && st_R <= node.en_R) );
//    return ok_left && ok_right;
//}

//int64_t BedpeNode::cal_bias_value_with(const BedpeNode &node) {
//
//}