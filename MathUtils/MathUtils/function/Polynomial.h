/**
 * @file MathUtils/function/Polynomial.h
 * @date February 19, 2014
 * @author Nikolaos Apostolakos
 */

#ifndef MATHUTILS_POLYNOMIAL_H
#define	MATHUTILS_POLYNOMIAL_H

#include <vector>

#include "ElementsKernel/Export.h"

#include "MathUtils/function/Function.h"
#include "MathUtils/function/Differentiable.h"

namespace Euclid {
namespace MathUtils {

/**
 * @class Polynomial
 *
 * @brief Represents a polynomial function
 */
class ELEMENTS_API Polynomial : public Differentiable {

public:

  /**
   * Constructs a new Polynomial function with the given coefficients. The
   * index of the coefficients in the given vector corresponds to the degree of
   * the coefficient.
   *
   * @param coefficients the polynomial coefficients
   */
  Polynomial(std::vector<double> coefficients);

  /// Default destructor
  virtual ~Polynomial() = default;

  /// Returns the coefficients of the polynomial
  const std::vector<double>& getCoefficients() const;

  /// Calculates the value of the polynomial for the given value
  double operator()(const double) const override;

  /// Creates a new polynomial with the same coefficients
  std::unique_ptr<Function> clone() const override;

  /// Returns the derivative of the polynomial
  std::shared_ptr<Function> derivative() const override;

  /// Returns the indefinite integral of the polynomial
  std::shared_ptr<Function> indefiniteIntegral() const override;

private:

  /// The vector where the polynomial coefficients are stored
  std::vector<double> m_coef;
  /// The function representing the derivative (uses lazy initialization)
  mutable std::shared_ptr<Function> m_derivative {};
  /// The function representing the indefinite integral (uses lazy initialization)
  mutable std::shared_ptr<Function> m_indefIntegral {};

}; // End of Polynomial

} // End of MathUtils
} // end of namespace Euclid

#endif	/* MATHUTILS_POLYNOMIAL_H */

