/*
 * Copyright (C) 2012-2021 Euclid Science Ground Segment
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
 * @file tests/src/Catalog_test.cpp
 *
 *  Created on: March 1, 2013
 *      Author: Pierre Dubath
 */

#include "SourceCatalog/Catalog.h"
#include "SourceCatalog/Source.h"
#include <boost/test/unit_test.hpp>

#include "ElementsKernel/Exception.h"

#include <iostream>
#include <map>

//-----------------------------------------------------------------------------
// Include the CatalogFixture which include a photometry mock object we use here for the test
#include "tests/src/CatalogFixture.h"

using namespace Euclid::SourceCatalog;

//-----------------------------------------------------------------------------
//

BOOST_AUTO_TEST_SUITE(Catalog_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(size_test, CatalogFixture) {

  BOOST_TEST_MESSAGE("--> size test ");
  BOOST_CHECK_EQUAL(source_vector.size(), catalog.size());
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(find_test, CatalogFixture) {

  BOOST_TEST_MESSAGE("--> find test ");

  std::shared_ptr<Source>      a_source(catalog.find(expected_source_id_2));
  std::shared_ptr<Coordinates> coordinates(a_source->getAttribute<Coordinates>());

  BOOST_CHECK_EQUAL(expected_ra_2, coordinates->getRa());
  BOOST_CHECK_EQUAL(expected_dec_2, coordinates->getDec());
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(find_missig_source_test, CatalogFixture) {

  BOOST_TEST_MESSAGE("--> find test ");

  std::shared_ptr<Source> pSource(catalog.find(999999));

  BOOST_CHECK(nullptr == pSource);
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(identical_sources_test, CatalogFixture) {

  BOOST_TEST_MESSAGE("--> identical_sources test ");

  std::vector<Source> source_vector_identical{};

  source_vector_identical.push_back(Source(expected_source_id_1, attribute_vector_1));
  source_vector_identical.push_back(Source(expected_source_id_2, attribute_vector_2));
  source_vector_identical.push_back(Source(expected_source_id_1, attribute_vector_1));

  bool identical = false;

  try {
    Catalog catalog_test_value{source_vector_identical};
  } catch (Elements::Exception& e) {
    identical = true;
  }

  BOOST_CHECK_EQUAL(identical, true);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
