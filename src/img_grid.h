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

#ifndef __IMG_GRID_H__
#define __IMG_GRID_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Diferentes grids que podemos poner sobre una imagen.
 *
 *   - CUIDADO: cuidado con devolver en funciones Grid<Grid_d<Image>> g.
 *	    ¡¡¡No funciona!!!
 *
 *   - COMENTARIOS: 
 *	+ Grid:
 *	  ----
 *	    Grid<Image> g{img, m, n};	// divide la imagen en subimagenes de
 *					// tamaño m x n
 *
 *	    const Grid<(const) Image> g{img, m, n};
 *
 *	    auto g = grid(img, m, n);	// crea const (o no) si img es const
 *					// (o no)
 *
 *   - HISTORIA:
 *           alp  - 05/07/2016 Escrito
 *
 ****************************************************************************/
#include "img_view.h"

namespace img{


/*****************************************************************************
 * 
 *   - CLASE: Grid
 *
 *   - DESCRIPCIÓN: Le ponemos a una imagen de dimensiones M x N, un grid
 *	de dimensiones m_g x n_g, en donde cada región del grid es de
 *	dimensiones m x n.
 *
 *	Es un grid genérico que le podemos poner a cualquier ContenedorB.
 *
 *
 ***************************************************************************/
template<typename ContenedorB>
class Grid{
public:
    using value_type	= alp::Value_type<ContenedorB>;
    using size_type	= typename ContenedorB::size_type;

    using iterator	 = alp::Iterator_rnd_2D<Grid>;
    using const_iterator = alp::Iterator_rnd_2D<const Grid>;

    using Fila		= alp::Fila<iterator>;
    using const_Fila	= alp::Fila<const_iterator>;

    using it_fila	= alp::It_fila_rnd<Grid>;
    using const_it_fila	= alp::It_fila_rnd<const Grid>;


    // (I0, J0) = esquina superior izquierda
    // m x n = dimensiones de cada región en la que dividimos la imagen
    Grid(ContenedorB& vc, int vm, int vn)
	:c(&vc), m{vm}, n{vn}
    {
	m_g = c->rows()/m;
	n_g = c->cols()/n;
    }


    // ----------------------
    // Parte común con Región
    // ----------------------
    // acceso como contenedor unidimensional
    iterator begin() {return iterator{*this, 0, 0};}
    iterator end()   {return iterator{*this, rows(), 0};}
    iterator it(int i, int j) {return iterator{*this, i, j};}

    const_iterator begin() const {return const_iterator{*this, 0, 0};}
    const_iterator end()   const {return const_iterator{*this, rows(), 0};}
    const_iterator it(int i, int j) const {return const_iterator{*this, i, j};}

    // acceso bidimensional
    Fila fila(int i) {return Fila{iterator{*this, i, 0}
				, iterator{*this, i+1, 0}, cols()};}

    const_Fila fila(int i) const {return const_Fila{const_iterator{*this, i, 0}
				, const_iterator{*this, i+1, 0}, cols()};}

    it_fila row_begin() {return it_fila{*this, 0};}
    it_fila row_end() {return it_fila{*this, rows()};}

    const_it_fila row_begin() const {return const_it_fila{*this, 0};}
    const_it_fila row_end() const {return const_it_fila{*this, rows()};}

    // ----------------------------
    // Fin - parte común con Región
    // ----------------------------

    // acceso aleatorio
    size_type size() const {return m_g*n_g;}
    size_type rows() const {return m_g;}
    size_type cols() const {return n_g;}

    size_type rows_por_pixel() const {return m;}
    size_type cols_por_pixel() const {return n;}

    // (i, j) = índices de la región
    alp::Submatriz<ContenedorB> operator()(size_type i, size_type j)
	{return alp::Submatriz<ContenedorB>{*c, I(i),J(j), m, n};}

    const alp::Submatriz<const ContenedorB> operator()(size_type i, size_type j) const 
	{return alp::Submatriz<const ContenedorB>{*c, I(i),J(j), m, n};}


    template<typename C>
    friend std::ostream& operator<<(std::ostream& out, const Grid<C>& c);

private:
    ContenedorB* c; // contenedor donde colocamos la máscara
    int m, n;	    // dimensiones de cada región del grid
    int m_g, n_g;   // dimensiones del grid (rows, cols)


    // (i,j) --> (I,J) = posición de la esquina superior izquierda de cada
    // región en que dividimos el contenedor
    size_type I(size_type i) const {return i*m;}
    size_type J(size_type j) const {return j*n;}
};

template<typename C>
inline std::ostream& operator<<(std::ostream& out, const Grid<C>& c)
{
    out << "GRID:\n";
    out << "-----\n";
    out << "Dimensiones del grid: " << c.m_g << " filas x " << c.n_g << " columnas\n";
    out << "Cada pixel del grid tiene: " << c.m << " filas x " << c.n << " columnas pixeles de la imagen\n";
    out << "Puntero c: \n" << c.c << "\n\n";
    out << "Contenido de c: \n";
    out << *c.c;


    return out;
}

// funciones auxiliares para crear Grids
template<typename ContenedorB>
Grid<ContenedorB> grid(ContenedorB& c, int m, int n)
{return Grid<ContenedorB>{c, m, n};}

template<typename ContenedorB>
Grid<const ContenedorB> grid(const ContenedorB& c, int m, int n)
{return Grid<const ContenedorB>{c, m, n};}


/*****************************************************************************
 * 
 *   - CLASE: Grid_d
 *
 *   - DESCRIPCIÓN: Le ponemos un grid a una imagen aumentando cada pixel
 *	en m x n. Lo que hacemos es dividir cada pixel en m x n componentes
 *	(de ahí viene la 'd'). Las dimensiones del grid son m_g x n_g
 *
 *	Es un grid genérico que le podemos poner a cualquier ContenedorB.
 *
 *   - HACER?:
 *	    Las diferencias entre Subimage y Grid_d son únicamente las funciones
 *	    de acceso aleatorio, ya que todos los iteradores se implementan a
 *	    partir de estas funciones. Se puede heredar estas dos clases de
 *	    una clase genérica 'Mascara' que incluya estos iteradores.
 *
 ***************************************************************************/
template<typename ContenedorB>
class Grid_d{
public:
    using value_type	= alp::Value_type<ContenedorB>;
    using reference	= std::result_of_t<ContenedorB(int, int)>;

    using size_type	= typename ContenedorB::size_type;

    using iterator	 = alp::Iterator_rnd_2D<Grid_d>;
    using const_iterator = alp::Iterator_rnd_2D<const Grid_d>;

    using Fila		= alp::Fila<iterator>;
    using const_Fila	= alp::Fila<const_iterator>;

    using it_fila	= alp::It_fila_rnd<Grid_d>;
    using const_it_fila	= alp::It_fila_rnd<const Grid_d>;


    // (I0, J0) = esquina superior izquierda
    // m x n = dimensiones de cada región en la que dividimos la imagen
    Grid_d(ContenedorB& vc, int vm, int vn)
	:c(&vc), m{vm}, n{vn}
    {
	m_g = c->rows()*m;
	n_g = c->cols()*n;
    }


    // ----------------------
    // Parte común con Región
    // ----------------------
    // acceso como contenedor unidimensional
    iterator begin() {return iterator{*this, 0, 0};}
    iterator end()   {return iterator{*this, rows(), 0};}
    iterator it(int i, int j) {return iterator{*this, i, j};}

    const_iterator begin() const {return const_iterator{*this, 0, 0};}
    const_iterator end()   const {return const_iterator{*this, rows(), 0};}
    const_iterator it(int i, int j) const {return const_iterator{*this, i, j};}

    // acceso bidimensional
    Fila fila(int i) {return Fila{iterator{*this, i, 0}
				, iterator{*this, i+1, 0}, cols()};}

    const_Fila fila(int i) const {return const_Fila{const_iterator{*this, i, 0}
				, const_iterator{*this, i+1, 0}, cols()};}

    it_fila row_begin() {return it_fila{*this, 0};}
    it_fila row_end() {return it_fila{*this, rows()};}

    const_it_fila row_begin() const {return const_it_fila{*this, 0};}
    const_it_fila row_end() const {return const_it_fila{*this, rows()};}

    // ----------------------------
    // Fin - parte común con Región
    // ----------------------------

    // acceso aleatorio
    size_type size() const {return m_g*n_g;}
    size_type rows() const {return m_g;}
    size_type cols() const {return n_g;}

    // (i, j) = índices de la región
    // En este grid NO se puede escribir!!! Escribir en el punto (i,j)
    // supondría escribir en todo el pixel (I,J)!!!
//    reference operator()(size_type i, size_type j)
//	{return c(I(i),J(j));}

    const reference operator()(size_type i, size_type j) const 
	{return (*c)(I(i),J(j));}

    // de información del Grid_d
    size_type rows_por_pixel() const{return m;}
    size_type cols_por_pixel() const{return n;}

    template<typename C>
    friend std::ostream& operator<<(std::ostream& out, const Grid_d<C>& c);

private:
    ContenedorB* c; // contenedor donde colocamos la máscara
    int m, n;	    // dimensiones de cada región del grid
    int m_g, n_g;   // dimensiones del grid (rows, cols)

    // (i,j) --> (I,J) = pixel de la imagen al que pertenece (i,j)
    // región en que dividimos el contenedor
    size_type I(size_type i) const {return int(i/m);}
    size_type J(size_type j) const {return int(j/n);}
};

template<typename C>
inline std::ostream& operator<<(std::ostream& out, const Grid_d<C>& c)
{
    out << "GRID_D:\n";
    out << "-------\n";
    out << "Dimensiones del grid: " << c.m_g << " filas  x " << c.n_g << " columnas\n";
    out << "Cada pixel lo dividimos en: " << c.m << " filas x " << c.n << " columnas\n";
    out << "Puntero c = " << c.c << "\n\n";

    out << "Contenido de c: \n";
    out << *c.c;

    return out;
}

} // namespace

#endif

