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


#include "img_draw.h"
#include "img_view.h"


namespace img{

Image imagen_monocolor(Image::Ind rows, Image::Ind cols
					    , const ColorRGB& color)
{
    Image img1{rows, cols};

    std::fill(img1.begin(), img1.end(), color);

    return img1;
}


/****************************************************************************
 *
 *   - FUNCIÓN: draw
 *
 *   - DESCRIPCIÓN: Dibuja un segmento usando el algoritmo de Bresenham
 *
 *   - PRECONDICIÓN: da por supuesto que el segmento pertenece a la imagen
 *
 *   - INPUT: Segmento s = segmento a dibujar
 *	      color	 = color del que dibujamos el segmento
 *
 *   - OUTPUT: 
 *
 ****************************************************************************/
void draw(Image& img, const Segmento& s, const ColorRGB& color)
{   
    using Ind = Image::Ind;

    // Caso degenerado
    //if(es_un_punto(s)){	
    if(s.A == s.B){	
	img({s.A.i, s.A.j}) = color;
	return;
    }

    // elegimos los extremos del segmento correctamente ("A < B")
    Position A{0,0}, B{0,0};
    if(s.A.i < s.B.i) { A = s.A; B = s.B; }
    else{
	if(s.A.i == s.B.i){
	    if(s.A.j < s.B.j) {A = s.A; B = s.B;}
	    else	      {A = s.B; B = s.A;}
	}
	else 
	    {A = s.B; B = s.A;}
    }

    // Elegimos el driving axis
    Ind incrI = B.i-A.i;
    Ind incrJ = B.j-A.j;
    Ind x0, x1, y0;
    Ind incrX, incrY;
    bool ejeXDA = true;	// el ejeX es el driving axis?

    if(incrJ <= incrI)
    {// el eje x es el driving axis
	incrX = incrI;
	incrY = incrJ;
	x0 = A.i; x1 = B.i;
	y0 = A.j;
	ejeXDA = true;
    }
    else
    {// el eje y es el driving axis
	incrX = incrJ;
	incrY = incrI;
	x0 = A.j; x1 = B.j;
	y0 = A.i;
	ejeXDA = false;
    }

    // Algoritmo de Bresenham. Dibujamos la línea
    Ind eps = incrY-incrX;
    Ind y = y0;

    // necesito incluir el signo de la pendiente para dibujar m < 0
    int signo = 1;
    if(incrY < 0) signo = -1;

    for(Ind x = x0 ; x <= x1; ++x)
    {
	if(ejeXDA) img({x,y}) = color;
	else img({y,x}) = color;

	if(eps>= 0)
	{
	    y = y + signo;  // ++y ó --y
	    eps-=incrX;
	}
	eps = eps + signo*incrY; // eps+= incrY; ó eps-=incrY
    }


}



}// namespace

