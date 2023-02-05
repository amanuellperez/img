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


#include "img_color.h"

#include <iostream>

namespace img{

std::ostream& operator<<(std::ostream& out, const ColorRGB& c)
{
    out << '(';

    out.width(3); 
    if(c.r >= 0) out.fill('0'); 
    out << std::right << c.r << ", ";

    out.width(3); 
    if(c.g >= 0) out.fill('0'); 
    out << std::right << c.g << ", ";

    out.width(3); 
    if(c.b >= 0) out.fill('0'); 
    out << std::right << c.b << ')';

    return out;

}

// Función de implementación
// Lee el color de in, sin gestionar el estado de in en caso de que falle.
// Lo descompongo en dos funciones operator>> para que se lea mejor el flujo.
static bool read_colorRGB(std::istream& in, ColorRGB& color)
{
    char c;

    in >> c;
    if (!in) return false;

    if (c != '(') return false;

    in >> color.r >> c;
    if (!in or c != ',') return false;

    in >> color.g >> c;
    if (!in or c != ',') return false;

    in >> color.b >> c;
    if (!in or c != ')') return false;

    return true;
}

// Leemos un color en formato txt
std::istream& operator>>(std::istream& in, ColorRGB& color)
{
    if (!read_colorRGB(in, color))
	in.setstate(std::ios_base::failbit);

    return in;
}


} // namespace


