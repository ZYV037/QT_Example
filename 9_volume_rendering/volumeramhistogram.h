/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2019 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#ifndef IVW_VOLUMERAMHISTOGRAM_H
#define IVW_VOLUMERAMHISTOGRAM_H

#include "histogram.h"
#include "rangecolordefine.h"
#include <cmath>
#include "indexmapper.h"


template<typename genType>
genType getMin(genType x, genType y)
{
    //GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559 || std::numeric_limits<genType>::is_integer, "'min' only accept floating-point or integer inputs");
    return (y < x) ? y : x;
}

template<typename genType>
genType getMax(genType x, genType y)
{
    //GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559 || std::numeric_limits<genType>::is_integer, "'max' only accept floating-point or integer inputs");

    return (x < y) ? y : x;
}

template <typename T>
struct rank : public std::rank<T> {};

template <typename T>
struct rank<const T> : public rank<T> {};

template <typename T, typename std::enable_if<rank<T>::value == 0, int>::type = 0>
constexpr auto glmcomp(T& elem, size_t) -> T& {
    return elem;
}
template <typename T, typename std::enable_if<rank<T>::value == 1, int>::type = 0>
constexpr auto glmcomp(T& elem, size_t i) ->
    typename std::conditional<std::is_const<T>::value, const typename T::value_type&,
                              typename T::value_type&>::type {
    return elem[i];
}
//#include <inviwo/core/util/indexmapper.h>
template <typename T>
HistogramContainer calculateVolumeHistogram(const T* data, size3_t dimensions, doubleVec2 dataRange,
                                            const bool& stop = false, size_t bins = 2048,
                                            size3_t sampleRate=size3_t(0,0,0))
{
    // a double type with the same extent as T
    typedef double D;
    // a size_t type with same extent as T
    typedef size_t I;

    const size_t extent =  1;

    // check whether number of bins exceeds the data range only if it is an integral type
    if (!std::is_floating_point<T>::value)
    {
        bins = std::min(bins, static_cast<std::size_t>(dataRange.y - dataRange.x + 1));
    }

    HistogramContainer histograms;
    for (size_t i = 0; i < extent; ++i)
    {
        histograms.add(new NormalizedHistogram(bins));
    }

    D min(std::numeric_limits<double>::max());
    D max(std::numeric_limits<double>::lowest());
    D sum(0);
    D sum2(0);
    double count(0);

    D val;
    I ind;
    size_t v;

    const D rangeMin(dataRange.x);
    const D rangeScaleFactor(static_cast<double>(bins - 1) / (dataRange.y - dataRange.x));

    IndexMapper3D mapper(dimensions);

    size3_t pos(0,0,0);
    // Column major data, so x is the fastest index.
    for (pos.z = 0; pos.z < dimensions.z; pos.z += sampleRate.z) {
        for (pos.y = 0; pos.y < dimensions.y; pos.y += sampleRate.y) {
            for (pos.x = 0; pos.x < dimensions.x; pos.x += sampleRate.x) {
                if (stop) return histograms;

                val = static_cast<D>(data[mapper(pos)]);

                min = getMin(min, val);
                max = getMax(max, val);
                sum += val;
                sum2 += val * val;
                count++;

                ind = static_cast<I>((val - rangeMin) * rangeScaleFactor);

                for (size_t i = 0; i < extent; ++i) {
                    v = glmcomp(ind, i);
                    if (v < bins) {
                        histograms[i][v]++;
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < extent; ++i) {
        histograms[i].dataRange_ = dataRange;
        histograms[i].stats_.min = glmcomp(min, i);
        histograms[i].stats_.max = glmcomp(max, i);
        histograms[i].stats_.mean = glmcomp(sum, i) / count;
        histograms[i].stats_.standardDeviation = std::sqrt(
            (count * glmcomp(sum2, i) - glmcomp(sum, i) * glmcomp(sum, i)) /
            (count * (count - 1)));

        histograms[i].calculatePercentiles();
        histograms[i].performNormalization();
        histograms[i].calculateHistStats();
        histograms[i].setValid(true);
    }

    return histograms;
}

#endif  // IVW_VOLUMERAMHISTOGRAM_H
