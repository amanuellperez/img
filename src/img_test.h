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

#ifndef __IMG_TEST_H__
#define __IMG_TEST_H__

#include <alp_exception.h>
#include "img_image.h"

namespace test{


/// Crea una nueva imagen de rows x cols a partir de lst. Solo escribe el red.
template <typename It>
inline img::Image nueva_imagen_red(int rows, int cols, It p0, It pe)
{
    img::Image res{rows, cols};

//    auto p0 = lst.begin();
//    auto pe = lst.end();
    
    auto q0 = res.begin();
    auto qe = res.end();

    while (p0 != pe and q0 != qe){
	q0->r = *p0;
	q0->g = 0;
	q0->b = 0;
	++p0;
	++q0;
    }

    if (p0 != pe or q0 != qe)
        throw alp::Excepcion("ERROR: nueva_imagen: Se está intentando crear "
                             "una imagen con un número de filas o columnas "
                             "erróneo");

    return res;
}


/// Crea una nueva imagen de rows x cols a partir de lst. Solo escribe el red.
inline img::Image nueva_imagen_red(int rows, int cols, std::initializer_list<int> lst)
{
    return nueva_imagen_red(rows, cols, lst.begin(), lst.end());
}



// TODO: meterla en .cpp
inline img::Image lee_img_red(const std::string& nom_fichero)
{
    std::ifstream in{nom_fichero};

    if (!in)
	throw alp::File_cant_read{nom_fichero};

    std::vector<int> imagen;
    size_t num_lineas = 0;

    for (std::string tmp; std::getline(in, tmp); ++num_lineas){
	std::stringstream str;
	str << tmp;
	int red;
	while (str >> red)
	    imagen.push_back(red);
    }

    if (num_lineas)
	return nueva_imagen_red(num_lineas, imagen.size()/num_lineas, imagen.begin(), imagen.end());

    throw alp::Error_de_formato{"El fichero está vacío"};
}

}// namespace

#endif
