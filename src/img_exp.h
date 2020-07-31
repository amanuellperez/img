// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __ALP_IMG_EXP_H__
#define __ALP_IMG_EXP_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones experimentales.
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 13/03/2017 Escrito
 *  TODO: borrar (comentado el 31/07)
 *
 ****************************************************************************/
//
//#include <alp_math.h>
//
//
//namespace img{
///*!
// *  \brief  Hay veces que me encuentro con que agrupo los puntos de un plano
// *	(píxeles en una imagen, ...) en grupos de ancho a. Esta función 
// *	calcula el grupo al que pertenece x.
// *		
// *
// *  \code
// *	int xg = grupo(x, a);
// *
// *	Punto pg = grupo(p, a);
// *  \endcode
// *
// *  DUDA: ¿funciona bien esta función? Mirar num_grupos, que la estoy usando
// *	y es diferente!!!
// */
//inline constexpr int grupo(int x, int a)
//{return alp::cociente(x, a);}
//
///// Devuelve el número de grupos en que queda dividido n píxeles al 
///// agruparlos en grupos de ancho a. 
/////
///// Uso: por ejemplo, al muestrear una imagen.
//inline constexpr int num_grupos(int n, int a)
//{
//    auto c = alp::cociente(n, a);
//    auto r = alp::resto(n, a);
//
//    if(r == 0)	return c;
//
//    return (c+1);
//}
//
//

//}// namespace alp

#endif
