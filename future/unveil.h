/**
 * Created by vasistas on 04/04/17.
 */


//
// Created by Giacomo Bergami on 04/04/17.
//

#ifndef GRAPHSAMPLER_UNVEIL_H
#define GRAPHSAMPLER_UNVEIL_H

#if (defined __cplusplus) && (__cplusplus >= 201700L)
#include <optional>
#else
#include "forward/optional.h"
#endif

#if (defined __cplusplus) && (__cplusplus >= 201700L)
using std::optional;
#else
using std::experimental::optional;
#endif

#endif //GRAPHSAMPLER_UNVEIL_H
