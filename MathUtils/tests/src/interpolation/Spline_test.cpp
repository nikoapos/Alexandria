/*
 * Copyright (C) 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
* @file tests/src/interpolation/Spline_test.cpp
* @date February 20, 2014
* @author Nikolaos Apostolakos
*/

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <cmath>
#include "MathUtils/interpolation/interpolation.h"
#include "MathUtils/function/Piecewise.h"
#include "MathUtils/numericalDifferentiation/FiniteDifference.h"

using namespace Euclid::MathUtils;


struct Spline_Fixture {
  double close_tolerance{1.2E-2};
  double small_tolerance{1E-12};

  std::vector<double> x, y;

  Spline_Fixture() {
    for (double xValue = -10.; xValue <= 10.; xValue += 0.1) {
      x.push_back(xValue);
      y.push_back(sin(xValue));
    }
  }
};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (Spline_test)

//-----------------------------------------------------------------------------
// The interpolated function value at x must match the same value as the original
//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(fx, Spline_Fixture) {
  // When
  auto cubic = interpolate(x, y, InterpolationType::CUBIC_SPLINE);
  std::vector<double> result{};
  for (double xValue : x) {
    result.push_back((*cubic)(xValue));
  }
  double outside1 = (*cubic)(-11.);
  double outside2 = (*cubic)(11.);

  // Then
  for (size_t i = 0; i < y.size(); i++) {
    BOOST_CHECK_CLOSE(result[i], y[i], close_tolerance);
  }
  BOOST_CHECK_SMALL(outside1, small_tolerance);
  BOOST_CHECK_SMALL(outside2, small_tolerance);
}

//-----------------------------------------------------------------------------
// Spline interpolation is piecewise, so check that for each knot, the value of
// the splines to each side match with the original value
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE(Spline_fx, Spline_Fixture) {
  auto cubic_f = interpolate(x, y, InterpolationType::CUBIC_SPLINE);
  auto cubic_pieces = dynamic_cast<Piecewise *>(cubic_f.get());
  auto splines = cubic_pieces->getFunctions();

  for (size_t i = 0; i < splines.size() - 1; ++i) {
    auto x0 = x[i + 1];
    auto left = splines[i];
    auto right = splines[i + 1];
    BOOST_CHECK_CLOSE((*left)(x0), (*right)(x0), close_tolerance);
    BOOST_CHECK_CLOSE((*left)(x0), y[i + 1], close_tolerance);
  }
}

//-----------------------------------------------------------------------------
// Similarly, the first derivative of the splines to each side must match
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE(Spline_dfx, Spline_Fixture) {
  auto cubic_f = interpolate(x, y, InterpolationType::CUBIC_SPLINE);
  auto cubic_pieces = dynamic_cast<Piecewise *>(cubic_f.get());
  auto splines = cubic_pieces->getFunctions();

  for (size_t i = 0; i < splines.size() - 1; ++i) {
    auto x0 = x[i + 1];
    auto left = splines[i];
    auto right = splines[i + 1];

    auto left_dy = derivative(*left, x0);
    auto right_dy = derivative(*right, x0);

    BOOST_CHECK_CLOSE(left_dy, right_dy, close_tolerance);
  }
}

//-----------------------------------------------------------------------------
// Not strictly a requirement for spline interpolation, for some applications
// also the second derivative of the splines to each side must match
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE(Spline_ddfx, Spline_Fixture) {
  const double eps = std::numeric_limits<double>::epsilon();

  auto cubic_f = interpolate(x, y, InterpolationType::CUBIC_SPLINE);
  auto cubic_pieces = dynamic_cast<Piecewise *>(cubic_f.get());
  auto splines = cubic_pieces->getFunctions();

  for (size_t i = 0; i < splines.size() - 1; ++i) {
    auto x0 = x[i + 1];
    auto left = splines[i];
    auto right = splines[i + 1];

    auto left_ddy = derivative2nd(*left, x0);
    auto right_ddy = derivative2nd(*right, x0);

    if (std::abs(left_ddy) <= eps || std::abs(right_ddy) <= eps) {
      BOOST_CHECK_SMALL(left_ddy, small_tolerance);
      BOOST_CHECK_SMALL(right_ddy, small_tolerance);
    }
    else {
      BOOST_CHECK_CLOSE(left_ddy, right_ddy, 2e-2);
    }
  }
}

//-----------------------------------------------------------------------------
// Second derivative at the endpoints should be 0
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE(Spline_ddfx_endpoint, Spline_Fixture) {
  auto cubic_f = interpolate(x, y, InterpolationType::CUBIC_SPLINE);
  auto cubic_pieces = dynamic_cast<Piecewise *>(cubic_f.get());
  auto splines = cubic_pieces->getFunctions();

  auto left = splines.front();
  auto right = splines.back();

  auto left_ddy = derivative2nd(*left, x.front());
  auto right_ddy = derivative2nd(*right, x.back());

  BOOST_CHECK_SMALL(left_ddy, 1e-5);
  BOOST_CHECK_SMALL(right_ddy, 1e-5);
}

//-----------------------------------------------------------------------------
// Same, but with extrapolation
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE(Spline_extrapolation, Spline_Fixture) {
  auto cubic_f = interpolate(x, y, InterpolationType::CUBIC_SPLINE, true);

  auto left_ddy = derivative2nd(*cubic_f, x.front());
  auto right_ddy = derivative2nd(*cubic_f, x.back());

  BOOST_CHECK_SMALL(left_ddy, 1e-5);
  BOOST_CHECK_SMALL(right_ddy, 1e-5);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()
