#include "palisade/core/lattice/backend.h"
#include "palisade/core/math/backend.h"
#include "palisade/core/math/distrgen.h"
#include "palisade/core/math/nbtheory.h"
#include "palisade/core/utils/inttypes.h"
#include "palisade/core/utils/utilities.h"
//#include "gtest/gtest.h"

#include "palisade/core/lattice/trapdoor.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>

// using namespace std;
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


void GaussSampTest(){ 
DEBUG_FLAG(false);
  DEBUG("in test");
  usint m = 16;
  usint n = m / 2;

  BigInteger modulus("67108913");
  BigInteger rootOfUnity("61564");
  double sigma = SIGMA;

  double val = modulus.ConvertToDouble();  // TODO get the next few lines
                                           // working in a single instance.
  double logTwo = log(val - 1.0) / log(2) + 1.0;
  usint k = (usint)floor(logTwo);  // = this->m_cryptoParameters.GetModulus();

  DEBUG("k = " << k);
  DEBUG("sigma = " << sigma);
  DEBUG("m = " << m);
  DEBUG("modulus = " << modulus);
  DEBUG("root = " << rootOfUnity);

  auto params = std::make_shared<ILParams>(m, modulus, rootOfUnity);

  std::pair<Matrix<Poly>, RLWETrapdoorPair<Poly>> trapPair =
      RLWETrapdoorUtility<Poly>::TrapdoorGen(params, sigma);

  Matrix<Poly> eHat = trapPair.second.m_e;
  Matrix<Poly> rHat = trapPair.second.m_r;
  // auto uniform_alloc = Poly::MakeDiscreteUniformAllocator(params,
  // Format::EVALUATION);

  Poly::DggType dgg(sigma);
  Poly::DugType dug = Poly::DugType();
  dug.SetModulus(modulus);

  uint32_t base = 2;
  double c = (base + 1) * SIGMA;
  double s = SPECTRAL_BOUND(n, k, base);
  Poly::DggType dggLargeSigma(sqrt(s * s - c * c));

  Poly u(dug, params, Format::COEFFICIENT);

  DEBUG("u " << u);
  u.SwitchFormat();
  DEBUG("u " << u);

  Matrix<Poly> z = RLWETrapdoorUtility<Poly>::GaussSamp(
      n, k, trapPair.first, trapPair.second, u, dgg, dggLargeSigma);


  Poly uEst = (trapPair.first * z)(0, 0);

  DEBUG("uEst " << uEst);
  DEBUG("u " << u);

  DEBUG("uEst.GetModulus() " << uEst.GetModulus());
  DEBUG("u.GetModulus() " << u.GetModulus());

  uEst.SwitchFormat();
  u.SwitchFormat();

}

void GaussSampTest2(){ 
  usint n = 128;  // cyclotomic order
  size_t kRes = 23;
  size_t base = 8;

  size_t size = 4;

  double sigma = SIGMA;

  std::vector<NativeInteger> moduli;
  std::vector<NativeInteger> roots_Of_Unity;

  NativeInteger q = NativeInteger(1) << (kRes - 1);
  q = lbcrypto::FirstPrime<NativeInteger>(kRes, 2 * n);
  //std::cout<< "q = " << q << std::endl;
  NativeInteger rootOfUnity(RootOfUnity<NativeInteger>(2 * n, q));
  moduli.push_back(q);
  roots_Of_Unity.push_back(rootOfUnity);

  NativeInteger nextQ = q;
  for (size_t i = 1; i < size; i++) {
    nextQ = lbcrypto::NextPrime<NativeInteger>(nextQ, 2 * n);
    NativeInteger nextRootOfUnity(RootOfUnity<NativeInteger>(2 * n, nextQ));
    moduli.push_back(nextQ);
    roots_Of_Unity.push_back(nextRootOfUnity);
  }

  auto params =
      std::make_shared<ILDCRTParams<BigInteger>>(2 * n, moduli, roots_Of_Unity);

  int64_t digitCount = (long)ceil(log2(q.ConvertToDouble()) / log2(base));

  std::pair<Matrix<DCRTPoly>, RLWETrapdoorPair<DCRTPoly>> trapPair =
      RLWETrapdoorUtility<DCRTPoly>::TrapdoorGen(params, sigma, base);

  Matrix<DCRTPoly> eHat = trapPair.second.m_e;
  Matrix<DCRTPoly> rHat = trapPair.second.m_r;

  DCRTPoly::DggType dgg(sigma);
  DCRTPoly::DugType dug = DCRTPoly::DugType();
  DCRTPoly u(dug, params, Format::COEFFICIENT);

  usint k = moduli.size() * digitCount;

  double c = (base + 1) * SIGMA;
  double s = SPECTRAL_BOUND(n, k, base);
  DCRTPoly::DggType dggLargeSigma(sqrt(s * s - c * c));

  u.SwitchFormat();

  const int iterations = 1000;
  std::vector<long long> gauss_samp_timings;
  for(int ttt=0; ttt<iterations; ttt++){
    auto tn=n, tk=k;
    auto ttrapPair=trapPair;
    auto tu=u;
    auto tdgg=dgg;
    auto tdggLargeSigma=dggLargeSigma;
    auto tbase=base;

    auto start = std::chrono::high_resolution_clock::now();
    Matrix<DCRTPoly> z = RLWETrapdoorUtility<DCRTPoly>::GaussSamp(
      tn, tk, ttrapPair.first, ttrapPair.second, tu, tdgg, tdggLargeSigma, tbase);
    auto end = std::chrono::high_resolution_clock::now();
    gauss_samp_timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
  }
  print_results("RLWETrapdoorUtility::GaussSamp", gauss_samp_timings, iterations);
}

int main(){
  GaussSampTest();
  //GaussSampTest2();
  return 0;
}