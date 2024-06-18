// Copyright (C) 2020-2022 Manuel Perez <manuel2perez@proton.me>
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
 *  - HISTORIA:
 *    Manuel Perez
 *	30/12/2017 Escrito
 *	26/07/2020 Cambio interfaz de Image_xy. Era raro...
 *	28/11/2020 Migro Image_xy a alp.
 *	01/09/2022 Image_as_array
 *
 ****************************************************************************/
#include <alp_concepts.h>
#include <alp_matrix_view.h>
#include <alp_submatrix.h>
#include <alp_matrix_algorithm.h> // posicion_del_centro
#include <alp_math.h>
#include <alp_type_traits.h>
#include <alp_rframe_xy.h>

#include "img_image.h"	// Rectangulo
#include "img_color.h"	// Color_red...

namespace img{
/***************************************************************************
 *				SUBIMAGEN
 ***************************************************************************/
using Subimage       = alp::Submatrix<Image>;
using const_Subimage = alp::Submatrix<const Image>;



// Esta es una copia de la función alp::clone. No puedo usar la de alp ya que
// usa Matrix y no Image (a día de hoy son distintas clases). Si las fusiono
// en el futuro sobra esta función.
inline auto clone(const Subimage& img0)
{
     Image res{img0.size2D()};

    auto f0 = img0.row_begin();
    auto f1 = res.row_begin();

    for (; f0 != img0.row_end(); ++f0, ++f1)
	std::copy(f0->begin(), f0->end(), f1->begin());

    return res;
}


inline auto clone(const const_Subimage& img0)
{
     Image res{img0.size2D()};

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
using Image_view = alp::Matrix_view<It>;


// Hay un problema con los alias de templates: en los alias no funciona la 
// deducción automática de tipos en el constructor, luego si bien compila
//	Matrix_view m{img0, Color_red{}};
//
//  no compila
//	Image_view m{img0, Color_red{}};
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
 *				Image_xy
 ***************************************************************************/
// TODO: no funciona la deducción automática con el using!!!
template <int x_sign = +1, int y_sign = +1>
using Image_xy =
    alp::Matrix_xy<Image::value_type, Image::Ind, x_sign, y_sign>;

template <int x_sign = +1, int y_sign = +1>
using const_Image_xy =
    alp::const_Matrix_xy<Image::value_type, Image::Ind, x_sign, y_sign>;

/***************************************************************************
 *				Image_rt
 ***************************************************************************/
/*!
 *  \brief  Image donde las coordenadas las damos en polares (rho, theta)
 *
 *  De momento solo voy a dibujar imagenes polares tales que 0 <= r <= r_max
 *  y cualquier theta. Esto es, el origen estará en el centro de la imagen,
 *  correspondiendo al punto r = 0.
 */
class Image_rt{
public:
    using Image_xy = img::Image_xy<1,1>;
    // Tipos
    using Ind	= double;	// voy a permitir pasar decimales
    using Radio = double;
    using Angulo= double;

    using Longitud  = Image_xy::Ind;
    using value_type= Image_xy::value_type;
    using Point	    = alp::Vector_xy<Ind>;

    using Ind_xy = Image_xy::Ind;


    /// Colocamos una máscara sobre la imagen img0
    Image_rt(Image& img0)
	    :img_{img0}
    { img_.origen_de_coordenadas_en_el_centro(); }


    /// Valor máximo de r
    Radio r_max() const {return img_.x_max();}


    /// Número de filas
    Image_xy::Ind rows() const {return img_.rows();}

    /// Número de columnas
    Image_xy::Ind cols() const {return img_.cols();}


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
    Image_xy img_;
};



/***************************************************************************
 *			    Image_as_array
 ***************************************************************************/
template <typename Image_type>
class Image_as_array_base{
public:
    using size_type = Image_type::size_type;
    static constexpr int ncolors = Image_type::value_type::ncolors;
    static_assert(ncolors == 3); // usamos que son r, g, b en operator()

    Image_as_array_base(Image_type& img0) : img_{img0} { }

    size_type size() const { return img_.size() * ncolors;}

    alp::Same_const_as<int, Image_type>& operator[](size_type n);
    const int& operator[](size_type n) const;

private:
    Image_type& img_;
};


template <typename Image_type>
alp::Same_const_as<int, Image_type>& 
Image_as_array_base<Image_type>::operator[](size_type n)
{
    size_type cols_x_i_plus_j = n / 3; // cols*i + j
    size_type c = n - 3*cols_x_i_plus_j;
    size_type i = cols_x_i_plus_j / img_.cols();
    size_type j = cols_x_i_plus_j - i * img_.cols();

    switch (c){
	break; case 0: return img_(i, j).r;
	break; case 1: return img_(i, j).g;
	break; case 2: return img_(i, j).b;
    }

    throw std::logic_error{
        "Image_as_array_base::operator[]: what I am doing here?"};
}


template <typename Image_type>
const int& 
Image_as_array_base<Image_type>::operator[](size_type n) const
{
    size_type cols_x_i_plus_j = n / 3; // cols*i + j
    size_type c = n - 3*cols_x_i_plus_j;
    size_type i = cols_x_i_plus_j / img_.cols();
    size_type j = cols_x_i_plus_j - i * img_.cols();

    switch (c){
	break; case 0: return img_(i, j).r;
	break; case 1: return img_(i, j).g;
	break; case 2: return img_(i, j).b;
    }

    throw std::logic_error{
        "Image_as_array_base::operator[]: what I am doing here?"};
}


using Image_as_array = Image_as_array_base<Image>;
// No funciona bien const_Image_as_array. Si se descomenta probarlo.
//using const_Image_as_array = Image_as_array_base<const Image>;


}



#endif


