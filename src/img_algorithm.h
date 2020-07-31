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
 *    A.Manuel L.Perez
 *	22/03/2016 Escrito
 *
 ****************************************************************************/

#include <fstream>

#include "img_imagen.h"
#include "img_view.h"
#include "img_exp.h"



namespace img{

/// Rota la imagen img0 `angle` grados.
Imagen rotate(const Imagen& img0, const alp::Degree& angle);

/// Rota la imagen +90 grados.
///
/// Para rotar una imagen: img0 = rota_mas_90(img0);
Imagen rota_mas_90(const Imagen& img0);

/// Rota la imagen -90 grados.
///
/// Para rotar una imagen: img0 = rota_menos_90(img0);
Imagen rota_menos_90(const Imagen& img0);

/// Rota la imagen 180 grados.
///
/// Para rotar una imagen: img0 = rota_180(img0);
Imagen rota_180(const Imagen& img0);


/// Devuelve la imagen simétrica a img0, respecto del eje y
/// 
/// Es perfectamente válido: img1 = simetrica_y(img1);
Imagen simetrica_y(const Imagen& img0);

/// Devuelve la imagen simétrica a img0, respecto del eje x
/// 
/// Es perfectamente válido: img1 = simetrica_x(img1);
Imagen simetrica_x(const Imagen& img0);




/// Muestrea una imagen, devolviendo la imagen muestreada.
/// La distancia de muestreo (distancia entre puntos que seleccionamos)
/// es di, en la dirección de i, y dj en la dirección de j.
///
/// Observar que es un algoritmo genérico, que opera sobre contenendores
/// bidimensionales.
//Imagen muestrea(const Imagen& img0, int /* Distancia */ di
//				 ,  int /* Distancia */ dj);

/// Expande una imagen, haciéndola más grande. 
/// Convierte cada pixel en un punto gordo de ancho 'ancho'.
///
/// \todo Esta función está definida en img_escala???
Imagen expande(const Imagen& img0, int /* Ancho */ ancho);





inline std::ostream& operator<<(std::ostream& out, const Imagen& img)
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
//    using const_it_contenedor	= Imagen::const_row_iterator;
//    using Contenedor		= Imagen::const_Row;
//    using const_iterator	= Contenedor::const_iterator;
//
//
//    static const_it_contenedor beginC(const Imagen& m) {return m.row_begin();}
//    static const_it_contenedor endC(const Imagen& m)   {return m.row_end();}
//
////    static const_it_contenedor beginC(const_Mascara& m) {return m.row_begin();}
////    static const_it_contenedor endC(const_Mascara& m)   {return m.row_end();}
//
//    static const_iterator begin(const Contenedor& c) {return c.begin();}
//    static const_iterator end(const Contenedor& c) {return c.end();}
//
//    // para iterar solo en una parte de una fila
//    static Contenedor contenedor(const Imagen& img , Posicion p0, Posicion pe)
//    {return img.fila(p0.i, p0.j, pe.j);}
//
//};
//
//class BarridoV{
//public:
//    using const_it_contenedor	= Imagen::const_col_iterator;
//    using Contenedor		= Imagen::const_Columna;
//    using const_iterator	= Contenedor::const_iterator;
//
//    static const_it_contenedor beginC(const Imagen& m) {return m.columnaB();}
//    static const_it_contenedor endC(const Imagen& m)   {return m.columnaE();}
//
////    static const_it_contenedor beginC(const_Mascara& m) {return m.columnaB();}
////    static const_it_contenedor endC(const_Mascara& m)   {return m.columnaE();}
//
//    static const_iterator begin(const Contenedor& c) {return c.begin();}
//    static const_iterator end(const Contenedor& c) {return c.end();}
//
//    // para iterar solo en una parte de una columna
//    static Contenedor contenedor(const Imagen& img , Posicion p0, Posicion pe)
//    {return img.columna(p0.j, p0.i, pe.i);}
//};




/*!
 *  \brief  Devuelve los puntos de la imagen que se encuentran alrededor de p.
 *
 *  Ejemplo:
 *  \code
 *		for(auto p: alrededor(img0, {3,4})) ...
 *  \endcode
 *
 */
template <typename ContBi>
// requires: Contenedor_bidimensional(ContBi)
inline std::vector<Posicion> alrededor(const ContBi& c, Posicion p)
{
    std::vector<Posicion> v;
    v.reserve(8);
   
    // (i1, j1) valores máximos de los índices
    auto i1 = c.rows()-1; auto  j1 = c.cols() - 1;
    auto i = p.i; auto j = p.j;

    if(i-1 >= 0){
	if(j-1 >= 0)	v.push_back(Posicion{i-1, j-1});
	v.push_back(Posicion{i-1, j});
	if(j+1 <= j1)	v.push_back(Posicion{i-1, j+1});
    }

    // i
    if(j-1 >= 0)    v.push_back(Posicion{i, j-1});
//		    v.push_back(Posicion{i, j}); <--- este no está alrededor!
    if(j+1 <= j1)   v.push_back(Posicion{i, j+1});


    if(i+1 <= i1){
	if(j-1 >= 0)	v.push_back(Posicion{i+1, j-1});
	v.push_back(Posicion{i+1, j});
	if(j+1 <= j1)	v.push_back(Posicion{i+1, j+1});
    }

    return v;
}



/***************************************************************************
 *		DIFERENCIAS PRIMERAS (primera derivada discreta)
 ***************************************************************************/
/*!
 *  \brief  Iteramos por las diferencias en x de una imagen
 *
 *  Ejemplo:
 *  \code
 *    for (int i = 0; i < img0.rows(); ++i){
 *	for (auto p = diferencias_x_B(Imagen_red{img0}, i);
 *		p != diferencias_x_E(Imagen_red{img0});
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
    using size_t = Imagen::Ind;
 
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


    friend Posicion posicion(const It_diferencias_x& a)
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
diferencias_x_B(const Img& img0, Imagen::Ind i0, Imagen::Ind j0 = 0)
{
    return It_diferencias_x<Img>{img0, i0, j0};
}

template <typename Img>
inline It_diferencias_x<Img>
diferencias_x_E(const Img& img0)
{
    return It_diferencias_x<Img>{img0, 0, img0.cols() - 1};
}




}// namespace img
#endif 


