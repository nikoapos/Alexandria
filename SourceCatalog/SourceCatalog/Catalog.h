/**
 * @file SourceCatalog/Catalog.h
 *
 * @author Nicolas Morisset
 *
 * Created on: Feb 4, 2014
 */

#ifndef CATALOG_H_
#define CATALOG_H_

#include <map>
#include <memory>

#include "ElementsKernel/Export.h"

#include "SourceCatalog/Source.h"

namespace Euclid {
namespace SourceCatalog {

/**
 * @class Catalog
 *
 * @brief
 *  Catalog contains a container of sources
 *
 */
class ELEMENTS_API Catalog
{

public:

  /**
   * @brief
   *  Build a catalog of Source objects
   *
   * @details
   *  Constructs a vector container of Source objects, a map of source
   *  identification and an index which is the location of the Source
   *  object in the vector container
   * @param source_vector
   *  Vector container of Source objects
   * @throw Elements::Exception
   *  A Source object can not be inserted twice in the map
   */
  Catalog(std::vector<Source> source_vector);
  
  typedef std::vector<Source>::const_iterator const_iterator;

  /**
   * @brief Destructor
   */
  virtual ~Catalog() = default;

  /**
   * @brief
   *  Get a const_iterator pointing to the first element in the m_source_vector
   *  vector
   * @return
   *  Returns a const_iterator pointing to the first element in the m_source_vector
   *  container
   */
  const_iterator begin() const { return m_source_vector.cbegin() ; }

  /**
   * @brief
   *  Get an const_iterator pointing to the last element in the m_source_vector
   *  vector
   * @return
   *  Returns a const_iterator pointing to the past-the-end element in the
   *  m_source_vector container
   */
  const_iterator end() const { return m_source_vector.cend() ; }

  /**
   * @brief
   *  Find the Source object from its identification number
   * @param source_id
   * The source identification number
   * @return
   * A shared pointer to the Source object or a null pointer in case of
   * no object was found for this source_id
   */
  std::shared_ptr<Source> find(const int64_t source_id) const;

  /**
   * @brief
   *  Get the size of the vector container
   * @return
   *  The size of the container which is the number of Source objects
   */
  size_t size() const { return m_source_vector.size();}

private:
  // Vector of Source objects
  std::vector<Source>        m_source_vector { };
  // Map of the Source identification and their location
  // in the Source vector
  std::map<int64_t, size_t> m_source_index_map { };

};

} /* namespace SourceCatalog */
} // end of namespace Euclid

#endif /* CATALOG_H_ */
