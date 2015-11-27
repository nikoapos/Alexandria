/**
 * @file MathUtils/numericalIntegration/AdaptativeIntegration.h
 * @date July 2, 2015
 * @author Florian Dubath
 */

#ifndef MATHUTILS_MATHUTILS_NUMERCALINTEGRATION_ADAPTATIVEINTEGRATION_H_
#define MATHUTILS_MATHUTILS_NUMERCALINTEGRATION_ADAPTATIVEINTEGRATION_H_
#include <cmath>
#include "ElementsKernel/Real.h"
#include "MathUtils/function/Function.h"
#include "MathUtils/function/function_tools.h"

namespace Euclid {
namespace MathUtils {

/**
 * @class AdaptativeIntegration
 *
 * @brief Class implementing the NumericalIntegrationScheme interface.
 *
 * @details
 * AdaptativeIntegration is sampling the interval in 2^order points. It
 * apply a quadrature to get an approximation of the integral.
 * Then it double the sampling and recompute the integral computation and loop,
 * increasing the order.
 * The process stops when the relative difference between one approximation and
 * the next is smaller than the prescription.
 *
 * @tparam quadrature The numerical quadrature used to compute the integral
 * approximation.
 */
template<typename Quadrature>
class AdaptativeIntegration: public NumericalIntegrationScheme {

public:
  /**
   * @brief Constructor.
   *
   * @param relative_precision A double representing the maximal relative
   * difference between an iteration and the next in order the
   * computation to stop.
   *
   * @param initial_order an integer giving the number of sampling,
   * computed as 2^order, for the first integral approximation. Note that the
   * quadrature may require a minimal order to work.
   */
  AdaptativeIntegration(double relative_precision, int initial_order);

  /**
   * @brief Functional call.
   *
   * @details
   * Compute the integral using the quadrature and increasing the order until
   * two successive iteration have relative difference smaller than the prescription.
   *
   * @param function the Function to integrate.
   * @param min The minimum range of the integration.
   * @param max The maximum range of the integration.
   */
  double operator()(const Function& function, double min, double max) override;

private:
  Quadrature m_quadrature { };
  double m_relative_precion;
  int m_initial_order;
};

}
}

#include "MathUtils/numericalIntegration/_impl/AdaptativeIntegration.icpp"

#endif /* MATHUTILS_MATHUTILS_NUMERCALINTEGRATION_ADAPTATIVEINTEGRATION_H_ */