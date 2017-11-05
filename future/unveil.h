/*
 * unveil.h
 * This file is part of graphSampler
 *
 * Copyright (C) 2017 - Giacomo Bergami
 * Created by Giacomo Bergami on 04/04/17.
 *
 * graphSampler is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * graphSampler is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with graphSampler. If not, see <http://www.gnu.org/licenses/>.
 */



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
