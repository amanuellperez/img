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


#ifndef __IMG_IMAGE_H__
#define __IMG_IMAGE_H__
/****************************************************************************
 *
 *   - DESCRIPCIÓN: Tipos básicos de Image
 *
 *   - HISTORIA:
 *           alp  - 29/03/2016 Escrito
 *		    25/08/2017 Reescribo la forma de iterar por filas.
 *			       Añado iteradores bidimensionales.
 *		    19/11/2017 Añado Image_xy.
 *		    31/03/2019 Convierto Image en Matrix<ColorRGB>
 *
 ****************************************************************************/
#include <iostream>
#include <iterator>
#include <cstddef>  // ptrdiff_t

#include <alp_matrix.h>	// Row_iterator
#include <alp_matrix_algorithm.h>

#include "img_color.h"

// Todos los iteradores son genéricos a matrices, por eso van dentro de 
// este espacio de nombres.
namespace img{


/***************************************************************************
 *				TIPOS 
 ***************************************************************************/
// El índice y la posición son tipos pertenecientes a Image.
// Sin embargo, en todo el paquete img vamos a estar accediendo continuamente
// a zonas de la imagen y es un incordio tener que estar escribiendo
// continuamente Image::Ind, ... Por eso doy todos estos alias, por
// comodidad.
using Ind	= int;	// Tiene que ser tipo signed para que funcionen
		    // alp::Position, Rectangulo...

using Image   = alp::Matrix<ColorRGB, Ind>;
using Position = alp::Matrix<ColorRGB, Ind>::Position;
using Size2D   = alp::Matrix<ColorRGB, Ind>::Size2D;
using Range2D  = alp::Matrix<ColorRGB, Ind>::Range2D;

using Range2D_acotado = alp::Range_acotado_ij<Ind>;


/// Indica si una posición pertenece a una imagen o no
inline bool belongs(Position p, const Image& img)
{ return alp::belongs(p).to(img.extension()); }

// Puntos notables
// ---------------
/// Devuelve la posición que ocupa el centro de la imagen
inline Position posicion_del_centro(const Image& img)
{ return alp::posicion_del_centro(img.extension()); }


// Lectura/escritura
// -----------------
/// Lee la imagen del fichero 'name'.
Image read(const std::string& name);

/// Escribe la imagen en el fichero 'name'.
void write(const Image& img, const std::string& name);


} //namespace img


#endif

