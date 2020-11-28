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
 *	28/11/2020 Migro Imagen_xy a alp.
 *
 ****************************************************************************/


#include <alp_concepts.h>
#include <alp_matrix_view.h>
#include <alp_submatrix.h>
#include <alp_matrix_algorithm.h> // posicion_del_centro
#include <alp_math.h>
#include <alp_type_traits.h>
#include <alp_rframe_xy.h>

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
using Imagen_xy = alp::Matrix_xy<Imagen::value_type, Imagen::Ind>;
using const_Imagen_xy = alp::const_Matrix_xy<Imagen::value_type, Imagen::Ind>;


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
    using Point	    = alp::Vector_xy<Ind>;

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


