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


/****************************************************************************
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *	22/03/2016 Escrito
 *
 ****************************************************************************/
#include "img_algorithm.h"

#include <alp_rframe_xy.h>

#include "img_draw.h"

namespace img
{

Size2D _rotate_dimensions(const Imagen& img0, const alp::Degree& angle)
{
    using Vector= const_Imagen_xy::Vector;

    const_Imagen_xy img_xy{img0};

    img_xy.origen_de_coordenadas_en_el_centro();

    // diagonales
    Vector d1 = upper_right_corner(img_xy) - bottom_left_corner(img_xy);
    Vector d2 = upper_left_corner(img_xy) - bottom_right_corner(img_xy);

    d1 = alp::rotate(d1, angle);
    d2 = alp::rotate(d2, angle);
    
    Size2D sz;
    sz.rows = std::max(std::abs(d1.y), std::abs(d2.y)) + 1;
    sz.cols = std::max(std::abs(d1.x), std::abs(d2.x)) + 1;

    return sz;
}




Imagen rotate(const Imagen& img0, alp::Degree angle)
{
    angle = alp::normalize(angle);

    const_Imagen_xy v0{img0}; // v0 = view0
    v0.origen_de_coordenadas_en_el_centro();

    auto y = img::imagen_negra(_rotate_dimensions(img0, angle));
    Imagen_xy v1{y};
    v1.origen_de_coordenadas_en_el_centro();

    Reference_frame_rotation rota{angle};

    for (int x = v0.x_min(); x <= v0.x_max(); ++x){
	for (int y = v0.y_min(); y <= v0.y_max(); ++y){
	    auto [X, Y] = rota(x, y);

	    if (v1.x_min() <= X and X <= v1.x_max()
			    and
		v1.y_min() <= Y and Y <= v1.y_max()) {
		v1(X,Y) = v0(x,y);
	    }
	}
    }


    return y;

}



// Rota la imagen +90 grados.
//
// Para rotar una imagen: img0 = rota_mas_90(img0);
Imagen rota_mas_90(const Imagen& img0)
{
    Imagen res{img0.cols(), img0.rows()};

    using size_t = Imagen::Ind;
    for(size_t I = 0; I < img0.rows(); ++I)
	for(size_t J = 0, i = res.rows() - 1; J < img0.cols(); ++J, --i)
	    res(i, I) = img0(I, J);

    return res;
}



// Rota la imagen -90 grados.
//
// Para rotar una imagen: img0 = rota_menos_90(img0);
Imagen rota_menos_90(const Imagen& img0)
{
    Imagen res{img0.cols(), img0.rows()};
    
    using size_t = Imagen::Ind;
    for(size_t I = 0, j = res.cols() - 1; I < img0.rows(); ++I, --j)
	for(size_t J = 0; J < img0.cols(); ++J)
	    res(J, j) = img0(I, J);

    return res;
}



// Rota la imagen 180 grados.
//
// Para rotar una imagen: img0 = rota_180(img0);
Imagen rota_180(const Imagen& img0)
{
    Imagen res{img0.rows(), img0.cols()};

    using size_t = Imagen::Ind;
    for(size_t I = 0, i = res.rows() - 1; I < img0.rows(); ++I, --i)
	for(size_t J = 0, j = res.cols() - 1; J < img0.cols(); ++J, --j)
	    res(i, j) = img0(I, J);

    return res;
}

// Devuelve la imagen simétrica a img0, respecto del eje y
Imagen simetrica_y(const Imagen& img0)
{
    Imagen res{img0.rows(), img0.cols()};

    for(Imagen::Ind i = 0; i < img0.rows(); ++i)
	for(Imagen::Ind j = 0, jp = img0.cols()-1
				    ; j < img0.cols(); ++j, --jp)
	    res(i, jp) = img0(i,j);

    return res;
}


// Devuelve la imagen simétrica a img0, respecto del eje y
Imagen simetrica_x(const Imagen& img0)
{
    Imagen res{img0.rows(), img0.cols()};

    for(Imagen::Ind i = 0, ip = img0.rows()-1;
				    i < img0.rows(); ++i, --ip)
	for(Imagen::Ind j = 0; j < img0.cols(); ++j)
	    res(ip, j) = img0(i,j);

    return res;
}


// Muestrea una imagen, devolviendo la imagen muestreada.
// La distancia de muestreo (distancia entre puntos que seleccionamos)
// es di, en la dirección de i, y dj en la dirección de j.
//
// Observar que es un algoritmo genérico, que opera sobre contenendores
// bidimensionales.
//Imagen muestrea(const Imagen& img0, int /* Distancia no nula */ di
//				  , int /* Distancia */ dj)
//{
//    // precondición
//    if(img0.rows() == 0 or img0.cols() == 0){
//	alp::trace(6) <<   "ERROR: img::muestrea: "
//			    "Intentando muestrear una imagen nula\n";
//
//	return img0;
//    }
//
//
//    int /* Num_filas */	    m = num_grupos(img0.rows(), di);
//    int /* Num_columnas */  n = num_grupos(img0.cols(), dj);
//
//    Imagen res{m , n};
//
//    for(Imagen::Ind i = 0; i < m; ++i)
//	for(Imagen::Ind j = 0; j < n; ++j)
//	    res(i, j) = img0(i*di, j*dj);
//
//    return res;
//}




// Expande una imagen, haciéndola más grande. 
// Convierte cada pixel en un punto gordo de ancho 'a'
Imagen expande(const Imagen& img0, Imagen::Ind a)
{
    Imagen res{img0.rows()*a, img0.cols()*a};

    for(Imagen::Ind i = 0; i < img0.rows(); ++i)
	for(Imagen::Ind j = 0; j < img0.cols(); ++j){
	    Subimagen sb{res
			, Posicion{a*i, a*j}
			, Posicion{a*(i+1)-1, a*(j+1)-1}};
	    std::fill(sb.begin(), sb.end(), img0(i,j));
	}

    return res;
}


Reference_frame_rotation::Reference_frame_rotation(const alp::Degree& angle)
    : sin_{alp::sin(angle)}, cos_{alp::cos(angle)} { }


std::pair<int, int> Reference_frame_rotation::operator()(int x, int y) const
{
    int X = std::round(x*cos_ - y*sin_);
    int Y = std::round(x*sin_ + y*cos_);

    return {X, Y};
}



}// namespace img


