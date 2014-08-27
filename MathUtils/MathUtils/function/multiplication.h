/**
 * @file MathUtils/function/multiplication.h
 * @date February 19, 2014
 * @author Nikolaos Apostolakos
 */

#ifndef MATHUTILS_MULTIPLICATION_H
#define	MATHUTILS_MULTIPLICATION_H

#include <map>
#include <utility>
#include <typeindex>

#include "ElementsKernel/Export.h"

namespace Euclid {
namespace MathUtils {

/// Alias of a function which multiplies Function objects
typedef std::unique_ptr<Function> (*MultiplyFunction)(const Function&, const Function&);

/**
 * A map for retrieving specific function multiplication implementations. The
 * keys of the map are the pairs of the Function types and the value of the
 * map is the function which can be used for performing this multiplication in
 * an efficient way.
 */
ELEMENTS_API extern std::map<std::pair<std::type_index,std::type_index>, MultiplyFunction> multiplySpecificSpecificMap;

/**
 * A map for retrieving specific function multiplication implementations. The
 * keys of the map are the type of a Function which can be multiplied with any
 * other function and the value of the map is the function which can be used for
 * performing this multiplication in an efficient way.
 */
ELEMENTS_API extern std::map<std::type_index, MultiplyFunction> multiplySpecificGenericMap;

} // End of MathUtils
} // end of namespace Euclid

#endif	/* MATHUTILS_MULTIPLICATION_H */

