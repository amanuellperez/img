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

#ifndef __IMG_DRAW_H__
#define __IMG_DRAW_H__

#include "img_image.h"
#include "img_view.h"

namespace img{

using Segmento   = alp::Segment_ij<int>;
//using Rectangulo = alp::Rectangulo<Position>;
using Rectangulo = alp::Rectangle_ij<int>;


/// Crea una imagen de tamaño num_filas x num_cols del color indicado
Image imagen_monocolor(Image::Ind num_filas
		    , Image::Ind num_cols, const ColorRGB& color);

/// Crea una imagen de tamaño sz indicado del color 'color'
inline Image imagen_monocolor(Image::Size2D sz, const ColorRGB& color)
{return imagen_monocolor(sz.rows, sz.cols, color);}

/// Crea una imagen de tamaño `rows x cols` totalmente negra
inline Image imagen_negra(Image::Ind rows, Image::Ind cols)
{ return imagen_monocolor(rows, cols, ColorRGB::negro());}

/// Crea una imagen de tamaño num_filas x num_cols totalmente negra
inline Image imagen_negra(Image::Size2D sz)
{return imagen_negra(sz.rows, sz.cols);}

/****************************************************************************
 *
 *   - FUNCIÓN: draw
 *
 *   - DESCRIPCIÓN: Funciones de dibujo
 *
 ****************************************************************************/
void draw(Image& img, const Segmento& s, const ColorRGB& color);


inline void draw(Image& img, const Rectangulo& r, const ColorRGB& color)
{
    draw(img, Segmento{r.upper_left_corner(), r.upper_right_corner()}, color);
    draw(img, Segmento{r.bottom_left_corner(), r.bottom_right_corner()}, color);
    draw(img, Segmento{r.upper_left_corner(), r.bottom_left_corner()}, color);
    draw(img, Segmento{r.upper_right_corner(), r.bottom_right_corner()}, color);

//    draw(img, Segmento{r.SI(), r.SD()}, color);
//    draw(img, Segmento{r.II(), r.ID()}, color);
//    draw(img, Segmento{r.SI(), r.II()}, color);
//    draw(img, Segmento{r.SD(), r.ID()}, color);
}

inline void draw_lineaH(Image& img, Image::Ind i, const ColorRGB& c)
{ draw(img, Segmento{{i, 0}, {i, img.cols()-1}}, c); }

inline void draw_lineaV(Image& img, Image::Ind j, const ColorRGB& c)
{ draw(img, Segmento{{0, j}, {img.rows()-1, j}}, c); }


// dibuja un eje horizontal que pasa por p
inline void draw_eje_x(Image& img, const Image::Position& p, const ColorRGB& c)
{ draw(img, Segmento{{p.i, 0}, {p.i, img.cols()-1}}, c); }

inline void draw_eje_x(Image& img, Image::Ind i, const ColorRGB& c) 
{draw_eje_x(img, {i,0}, c);}

// dibuja un eje vertical que pasa por p
inline void draw_eje_y(Image& img, const Image::Position& p, const ColorRGB& c)
{ draw(img, Segmento{{0, p.j}, {img.rows()-1, p.j}}, c); }

inline void draw_eje_y(Image& img, Image::Ind j, const ColorRGB& c) 
{draw_eje_y(img, {0,j}, c);}

inline void draw_ejes(Image& img, const Image::Position& p, const ColorRGB& c)
{
    draw_eje_x(img, p,c);
    draw_eje_y(img, p,c);
}

// coloreamos con un color determinado ColorFunc
// TODO: esto es fill!!!
template<typename ColorFunc, typename It>
inline void colorea(const It& begin, const It& end, int c)
{
    ColorFunc color;

    for(auto p = begin; p != end; ++p)
	color(*p, c);
}


// coloreamos con un color determinado ColorFunc
// n = número de elementos a colorear
// Devuelve: ps = siguiente iterador. Colorea [p0, ps)
template<typename ColorFunc, typename It>
inline It colorea(const It& p0, int n, int c)
{
    ColorFunc color;

    auto p = p0;
    for(int i = 0; i != n; ++i, ++p)
	color(*p, c);

    return p;
}



/***************************************************************************
 *		    FUNCIONES DE DIBUJO PARA Image_xy<1,1>
 ***************************************************************************/
/// Dibuja el eje x en la posición y de la imagen
inline void draw_eje_x(Image_xy<1,1>& img_xy, Image_xy<1,1>::Ind y, const ColorRGB& c)
{ draw_eje_x(img_xy.matrix(), img_xy.i(y), c);}

/// Dibuja el eje y en la posición x de la imagen
inline void draw_eje_y(Image_xy<1,1>& img_xy, Image_xy<1,1>::Ind x, const ColorRGB& c)
{ draw_eje_y(img_xy.matrix(), img_xy.j(x), c);}

/// Dibuja el segmento s en la imagen img del color indicado
// ¿merece la pena definir draw(Image_xy<1,1>::Segmento)??? De momento creo que
// no, por eso lo defino así. Ahora bien, si se usa mucho Image_xy<1,1> acabar por
// definir todas las funciones de dibujo para esta máscara.
inline void draw_segmento(Image_xy<1,1>& img_xy
			, const Image_xy<1,1>::Point& A
			, const Image_xy<1,1>::Point& B, const ColorRGB& c)
{ draw(img_xy.matrix(), Segmento{img_xy.posicion(A), img_xy.posicion(B)}, c);}


/***************************************************************************
 *		    FUNCIONES DE DIBUJO PARA Image_rt
 ***************************************************************************/
/// Dibuja la recta rho = cte de color c.
inline void draw_rayo(  Image_rt& img_rt
			, double theta, const ColorRGB& c)
{
    for (double r = 0.0; r <= img_rt.r_max(); ++r)
	img_rt(r, theta) = c;
}

/// Dibuja una circunferencia de radio r de color c.
/// incr_theta indica de cuánto en cuánto queremos aumentar el ángulo theta.
// DUDA: ¿se puede estimar incr_theta a partir de r y r_max? Seguramente.
inline void draw_circunferencia(  Image_rt& img_rt
			, double r, const ColorRGB& c
			, double incr_theta = 0.1)
{
    for (double theta = 0.0; theta < 360.0; theta += incr_theta)
	img_rt(r, theta) = c;
}


/// Dibuja un array de rayos separados una distancia ancho_grid_theta
inline void draw_array_rayos(Image_rt& img_rt
			    , double ancho_grid_theta, const ColorRGB& color)
{
    for (double theta = 0.0; theta < 360.0; theta += ancho_grid_theta)
	draw_rayo(img_rt, theta, color);
}


/// Dibuja un array de circunferencias separadas una distancia
/// ancho_grid_r
inline void draw_array_circunferencias(Image_rt& img_rt
				, double ancho_grid_r
				, const ColorRGB& color)
{
    for (double r = 0.0; r <= img_rt.r_max(); r += ancho_grid_r)
	draw_circunferencia(img_rt, r, color);
}


}// namespace

#endif


