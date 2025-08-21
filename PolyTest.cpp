#include "palisade/core/lattice/poly.h"
#include "palisade/core/utils/inttypes.h"
#include "palisade/core/utils/exception.h"
#include "palisade/core/lattice/ilparams.h"


using namespace lbcrypto;

int main() {
    try {
        // (n=256, q=8380417)
        auto params = std::make_shared<ILParamsImpl<BigInteger>>(256, 8380417);
        
        PolyImpl<BigVector> poly1(params, Format::EVALUATION, true);
        // poly1[0] = 1;  // x^0 
        // poly1[1] = 2;  // x^1 

        PolyImpl<BigVector> poly2(params, Format::EVALUATION, true);
        // poly2[0] = 3;  // x^0 
        // poly2[1] = 4;  // x^1 

        PolyImpl<BigVector> sum = poly1 + poly2;
        
        PolyImpl<BigVector> product = poly1 * poly2;

        std::cout << "Sum EVALUATION: ";
        for (size_t i = 0; i < sum.GetLength(); ++i) {
            std::cout << sum[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "Product EVALUATION: ";
        for (size_t i = 0; i < product.GetLength(); ++i) {
            std::cout << product[i] << " ";
        }
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}