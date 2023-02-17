//
// Created by devilcry on 2022/12/27.
//

#include <algorithm>
#include <queue>
#include <set>
#include <iostream>
#include <cmath>

#include "solver/freeloop.h"
#include "bedpe/bedpeNode.h"

void FreeLoop::set_vec_bedpe(const std::vector<Bedpe> &vec_bedpe) {
    vec_bedpe_nodes.clear();
    int id = 0;
    for (const auto &bedpe : vec_bedpe) {
        vec_bedpe_nodes.emplace_back(bedpe, id++);
    }
    edges.clear();
    edges.resize(vec_bedpe_nodes.size());
}

void FreeLoop::set_group(const std::string &group) {
    this->group = group;
}

void FreeLoop::set_allow_dis_max(int64_t val) {
    this->allow_dis_max = val;
}

void FreeLoop::preprocess() {
    if (vec_bedpe_nodes.empty()) {
        return;
    }
    for (const auto& bedpe_node : vec_bedpe_nodes) {
        left_anchor_mx_idx = std::max(left_anchor_mx_idx, bedpe_node.en_L);
        right_anchor_mx_idx = std::max(right_anchor_mx_idx, bedpe_node.en_R);
    }
}

void FreeLoop::add_edges(bool is_L2R)
{
    int fr_id, to_id;
    std::deque<BedpeNode> dq;
    for (const auto& node : vec_bedpe_nodes) {
        dq.emplace_back(node);
        /// Delete nodes which out of the allowed maximum gap range by slider-window
        while (dq.size() > 1) {
            if (is_L2R && std::abs(node.st_L - dq.front().st_L) > allow_dis_max) {
                dq.pop_front();
                continue;
            }
            if (!is_L2R && std::abs(node.st_R - dq.front().st_R) > allow_dis_max) {
                dq.pop_front();
                continue;
            }
            break;
        }
        /// Add edges for current node
        for (auto & it_bedpe_node : dq) {
            fr_id = std::min(node.id, it_bedpe_node.id);
            to_id = std::max(node.id, it_bedpe_node.id);
            if(fr_id == to_id)  // the last element, break
                break;

            if (!is_L2R) {
                std::swap(fr_id, to_id);    // avoid duplicate directed edges
            }
            edges[fr_id].emplace_back(to_id);
        }
    }
}

void FreeLoop::clustering(bool base_on_left) {
    std::deque<BedpeNode> dq;
    std::vector<bool> id_in_q(vec_bedpe_nodes.size());
    int64_t bound = allow_dis_max / 2;
    for (const auto& node : vec_bedpe_nodes) {
        dq.emplace_back(node);
        id_in_q[node.id] = true;
        /**
         * Similar with func 'add_edge()'
         */
        while (dq.size() > 1) {
            if (base_on_left && std::abs(node.st_L - dq.front().st_L) > allow_dis_max) {
                id_in_q[dq.front().id] = false;
                dq.pop_front();
                continue;
            }
            if (!base_on_left && std::abs(node.st_R - dq.front().st_R) > allow_dis_max) {
                id_in_q[dq.front().id] = false;
                dq.pop_front();
                continue;
            }
            break;
        }
        /**
         * Call link back nodes in deque
         */
        std::vector<int> vec_link_back_nodes;
        int nid;
        for (auto &it_bedpe_node : dq) {
            nid = it_bedpe_node.id;
            bool is_at_least_one = false;
            for(auto back_id : edges[nid]) {
                if(id_in_q[back_id]) {
                    vec_link_back_nodes.emplace_back(back_id);
                    is_at_least_one = true;
                }
            }
            if(is_at_least_one) {
                vec_link_back_nodes.emplace_back(nid);
            }
        }
        if(vec_link_back_nodes.empty())
            continue;
        /**
         * Remove duplicate nodes
         */
        std::sort(vec_link_back_nodes.begin(), vec_link_back_nodes.end());
        vec_link_back_nodes.erase(std::unique(vec_link_back_nodes.begin(), vec_link_back_nodes.end()),
                                  vec_link_back_nodes.end());
        /**
         * Generate candidate loops by cluster nodes, store in 'bedpe' format.
         */
//        BedpeNode bn = dq.front();
        BedpeNode bn = dq.at(dq.size()/2);
        Bedpe candidate_loop = Bedpe("", bn.st_L, bn.en_L, "", bn.st_R, bn.en_R);
        auto origin = candidate_loop;
        for(int i = 1; i < dq.size(); i++) {
            if(i == dq.size()/2)
                continue;
            bn = dq.at(i);
            if (base_on_left && abs((bn.st_R + bn.en_R) / 2 - (origin.st_R + origin.en_R) / 2) > bound)
                continue;
            if (!base_on_left && abs((bn.st_L + bn.en_L) / 2 - (origin.st_L + origin.en_L) / 2) > bound)
                continue;
            candidate_loop.st_L = std::min(candidate_loop.st_L, bn.st_L);
            candidate_loop.en_L = std::max(candidate_loop.en_L, bn.en_L);
            candidate_loop.st_R = std::min(candidate_loop.st_R, bn.st_R);
            candidate_loop.en_R = std::max(candidate_loop.en_R, bn.en_R);
        }
        vec_res.emplace_back(candidate_loop);
    }
}

void FreeLoop::merging_loops() {
    if(vec_res.empty()) {
        return;
    }

    std::sort(vec_res.begin(), vec_res.end());

    auto has_overlap = [](const Bedpe& lsh, const Bedpe& rsh) {
        if(lsh.en_L < rsh.st_L || lsh.en_R < rsh.st_R)
            return false;
        if(rsh.en_L < lsh.st_L || rsh.en_R < lsh.st_R)
            return false;
        return true;
    };

    Bedpe t_bedpe = vec_res[0];
    int new_cnt = 0;
    for(int i = 1; i < vec_res.size(); i++) {
        if(has_overlap(t_bedpe, vec_res[i])) {
            t_bedpe.st_L = std::min(t_bedpe.st_L, vec_res[i].st_L);
            t_bedpe.en_L = std::max(t_bedpe.en_L, vec_res[i].en_L);
            t_bedpe.st_R = std::min(t_bedpe.st_R, vec_res[i].st_R);
            t_bedpe.en_R = std::max(t_bedpe.en_R, vec_res[i].en_R);
        } else {
            vec_res[new_cnt++] = t_bedpe;
            t_bedpe = vec_res[i];
        }
    }
    vec_res[new_cnt++] = t_bedpe;

    vec_res.erase(vec_res.begin() + new_cnt, vec_res.end());

    /// set group
    int pos_split_char = group.find_first_of('-');
    std::string chr_L = group.substr(0, pos_split_char);
    std::string chr_R = group.substr(pos_split_char + 1, group.size() - pos_split_char - 1);

    for(auto &loop : vec_res) {
        loop.chr_L = chr_L;
        loop.chr_R = chr_R;
    }
}

void FreeLoop::run() {
    /// Building graph
    std::sort(vec_bedpe_nodes.begin(), vec_bedpe_nodes.end(), BedpeNode::cmp_left_anchor_first);
    add_edges(true);
    std::sort(vec_bedpe_nodes.begin(), vec_bedpe_nodes.end(), BedpeNode::cmp_right_anchor_first);
    add_edges(false);

    for(int i = 0; i < vec_bedpe_nodes.size(); i++) {
        std::sort(edges[i].begin(), edges[i].end());
    }

    /// Clustering
    //std::sort(vec_bedpe_nodes.begin(), vec_bedpe_nodes.end(), BedpeNode::cmp_right_anchor_first);
    clustering(false);
    std::sort(vec_bedpe_nodes.begin(), vec_bedpe_nodes.end(), BedpeNode::cmp_left_anchor_first);
    clustering(true);

    /// Merging results
    merging_loops();
}

std::vector<Bedpe> FreeLoop::get_results() {
    return vec_res;
}
