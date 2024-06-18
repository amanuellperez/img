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


#ifndef __IMG_ALGORITHM_H__
#define __IMG_ALGORITHM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Algoritmos básicos que podemos aplicar a un
 *	    contenedor bidimensional.
 *
 *  - COMENTARIOS: 
 *	    Estos algoritmos NO son sobre imágenes, sino sobre contenedores
 *	bidimensionales. No deben de saber nada de colores ni imágenes.
 *	Que no aparezca en este fichero en ningún sitio ColorRGB.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	22/03/2016 Escrito
 *	04/08/2020 rotate
 *
 ****************************************************************************/

#include <fstream>

#include "img_image.h"
#include "img_view.h"



namespace img{

// Devuelve las dimensiones donde alojar la imagen rotada.
Size2D _rotate_dimensions(const Image& img0, const alp::Degree& angle);

/// Rota la imagen img0 `angle` grados.
Image rotate(const Image& img0, alp::Degree angle);

/// Rota la imagen +90 grados.
///
/// Para rotar una imagen: img0 = rota_mas_90(img0);
Image rota_mas_90(const Image& img0);

/// Rota la imagen -90 grados.
///
/// Para rotar una imagen: img0 = rota_menos_90(img0);
Image rota_menos_90(const Image& img0);

/// Rota la imagen 180 grados.
///
/// Para rotar una imagen: img0 = rota_180(img0);
Image rota_180(const Image& img0);


/// Devuelve la imagen simétrica a img0, respecto del eje y
/// 
/// Es perfectamente válido: img1 = simetrica_y(img1);
Image simetrica_y(const Image& img0);

/// Devuelve la imagen simétrica a img0, respecto del eje x
/// 
/// Es perfectamente válido: img1 = simetrica_x(img1);
Image simetrica_x(const Image& img0);




/// Muestrea una imagen, devolviendo la imagen muestreada.
/// La distancia de muestreo (distancia entre puntos que seleccionamos)
/// es di, en la dirección de i, y dj en la dirección de j.
///
/// Observar que es un algoritmo genérico, que opera sobre contenendores
/// bidimensionales.
//Image muestrea(const Image& img0, int /* Distancia */ di
//				 ,  int /* Distancia */ dj);

/// Expande una imagen, haciéndola más grande. 
/// Convierte cada pixel en un punto gordo de ancho 'ancho'.
///
/// \todo Esta función está definida en img_escala???
Image expande(const Image& img0, int /* Ancho */ ancho);



// TODO: ¿Cómo generalizar esta clase?
// Ahora el operator(x,y) está pasando de (x,y) a (X,Y). ¿Cómo llamar a la
// función que haga lo contrario? 
// Usar Vector_xy en vez de (x,y). Se podía usar algo parecido a
// Vector_xy_in???:
//	Reference_frame_rotation rotate{alp::Degree{30}};
//	rotate(Vector_xy_in<0>): hace (X,Y) --> (x,y)
//	rotate(Vector_xy_in<1>): hace (x,y) --> (X,Y).
//  ¿O usar otro vector diferente a Vector_xy_in?
class Reference_frame_rotation{
public:
    Reference_frame_rotation(const alp::Degree& angle);

    // (x,y) --> (X,Y)
    std::pair<int, int> operator()(int x, int y) const;


private:
    double sin_;    // sin(angle)
    double cos_;    // cos(angle)

};


// TODO: comentar esta (???) Para depurar usar test::print2D ???
inline std::ostream& operator<<(std::ostream& out, const Image& img)
{ 
    for(auto f = img.row_begin(); f!= img.row_end(); ++f)
    {
	for(auto x: *f)
	    out << x << ' ';
	out << '\n';
    }

    return out;   
}


/*****************************************************************************
 * 
 *   - CLASE: Barridos
 *
 *   - DESCRIPCIÓN: Clases para poder hacer barridos horizontales y verticales
 *	tanto discretos como continuos. 
 *	Estas clases permiten desvincular el algoritmo de la forma en cómo
 *	hacemos el barrido, pudiendo usar un mismo algoritmo barriendo
 *	de diferentes formas.
 *
 *	La forma de iterar en un contenedor bidimensional es:
 *	    for(auto c = m.beginC(); c != m.endC(); ++c)
 *		for(auto p = c.begin(); p != c.end(); ++p)
 *		    ...
 *
 *	Usando estos barridos, lo reescribimos como:
 *	    for(auto c = Barrido::beginC(m); c != Barrido::endC(m); ++c)
 *		for(auto p = Barrido::begin(c); p != Barrido::end(c); ++p)
 *		    ...
 *
 ***************************************************************************/
//class BarridoH{
//public:
//    using const_it_contenedor	= Image::const_row_iterator;
//    using Contenedor		= Image::const_Row;
//    using const_iterator	= Contenedor::const_iterator;
//
//
//    static const_it_contenedor beginC(const Image& m) {return m.row_begin();}
//    static const_it_contenedor endC(const Image& m)   {return m.row_end();}
//
////    static const_it_contenedor beginC(const_Mascara& m) {return m.row_begin();}
////    static const_it_contenedor endC(const_Mascara& m)   {return m.row_end();}
//
//    static const_iterator begin(const Contenedor& c) {return c.begin();}
//    static const_iterator end(const Contenedor& c) {return c.end();}
//
//    // para iterar solo en una parte de una fila
//    static Contenedor contenedor(const Image& img , Position p0, Position pe)
//    {return img.fila(p0.i, p0.j, pe.j);}
//
//};
//
//class BarridoV{
//public:
//    using const_it_contenedor	= Image::const_col_iterator;
//    using Contenedor		= Image::const_Columna;
//    using const_iterator	= Contenedor::const_iterator;
//
//    static const_it_contenedor beginC(const Image& m) {return m.columnaB();}
//    static const_it_contenedor endC(const Image& m)   {return m.columnaE();}
//
////    static const_it_contenedor beginC(const_Mascara& m) {return m.columnaB();}
////    static const_it_contenedor endC(const_Mascara& m)   {return m.columnaE();}
//
//    static const_iterator begin(const Contenedor& c) {return c.begin();}
//    static const_iterator end(const Contenedor& c) {return c.end();}
//
//    // para iterar solo en una parte de una columna
//    static Contenedor contenedor(const Image& img , Position p0, Position pe)
//    {return img.columna(p0.j, p0.i, pe.i);}
//};





/***************************************************************************
 *		DIFERENCIAS PRIMERAS (primera derivada discreta)
 ***************************************************************************/
/*!
 *  \brief  Iteramos por las diferencias en x de una imagen
 *
 *  Ejemplo:
 *  \code
 *    for (int i = 0; i < img0.rows(); ++i){
 *	for (auto p = diferencias_x_B(Image_red{img0}, i);
 *		p != diferencias_x_E(Image_red{img0});
 *		++p)
 *	    cout << *p << ' ';
 *
 *	cout << '\n';
 *    }
 *  \endcode
 *
 */
template <typename Img>
// requires: Img es una imagen.
class It_diferencias_x{
public:
    using size_t = Image::Ind;
 
    // precondición: img0.cols() >= 1
    It_diferencias_x(const Img& img0, size_t i0, size_t j0)
	:img0_{img0}, i_{i0}, j1_{j0 + 1}
    {}
    
    It_diferencias_x& operator++() {++j1_; return *this;}

    // CUIDADO: no devuelve una referencia!!! No es un iterador
    // normal!!!
    auto operator*() const 
    {return img0_(i_, j1_) - img0_(i_, j1_ - 1);}


    friend bool operator==(const It_diferencias_x& a, const It_diferencias_x& b)
    {return a.j1_ == b.j1_;}


    friend Position posicion(const It_diferencias_x& a)
    {return {a.i_, a.j1_};}

private:
    const Img& img0_;

    // posición actual. Miramos el punto (i,j1) - (i, j1-1)
    size_t i_, j1_;
};



template <typename I>
inline bool operator!=(const It_diferencias_x<I>& a, const It_diferencias_x<I>& b)
{
    return !(a == b);
}


template <typename Img>
inline It_diferencias_x<Img>
diferencias_x_B(const Img& img0, Image::Ind i0, Image::Ind j0 = 0)
{
    return It_diferencias_x<Img>{img0, i0, j0};
}

template <typename Img>
inline It_diferencias_x<Img>
diferencias_x_E(const Img& img0)
{
    return It_diferencias_x<Img>{img0, 0, img0.cols() - 1};
}


// Corners
// -------
// ¿Qué sentido tienen las esquinas en un sistema cualquiera de referencia?
// Si el eje y va hacia abajo es un lio esto. ¿O dejamos un sistema de
// referencia humano siempre?
template <typename It>
inline alp::Matrix_xy_base<It,1,1>::Point
upper_left_corner(const alp::Matrix_xy_base<It, +1, +1>& img0)
{ return {img0.x_min(), img0.y_max()};}

template <typename It>
inline alp::Matrix_xy_base<It,1,1>::Point
upper_right_corner(const alp::Matrix_xy_base<It,1,1>& img0)
{ return {img0.x_max(), img0.y_max()};}

template <typename It>
inline alp::Matrix_xy_base<It,1,1>::Point
bottom_left_corner(const alp::Matrix_xy_base<It,1,1>& img0)
{ return {img0.x_min(), img0.y_min()};}

template <typename It>
inline alp::Matrix_xy_base<It,1,1>::Point
bottom_right_corner(const alp::Matrix_xy_base<It,1,1>& img0)
{ return {img0.x_max(), img0.y_min()};}


}// namespace img
#endif 


