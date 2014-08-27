/** 
 * @file tests/src/function/Multiplication_test.cpp
 * @date February 19, 2014
 * @author Nikolaos Apostolakos
 */

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <memory>
#include "MathUtils/function/function_tools.h"
#include "MathUtils/function/Polynomial.h"
#include "MathUtils/function/Piecewise.h"
#include "mocks.h"

struct Multiplication_Fixture {
  double close_tolerance {1E-10};
  double small_tolerance {1E-20};
};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (Multiplication_test)

//-----------------------------------------------------------------------------
// Test multiplication between two polynomials
//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(PolynomialWithPolynomial, Multiplication_Fixture) {
  
  // Given
  Euclid::MathUtils::Polynomial p1 {{1., 0.5, -2.}};
  Euclid::MathUtils::Polynomial p2 {{3., 0., 2.}};
  std::vector<double> expectedCoef {3., 1.5, -4., 1., -4.};
  
  // When
  auto multPointer = Euclid::MathUtils::multiply(p1, p2);
  
  // Then
  BOOST_CHECK(multPointer);
  Euclid::MathUtils::Polynomial* polPointer = dynamic_cast<Euclid::MathUtils::Polynomial*>(multPointer.get());
  BOOST_CHECK(polPointer);
  std::vector<double> multCoef = polPointer->getCoefficients();
  BOOST_CHECK_EQUAL_COLLECTIONS(multCoef.begin(), multCoef.end(), expectedCoef.begin(), expectedCoef.end());
}

//-----------------------------------------------------------------------------
// Test multiplication between a piecewise and a generic
//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(PiecewiseWithGeneric, Multiplication_Fixture) {
  
  // Given
  std::vector<double> knots {-1.,0.,1.,2.};
  std::vector<std::shared_ptr<Euclid::MathUtils::Function> > functions {};
  functions.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(1.)));
  functions.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(2.)));
  functions.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(1.)));
  Euclid::MathUtils::Piecewise piecewise{knots, functions};
  IntegrableMock generic {5.};
  
  // When
  auto multPointer = Euclid::MathUtils::multiply(piecewise, generic);
  
  // Then
  BOOST_CHECK(multPointer);
  Euclid::MathUtils::Piecewise* piecePointer = dynamic_cast<Euclid::MathUtils::Piecewise*>(multPointer.get());
  BOOST_CHECK(piecePointer);
  for (double x=-2.;x<= 3.; x+=.1) {
    BOOST_CHECK_CLOSE((*multPointer)(x), piecewise(x)*generic(x), close_tolerance);
  }
}

//-----------------------------------------------------------------------------
// Test multiplication between two piecewises
//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(PiecewiseWithPiecewise, Multiplication_Fixture) {
  
  // Given
  std::vector<double> knots1 {-1.,0.,1.,2.};
  std::vector<std::shared_ptr<Euclid::MathUtils::Function> > functions1 {};
  functions1.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(1.)));
  functions1.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(2.)));
  functions1.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(1.)));
  Euclid::MathUtils::Piecewise p1{knots1, functions1};
  std::vector<double> knots2 {.5,.7,1.,1.5,4.,6};
  std::vector<std::shared_ptr<Euclid::MathUtils::Function> > functions2 {};
  functions2.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(3.)));
  functions2.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(2.)));
  functions2.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(1.)));
  functions2.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(3.)));
  functions2.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(4.)));
  Euclid::MathUtils::Piecewise p2{knots2, functions2};
  
  // When
  auto multPointer = Euclid::MathUtils::multiply(p1, p2);
  
  // Then
  BOOST_CHECK(multPointer);
  Euclid::MathUtils::Piecewise* piecePointer = dynamic_cast<Euclid::MathUtils::Piecewise*>(multPointer.get());
  BOOST_CHECK(piecePointer);
  auto resKnots = piecePointer->getKnots();
  std::vector<double> expectedKnots {.5,.7,1.,1.5,2.};
  BOOST_CHECK_EQUAL_COLLECTIONS(resKnots.begin(), resKnots.end(), expectedKnots.begin(), expectedKnots.end());
  for (double x=-2.;x<= 7.; x+=.1) {
    BOOST_CHECK_CLOSE((*multPointer)(x), p1(x)*p2(x), close_tolerance);
  }
  
  // When
  // Multiplication order should not matter
  multPointer = Euclid::MathUtils::multiply(p2, p1);
  
  // Then
  BOOST_CHECK(multPointer);
  piecePointer = dynamic_cast<Euclid::MathUtils::Piecewise*>(multPointer.get());
  BOOST_CHECK(piecePointer);
  resKnots = piecePointer->getKnots();
  expectedKnots = {.5,.7,1.,1.5,2.};
  BOOST_CHECK_EQUAL_COLLECTIONS(resKnots.begin(), resKnots.end(), expectedKnots.begin(), expectedKnots.end());
  for (double x=-2.;x<= 7.; x+=.1) {
    BOOST_CHECK_CLOSE((*multPointer)(x), p1(x)*p2(x), close_tolerance);
  }
}

//-----------------------------------------------------------------------------
// Test multiplication between two piecewises with different ranges
//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(PiecewiseWithPiecewiseDiffRanges, Multiplication_Fixture) {
  
  // Given
  std::vector<double> knots1 {-1.,0.,1.,2.};
  std::vector<std::shared_ptr<Euclid::MathUtils::Function> > functions1 {};
  functions1.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(1.)));
  functions1.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(2.)));
  functions1.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(1.)));
  Euclid::MathUtils::Piecewise p1{knots1, functions1};
  std::vector<double> knots2 {3.,4.,5.,6.,7.,8.};
  std::vector<std::shared_ptr<Euclid::MathUtils::Function> > functions2 {};
  functions2.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(3.)));
  functions2.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(2.)));
  functions2.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(1.)));
  functions2.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(3.)));
  functions2.push_back(std::shared_ptr<Euclid::MathUtils::Function>(new IntegrableMock(4.)));
  Euclid::MathUtils::Piecewise p2{knots2, functions2};
  
  // When
  auto multPointer = Euclid::MathUtils::multiply(p1, p2);
  
  // Then
  BOOST_CHECK(multPointer);
  Euclid::MathUtils::Polynomial* ppolyPointer = dynamic_cast<Euclid::MathUtils::Polynomial*>(multPointer.get());
  BOOST_CHECK(ppolyPointer);
  auto resCoeff = ppolyPointer->getCoefficients();
  std::vector<double> expectedCoeff {.0};
  BOOST_CHECK_EQUAL_COLLECTIONS(resCoeff.begin(), resCoeff.end(), expectedCoeff.begin(), expectedCoeff.end());
  
  // When
  // Multiplication order should not matter
  multPointer = Euclid::MathUtils::multiply(p2, p1);
  
  // Then
  BOOST_CHECK(multPointer);
  ppolyPointer = dynamic_cast<Euclid::MathUtils::Polynomial*>(multPointer.get());
  BOOST_CHECK(ppolyPointer);
  resCoeff = ppolyPointer->getCoefficients();
  expectedCoeff = {.0};
  BOOST_CHECK_EQUAL_COLLECTIONS(resCoeff.begin(), resCoeff.end(), expectedCoeff.begin(), expectedCoeff.end());
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()