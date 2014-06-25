/**
 * @file XYDataset/XYDatasetProvider.h
 *
 * @date Apr 10, 2014
 * @author Nicolas Morisset
 */

#ifndef XYDATASETPROVIDER_H_
#define XYDATASETPROVIDER_H_

#include <memory>
#include <vector>
#include <string>
#include "XYDataset/XYDataset.h"
#include "XYDataset/QualifiedName.h"

namespace XYDataset {

/**
 * @class XYDatasetProvider
 * Interface class
 * @brief
 * This interface class provides the dataset following a qualified name object
 * @details
 * This class consists of two virtual functions, listContents and getDataset.
 * The listContents function lists all files contents in the "group" path and the
 * getDatatset function gets from a qualified name the dataset of a XYDataset type.
 */

class XYDatasetProvider
{
 public:
  /**
   * @brief
   * Virtual function to list all files contents in the "group" path
   * @details
   * let's take the following example. if you have a group sets to "A/B/C" and
   * under the "C" repository there is the following structure :
   * C/file1
   * C/file2
   * C/D/file3
   * etc...
   * then the vector of strings returned will contain the following elements:
   * vector[0] = "A/B/C/file1"
   * vector[1] = "A/B/C/file2"
   * vector[3] = "A/B/C/D/file3"
   * etc...
   * Note: The empty string for the group means the root group
   * @param group
   * Name of the dataset group
   * @return
   * A vector of the qualified names of all datasets inside the given group (recursively)
   */
   virtual std::vector<QualifiedName> listContents(const std::string& group) = 0;

  /**
   * @brief
   * Virtual function for getting from a qualified name the dataset of
   * XYDataset type
   * @param qualified_name
   * Qualified name of the dataset
   * @return
   * A unique pointer of XYDataset type to the dataset
   */
   virtual std::unique_ptr<XYDataset> getDataset(const QualifiedName& qualified_name) = 0;

   virtual ~XYDatasetProvider() = default;

 private:

};

} /* namespace XYDataset */

#endif // XYDATASETPROVIDER_H_ 
