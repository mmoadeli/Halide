#ifndef HALIDE_QUALIFY_H
#define HALIDE_QUALIFY_H

/** \file
 *
 * Defines methods for prefixing names in an expression with a prefix string.
 */
#include <string>

#include "Expr.h"
#include "Util.h"  // for HALIDE_EXPORT_FOR_PLUGINS

namespace Halide {
namespace Internal {

/** Prefix all variable names in the given expression with the prefix string. */
HALIDE_EXPORT_FOR_PLUGINS Expr qualify(const std::string &prefix, const Expr &value);

}  // namespace Internal
}  // namespace Halide

#endif
