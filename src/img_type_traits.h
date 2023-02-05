// Copyright (C) 2020 Manuel Perez <manuel2perez@proton.me>
//
// This file is part of the ALP Library.
//
// ALP Library is a free library: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#pragma once

#ifndef __IMG_TYPE_TRAITS_H__
#define __IMG_TYPE_TRAITS_H__


#include <iterator>
#include "img_color.h"

// --------------------------
// Traits de Image::iterator
// --------------------------
template<>
struct std::iterator_traits<img::ColorRGB*>{
    using difference_type = img::Image::difference_type;
    using value_type      = img::ColorRGB;
    using pointer         = img::ColorRGB*;
    using reference       = img::ColorRGB&;
    using iterator_category = std::random_access_iterator_tag;
};


template<>
struct std::iterator_traits<const img::ColorRGB*>{
    using difference_type = img::Image::difference_type;
    using value_type      = img::ColorRGB;
    using pointer         = const img::ColorRGB*;
    using reference       = const img::ColorRGB&;
    using iterator_category = std::random_access_iterator_tag;
};

