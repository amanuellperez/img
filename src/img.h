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


#ifndef __IMG_IMG_H_
#define __IMG_IMG_H_
/***************************************************************************
 *  
 *  El paquete img contiene las clases mínimas para manejar imágenes.
 *  Una imagen es un contenedor bidimensional de colores RGB. Todas las
 *  funciones aquí definidas son genéricas para manejar contenedores
 *  bidimensionales.
 *  Aquí entran todos los algoritmos que operen sobre contenedores
 *  bidimensionales, no sobre colores.
 *
 *  Los algoritmos para manejar imagenes están en el paquete vis.
 *
 ***************************************************************************/


// --------
// img_core
// --------
// Nivel 1
#include "img_color.h"

// Nivel 2
#include "img_image.h"	    // clase Imagen
#include "img_iterator2D.h"   // Iteradores bidimensionales
#include "img_view.h"   // Máscaras: Region
//#include "img_grid.h"	    // Máscara: Grid

// ----------
// Algoritmos
// ----------
#include "img_algorithm.h"  // Algoritmos genéricos de contenedores bidimensionales
#include "img_draw.h"	    // Funciones de dibujo

// Que facilitan la lectura de código



#endif 
