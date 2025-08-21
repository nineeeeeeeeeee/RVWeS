#include "palisade/core/lattice/poly.h"
#include "palisade/core/utils/inttypes.h"
#include <iostream>
#include <chrono>

using namespace lbcrypto;

int main() {
    const usint m = 256;
    const usint iterations = 1000;
    NativeInteger modulus("8380417");

    // Create ring parameters
    auto params = std::make_shared<ILParamsImpl<NativeInteger>>(m, modulus);

    // Create total time counters
    uint64_t total_add_time_ns = 0;
    uint64_t total_mul_time_ns = 0;

    for (usint iter = 0; iter < iterations; ++iter) {
        // Create polynomials per iteration
        PolyImpl<NativeVector> poly1(params, Format::EVALUATION, true);
        PolyImpl<NativeVector> poly2(params, Format::EVALUATION, true);

        // Fill with random coefficients
        for (usint i = 0; i < m/2; ++i) {
            poly1[i] = NativeInteger(rand() % modulus.ConvertToInt());
            poly2[i] = NativeInteger(rand() % modulus.ConvertToInt());
        }

        // Benchmark addition
        auto start_add = std::chrono::high_resolution_clock::now();
        auto sum = poly1 + poly2;
        auto end_add = std::chrono::high_resolution_clock::now();
        total_add_time_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(end_add - start_add).count();

        // Benchmark multiplication
        auto start_mul = std::chrono::high_resolution_clock::now();
        auto product = poly1 * poly2;
        auto end_mul = std::chrono::high_resolution_clock::now();
        total_mul_time_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(end_mul - start_mul).count();
    }

    // Output total and average time
    std::cout << "Total addition time over " << iterations << " iterations: " << total_add_time_ns << " ns\n";
    std::cout << "Average addition time: " << (total_add_time_ns / iterations) << " ns\n\n";

    std::cout << "Total multiplication time over " << iterations << " iterations: " << total_mul_time_ns << " ns\n";
    std::cout << "Average multiplication time: " << (total_mul_time_ns / iterations) << " ns\n";

    return 0;
}
