#include "palisade.h"
#include "palisade/core/math/matrix.h"
#include "palisade/core/math/discretegaussiangenerator.h"
#include "palisade/core/lattice/trapdoor.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>

using namespace lbcrypto;


void print_results(const std::string& function_name, const std::vector<long long>& timings_ns, int iterations) {
    if (timings_ns.empty()) {
        std::cout << "No timings recorded for " << function_name << std::endl;
        return;
    }

    long long total_duration = std::accumulate(timings_ns.begin(), timings_ns.end(), 0LL);
    long long average_duration = total_duration / timings_ns.size();

    std::cout << "==================================================" << std::endl;
    std::cout << "Performance Test Results for: " << function_name << std::endl;
    std::cout << "Parameters: Scheme = BFVrns, DCRTPoly, N auto-chosen" << std::endl;
    std::cout << "Total iterations: " << iterations << std::endl;
    std::cout << "Average time: " << average_duration << " ns" << std::endl;
    std::cout << "Average time: " << static_cast<double>(average_duration) / 1e3 << " us" << std::endl;
    std::cout << "Average time: " << static_cast<double>(average_duration) / 1e6 << " ms" << std::endl;
    std::cout << "==================================================" << std::endl;
}

void TrapdoorGenTest(){
    using Element = DCRTPoly;
    using ParmType = typename Element::Params;
    using DggType = DiscreteGaussianGeneratorImpl<NativeVector>;

    CryptoContext<DCRTPoly> cc = CryptoContextFactory<DCRTPoly>::genCryptoContextBFVrns(
        23, HEStd_128_classic, 3.2, 
        0, 1, 0,
        OPTIMIZED, 8, 0, 40, 0
    );

    cc->Enable(ENCRYPTION);
    cc->Enable(SHE);

    auto params = cc->GetCryptoParameters()->GetElementParams();

    const int iterations = 1000;
    std::vector<long long> trapdoor_samp_timings;

    // test TrapdoorGen
    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        auto [A, T] = RLWETrapdoorUtility<Element>::TrapdoorGen(params, 3.2, 2);
        auto end = std::chrono::high_resolution_clock::now();
        trapdoor_samp_timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }
    print_results("RLWETrapdoorUtility::TrapdoorGen", trapdoor_samp_timings, iterations);
    
}

int main() {
    TrapdoorGenTest();

    return 0;
}

