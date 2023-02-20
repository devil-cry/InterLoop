#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include <argparse/argparse.hpp>

#include "bedpe/bedpe.h"
#include "share/global_def.h"
#include "logger/logger.h"
#include "io/io.h"
#include "view/view.h"
#include "solver/freeloop.h"


int Bedpe::psm_bedpe_valid_elements = 6;

using namespace std;
using namespace global_def;

void set_argument_parser(argparse::ArgumentParser& parser)
{
    parser.add_argument("-f", "--file")
            .help("filename of valid pairs in '.bedpe' format.")
            .required();

    parser.add_argument("-o", "--output")
            .help("prefix of output loop filename.")
            .required();

    parser.add_argument("-t", "--thread")
            .help(string("threads used to run ") + PROGRAM_NAME + ".")
            .default_value(1);

    parser.add_argument("-ws", "--window_size")
            .help(string("anchors that are within the sliding window size from one bedpe to another "
                         "will be considered relevant."))
            .default_value(10000);

    parser.add_argument("-inc_chr", "--include_chromes")
            .help(string("only process the anchor whose one of the end is contained on the specified "
                         "chromatin, used ',' to split."))
            .default_value(string ("all"));

    parser.add_argument("-inc_pair", "--include_pairs")
            .help(string("only process the anchor whose both ends are contained on the specified chromatin,"
                         " each pair in 'chrA-chrB' format, and used ',' to split."))
            .default_value(string ("all"));

    parser.add_argument("-lo", "--log_output")
            .help("log file name")
            .default_value(string(PROGRAM_NAME) + ".log");

    parser.add_argument("-lv", "--log_level")
            .help("level of logger, select in {ERROR,WARN,INFO,DEBUG}.")
            .default_value(string("INFO"));
}

bool check_argv(argparse::ArgumentParser& parser)
{
    /// TODO
    return true;
}

condition_variable cv;
vector<mutex> vec_mtx;
atomic<int> atomic_running_thr_cnt(0);
void pipeline(const string& group, const vector<Bedpe>& vec_bedpe, const string& out_dir, int64_t ws, Logger& logger)
{
    for(auto &mtx_it : vec_mtx) {
        unique_lock<mutex> lk(mtx_it);
        cv.wait(lk, [] {
            return atomic_running_thr_cnt < vec_mtx.size();
        });
        break;
    }
    atomic_running_thr_cnt++;

    try {
        logger.INFO("Start thread : %s .", group.c_str());

//        View::show_golden_in_valid_bedpe("../res/dataset_2_gold.bedpe", group, vec_bedpe, logger);

        FreeLoop freeloop;
        freeloop.set_vec_bedpe(vec_bedpe);
        freeloop.set_group(group);
        freeloop.set_allow_dis_max(ws);

        freeloop.preprocess();
        freeloop.run();

        vector<Bedpe> vec_candidate_loop = freeloop.get_results();
//        string outfile = group + ".loop";
        string outfile = out_dir + "/" + group + ".loop";
        IO::output_loop_file(outfile, vec_candidate_loop, logger);

        logger.INFO("Finished thread : %s , found %d inter-loops and saved in file '%s'.",
                    group.c_str(), vec_candidate_loop.size(), outfile.c_str());
    } catch (const exception &err) {
        cerr << err.what() << "\n";
        logger.ERROR("Found something wrong in group '%s'.", group.c_str());
    }

    atomic_running_thr_cnt--;
    cv.notify_one();
}

int main(int argc, char** argv)
{
    argparse::ArgumentParser program(PROGRAM_NAME, PROGRAM_VERSION);
    set_argument_parser(program);

    try {
        program.parse_args(argc, argv);
    } catch (const runtime_error& err) {
        cerr << err.what() << "\n";
        cerr << program;
        exit(1);
    } catch (const exception &err) {
        cerr << err.what() << "\n";
        cerr << program;
        exit(1);
    }

    auto logger_file = program.get("--log_output");
    auto logger_level = program.get("--log_level");
    Logger logger(logger_file, logger_level);

    if (!check_argv(program)) {
        logger.ERROR("Arguments analysed failed.");
        exit(1);
    }
    logger.INFO("Argument analysed successful.");

    auto st_time = time(nullptr);

    set<string> set_inc_chromes, set_inc_pairs;
    set_inc_pairs = IO::get_include_pairs(program.get("--include_pairs"), logger);

    string out_dir = program.get("--output");
    IO::mk_dir(out_dir);

    map<string, vector<Bedpe>> mp_bedpe;
    IO::read_bedpe_file(program.get("--file"), set_inc_pairs, mp_bedpe, logger);

    auto ws = atoll(program.get("--window_size").c_str());

    auto thread_limit = atoi(program.get("--thread").c_str());
    vec_mtx = vector<mutex> (thread_limit);

#ifdef _SHOW_INTERACTION
    auto func = [&] (const string& group) {
        const int width = 100;
        const int cnt_per_ch = 5;

        logger.DEBUG("For pair %s :", group.c_str());
        vector<Bedpe>& target = mp_bedpe[group];
        long long int min_idx = numeric_limits<long long int>::max();
        long long int max_idx = numeric_limits<long long int>::min();
        for (auto& bedpe : target) {
            min_idx = min({min_idx, bedpe.st_R, bedpe.en_R});
            max_idx = max({max_idx, bedpe.st_R, bedpe.en_R});
        }
        logger.DEBUG("min value : %lld, max value : %lld", min_idx, max_idx);

        vector<int> idx_cnt(width + 1, 0);
        long long int base = max_idx / width;
        for (auto& bedpe : target) {
            long long int pos = (bedpe.st_R + bedpe.en_R) >> 1;
            idx_cnt[pos / base] ++;
        }

        for (int i = 0; i < idx_cnt.size(); i++) {
            int ch_cnt = idx_cnt[i] / cnt_per_ch;
            string s;
            while (ch_cnt--) {
                s += "*";
            }
            logger.DEBUG("id %-4d : %s", i, s.c_str());
        }
    };

    for (auto &pair : set_inc_pairs) {
        func(pair);
    }
#endif

    vector<thread*> vec_thr_p;

    for (const auto &[group, vec_bedpe] : mp_bedpe) {
        auto* thr = new thread(pipeline, std::cref(group), std::cref(vec_bedpe), std::cref(out_dir), ws, std::ref(logger));
        vec_thr_p.emplace_back(thr);
    }
    for (auto &it : vec_thr_p) {
        it->join();
    }

    auto end_time = time(nullptr);

    logger.INFO("All processes completed, used %d seconds of real time.", end_time - st_time);

    return 0;
}

