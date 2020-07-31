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


#ifndef __IMG_VIEW_H__
#define __IMG_VIEW_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Views que podemos aplicar a una imagen
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *	30/12/2017 Escrito
 *	26/07/2020 Cambio interfaz de Imagen_xy. Era raro...
 *
 ****************************************************************************/


#include <alp_concepts.h>
#include <alp_matrix_view.h>
#include <alp_submatrix.h>
#include <alp_matrix_algorithm.h> // posicion_del_centro
#include <alp_math.h>
#include <alp_type_traits.h>

#include "img_imagen.h"	// Rectangulo
#include "img_color.h"	// Color_red...

namespace img{
/***************************************************************************
 *				SUBIMAGEN
 ***************************************************************************/
using Subimagen       = alp::Submatrix<Imagen>;
using const_Subimagen = alp::Submatrix<const Imagen>;



// Esta es una copia de la función alp::clone. No puedo usar la de alp ya que
// usa Matrix y no Imagen (a día de hoy son distintas clases). Si las fusiono
// en el futuro sobra esta función.
inline auto clone(const Subimagen& img0)
{
     Imagen res{img0.size2D()};

    auto f0 = img0.row_begin();
    auto f1 = res.row_begin();

    for (; f0 != img0.row_end(); ++f0, ++f1)
	std::copy(f0->begin(), f0->end(), f1->begin());

    return res;
}


inline auto clone(const const_Subimagen& img0)
{
     Imagen res{img0.size2D()};

    auto f0 = img0.row_begin();
    auto f1 = res.row_begin();

    for (; f0 != img0.row_end(); ++f0, ++f1)
	std::copy(f0->begin(), f0->end(), f1->begin());

    return res;
}

/***************************************************************************
 *				IMAGEN_VIEW
 ***************************************************************************/
template <typename It>
using Imagen_view = alp::Matrix_view<It>;


// Hay un problema con los alias de templates: en los alias no funciona la 
// deducción automática de tipos en el constructor, luego si bien compila
//	Matrix_view m{img0, Color_red{}};
//
//  no compila
//	Imagen_view m{img0, Color_red{}};
//
//  Por eso necesito esta función: (syntactic sugar)

template <typename Container2D, typename View>
// requires: is_imagen<Container2D>
inline auto imagen_view(Container2D& img1, View view)
{
    return alp::matrix_view(img1.begin(), img1.rows(), img1.cols(), view);
}

template <typename Container2D, typename View>
// requires: is_imagen<Container2D>
inline auto const_imagen_view(Container2D& img1, View view)
{
return alp::const_matrix_view(img1.cbegin(), img1.rows(), img1.cols(), view);
}



/// Devuelve una view viendo solo el color red.
template <typename Container2D>
// requires: is_imagen<Container2D>
inline auto imagen_red(Container2D& img1)
{
    return alp::matrix_view(img1.begin(), img1.rows(), img1.cols(), Color_red{});
}

template <typename Container2D>
// requires: is_imagen<Container2D>
inline auto imagen_green(Container2D& img1)
{
    return alp::matrix_view(img1.begin(), img1.rows(), img1.cols(), Color_green{});
}

template <typename Container2D>
// requires: is_imagen<Container2D>
inline auto imagen_blue(Container2D& img1)
{
    return alp::matrix_view(img1.begin(), img1.rows(), img1.cols(), Color_blue{});
}

template <typename Container2D>
// requires: is_imagen<Container2D>
inline auto const_imagen_red(Container2D& img1)
{
    return alp::const_matrix_view(img1.cbegin(), img1.rows(), img1.cols(), 
							const_Color_red{});
}


template <typename Container2D>
// requires: is_imagen<Container2D>
inline auto const_imagen_green(Container2D& img1)
{
    return alp::const_matrix_view(img1.cbegin(), img1.rows(), img1.cols(), 
							const_Color_green{});
}

template <typename Container2D>
// requires: is_imagen<Container2D>
inline auto const_imagen_blue(Container2D& img1)
{
    return alp::const_matrix_view(img1.cbegin(), img1.rows(), img1.cols(), 
							const_Color_blue{});
}




/***************************************************************************
 *				Imagen_xy
 ***************************************************************************/
/*!
 *  \brief Iteramos por una imagen usando sistema de referencia {x,y}
 *
 *  En muchas ocasiones es mucho más natural usar como coordenadas un sistema
 *  de coordenadas cartesiano {x,y}. Esta máscara suministra dicho interfaz.
 *
 *  El sistema de referencia xy que se usa tendrá como origen de coordenadas
 *  el punto (i0, j0) (en coordenadas de matriz) y estará orientado de forma
 *  normal (el eje x hacia la derecha y el eje y hacia arriba).
 *
 *  Voy a permitir cambiar de coordenadas (x,y) a (i,j), y dar acceso a la
 *  imagen desde esta clase para poder reutilizar algoritmos de dibujo.
 *  Imaginar que creo una función draw_circle(Imagen); ¿cómo definir la
 *  equivalente para Imagen_xy sin hacerla friend? Basta con saber cómo
 *  convertir del sistema de coordenadas xy a ij y tener acceso a la imagen
 *  para poder reescribir cualquier función definida usando Imagen para
 *  Imagen_xy.
 *
 *
 *  TODO: es genérico si se parametriza por Imagen. Llevarlo a alp.
 */
class Imagen_xy{
public:
    // Las coordenadas (x,y) van a ser enteras.

    // Tipos
    using Ind	    = int;
    using Punto	    = alp::Punto_xy<Ind>;

    using value_type= Imagen::value_type;

    /// Colocamos una máscara sobre la imagen img0. Por defecto el origen de
    /// coordenadas esta en la esquina inferior izquierda de la imagen.
    Imagen_xy(Imagen& img0, Ind i0 = - 1, Ind j0 = -1)
	    :img_{&img0}
    {
	if (i0 != -1)
	    i0_ = i0;
	else
	    i0_ = img0.rows() - 1;

	if (j0 != -1)
	    j0_ = j0;
	else 
	    j0_ = 0;
    }

    /// Fijamos el origen de coordenadas. Después de la llamada de esta
    /// función cualquier acceso usará este nuevo origen.
    void origen_de_coordenadas(Ind i0, Ind j0);

    void origen_de_coordenadas(const Posicion& p0)
    {origen_de_coordenadas(p0.i, p0.j);}

    /// Colocamos el origen de coordenadas en el centro de la imagen
    void origen_de_coordenadas_en_el_centro()
    { origen_de_coordenadas(alp::posicion_del_centro(*img_)); }
    
    /// Valor mínimo de x
    Ind x_min() const {return x(0);}

    /// Valor máximo de x
    Ind x_max() const {return x(cols() - 1);}

    /// Valor mínimo de y
    Ind y_min() const {return y(rows() - 1);}

    /// Valor máximo de y
    Ind y_max() const {return y(0);}


    /// Número de filas
    Ind rows() const {return img_->rows();}

    /// Número de columnas
    Ind cols() const {return img_->cols();}

    /// Devuelve el color del pixel (x,y)
    /// OJO: se da por supuesto que el usuario sabe dónde está accediendo.
    /// Es responsabilidad del usuario validar que los puntos (x, y) sean
    /// puntos válidos de la imagen.
    value_type& operator()(Ind x, Ind y)
    {return (*img_)(posicion(x,y));}

    /// Devuelve el color del pixel (x,y)
    /// OJO: se da por supuesto que el usuario sabe dónde está accediendo.
    /// Es responsabilidad del usuario validar que los puntos (x, y) sean
    /// puntos válidos de la imagen.
    const value_type& operator()(Ind x, Ind y) const
    {return (*img_)(posicion(x,y));}


// Cambio de sistema de referencia
// -------------------------------
    /// Devuelve la posición (i,j) a la que apunta el punto de coordenadas (x,y)
    Posicion posicion(Ind x, Ind y) const
    { return {i(y), j(x)};}

    /// Devuelve la posición (i,j) a la que apunta el punto p
    Posicion posicion(const Punto& p) const
    {return posicion(p.x, p.y);}


    /// Cambia la coordenada x en la coordenada j correspondiente
    Imagen::Ind j(Ind x) const { return (j0_ + x); }

    /// Cambia la coordenada y en la coordenada i correspondiente
    Imagen::Ind i(Ind y) const { return (i0_ - y);}

    /// Pasamos de j a x (j --> x)
    Ind x(Imagen::Ind j) const { return (j - j0_);}

    /// Pasamos de i a y (i --> y)
    Ind y(Imagen::Ind i) const { return (i0_ - i);}


    /// Acceso a la imagen
    Imagen& img() {return *img_;}

    /// Acceso a la imagen
    const Imagen& img() const {return *img_;}

private:
    Imagen* img_;

    // Coordenadas de matriz (i, j) del origen del sistema de referencia.
    Ind i0_, j0_;

};


// Fijamos el origen de coordenadas. Después de la llamada de esta
// función cualquier acceso usará este nuevo origen.
inline void Imagen_xy::origen_de_coordenadas(Ind i0, Ind j0)
{
    i0_ = i0;
    j0_ = j0;
}




/***************************************************************************
 *				Imagen_rt
 ***************************************************************************/
/*!
 *  \brief  Imagen donde las coordenadas las damos en polares (rho, theta)
 *
 *  De momento solo voy a dibujar imagenes polares tales que 0 <= r <= r_max
 *  y cualquier theta. Esto es, el origen estará en el centro de la imagen,
 *  correspondiendo al punto r = 0.
 */
class Imagen_rt{
public:
    // Tipos
    using Ind	= double;	// voy a permitir pasar decimales
    using Radio = double;
    using Angulo= double;

    using Longitud  = Imagen_xy::Ind;
    using value_type= Imagen_xy::value_type;
    using Punto	    = alp::Punto_xy<Ind>;

    using Ind_xy = Imagen_xy::Ind;


    /// Colocamos una máscara sobre la imagen img0
    Imagen_rt(Imagen& img0)
	    :img_{img0}
    { img_.origen_de_coordenadas_en_el_centro(); }


    /// Valor máximo de r
    Radio r_max() const {return img_.x_max();}


    /// Número de filas
    Imagen_xy::Ind rows() const {return img_.rows();}

    /// Número de columnas
    Imagen_xy::Ind cols() const {return img_.cols();}


    /// Devuelve el color del pixel (rho, theta)
    value_type& operator()(Radio r, Angulo theta)
    {
	auto [x, y] = alp::polares2cartesianas(r, theta);
	return img_(static_cast<Ind_xy>(x), static_cast<Ind_xy>(y));
    }

    /// Devuelve el color del pixel (rho, theta)
    const value_type& operator()(Radio r, Angulo theta) const
    {
	auto [x, y] = alp::polares2cartesianas(r, theta);
	return img_(static_cast<Ind_xy>(x), static_cast<Ind_xy>(y));
    }

private:
    Imagen_xy img_;
};






}



#endif


