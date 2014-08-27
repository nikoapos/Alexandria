/** 
 * @file src/lib/interpolation/interpolation.cpp
 * @date February 21, 2014
 * @author Nikolaos Apostolakos
 */

#include "MathUtils/interpolation/interpolation.h"
#include "implementations.h"

namespace Euclid {
namespace MathUtils {

std::unique_ptr<Function> interpolate(const std::vector<double>& x, const std::vector<double>& y, InterpolationType type) {
  switch (type) {
  case InterpolationType::LINEAR:
    return linearInterpolation(x, y);
  case InterpolationType::CUBIC_SPLINE:
    return splineInterpolation(x, y);
  }
  return nullptr;
}

std::unique_ptr<Function> interpolate(const Euclid::XYDataset::XYDataset& dataset, InterpolationType type) {
  std::vector<double> x {};
  std::vector<double> y {};
  for (auto& pair : dataset) {
    x.push_back(pair.first);
    y.push_back(pair.second);
  }
  return interpolate(x, y, type);
}

} // End of MathUtils
} // end of namespace Euclid