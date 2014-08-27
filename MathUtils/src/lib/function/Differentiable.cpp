/** 
 * @file src/lib/function/Differentiable.cpp
 * @date February 18, 2014
 * @author Nikolaos Apostolakos
 */

#include "MathUtils/function/Differentiable.h"

namespace Euclid {
namespace MathUtils {

double Differentiable::integrate(const double x1, const double x2) const {
  std::shared_ptr<Function> antiderivative {this->indefiniteIntegral()};
  return (*antiderivative)(x2) - (*antiderivative)(x1);
}

} // end of MathUtils
} // end of namespace Euclid