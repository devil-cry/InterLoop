//
// Created by devilcry on 2023/1/24.
//

#include "argparse/argparse.hpp"
#include "bedpe/bedpe.h"
#include "logger/logger.h"
#include "share/global_def.h"
#include "io/io.h"

#include <iostream>
#include <set>
#include <map>
using namespace std;

int Bedpe::psm_bedpe_valid_elements = 6;

void set_argument_parser(argparse::ArgumentParser& parser)
{
    parser.add_argument("-gf", "--golden_file")
        .help("filename of golden standard loops in '.bedpe' format.")
        .required();

    parser.add_argument("-tf", "--test_file")
        .help("filename of loops to test accuracy in '.bedpe' format.")
        .required();

    parser.add_argument("-d", "--dis")
        .help("anchor distance between two anchor considered as positive sample.(negative value means overlap"
              " length)")
        .default_value(100000)
        .scan<'i',int>();

    parser.add_argument("-s", "--silent")
        .help("show title in test results.")
        .default_value(false)
        .implicit_value(true);

    parser.add_argument("-sm", "--single_match")
        .help("use single anchor matched model.(default: pair match model)")
        .default_value(false)
        .implicit_value(true);
}

bool check_if_belong_in_same_group(map<string, vector<Bedpe>>& mp1, map<string, vector<Bedpe>>& mp2)
{
    if(mp1.size() != 1 || mp2.size() != 1)
        return false;
    for(auto &[key, value] : mp1){
        return mp2.count(key);
    }
    return false;   //impossible line
}


int main(int argc, char** argv)
{
    argparse::ArgumentParser program("test_acc");

    set_argument_parser(program);

    try {
        program.parse_args(argc, argv);
    } catch (const runtime_error& err) {
        cerr << err.what() << endl;
        cerr << program;
        exit(1);
    } catch (const exception& err) {
        cerr << err.what() << endl;
        cerr << program;
        exit(1);
    }

    Logger logger("test_acc.log", "INFO");

    auto golden_file = program.get<string>("-gf");
    auto test_file = program.get<string>("-tf");

    // const std::string& file, const std::set<std::string>& inc_pairs,
    //                                std::map<std::string, std::vector<Bedpe>>& mp, Logger& logger
    set<string> inc_pairs;
    map<string, vector<Bedpe>> mp_gold, mp_test;
    IO::read_bedpe_file(golden_file, inc_pairs, mp_gold, logger);
    IO::read_bedpe_file(test_file, inc_pairs, mp_test, logger);

    if(!check_if_belong_in_same_group(mp_gold, mp_test)) {
        logger.ERROR("golden file and test file must only have the same group(eg:chr1-chr2) !");
        exit(1);
    }

    /**
     * calculate REC
     */
    auto is_silent = program.is_used("-s");
    if(!is_silent){
        string title = "GroupName \t GoldenCount \t TestCount \t GoldenTN \t REC";
        logger.INFO(title.c_str());
    }

    auto& groupName = (*mp_test.begin()).first;

    auto& vec_golden = (*mp_gold.begin()).second;
    auto& vec_test = (*mp_test.begin()).second;

    auto is_single_match_mode = program.is_used("-sm");

    auto dis = program.get<int>("-d");

    auto check_hit = [&](const Bedpe& g, const Bedpe& t) {
        int match_anchor_cnt = 0, final_match_cnt = 0;
        int64_t len;

        // anchor L
        len = (g.st_L < t.st_L ? t.st_L - g.en_L : g.st_L - t.en_L);
        match_anchor_cnt += (len <= dis ? 1 : 0);
        // anchor R
        len = (g.st_R < t.st_R ? t.st_R - g.en_R : g.st_R - t.en_R);
        match_anchor_cnt += (len <= dis ? 1 : 0);
        final_match_cnt = max(final_match_cnt, match_anchor_cnt);

        // two anchors cross connection situation
        match_anchor_cnt = 0;
        len = (g.st_L < t.st_R ? t.st_R - g.en_L : g.st_L - t.en_R);
        match_anchor_cnt += (len <= dis ? 1 : 0);
        len = (g.st_R < t.st_L ? t.st_L - g.en_R : g.st_R - t.en_L);
        match_anchor_cnt += (len <= dis ? 1 : 0);
        final_match_cnt = max(final_match_cnt, match_anchor_cnt);

        if(final_match_cnt == 0)
            return false;
        return is_single_match_mode || final_match_cnt == 2 ;

    };

    vector<bool> vis(vec_golden.size());
    for(auto& bedpe_t : vec_test) {
        for(int i=0; i < vec_golden.size(); i++){
            if(check_hit(vec_golden[i], bedpe_t))
                vis[i] = true;
        }
    }
    int goldenTN = count(vis.begin(), vis.end(), true);
    double rec = 100.0 * goldenTN / vec_golden.size();

    string str_out = groupName + "\t" + to_string(vec_golden.size()) + "\t" + to_string(vec_test.size()) + "\t" +
            to_string(goldenTN) + "\t" + to_string(rec) + "%%";

    logger.INFO(str_out.c_str());

    return 0;
}
