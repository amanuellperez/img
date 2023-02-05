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

#ifndef __IMG_ITERATOR2D_H__
#define __IMG_ITERATOR2D_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Iteradores de 2D (4 direcciones)
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 26/08/2017 Escrito
 *
 ****************************************************************************/
#include "img_image.h"

namespace img{
/*!
 *  \brief  Direcciones en las que nos podemos mover.
 *
 */
enum class Direccion {
    E, NE, N, NW, W, SW, S, SE,
    dcha   = E, izda   = W, arriba = N, abajo = S
};


/// Devolvemos una string identificando la dirección 'd'
inline std::string to_str(Direccion d)
{
    switch(d){
	case Direccion::E: return "E";
	case Direccion::NE: return "NE";
	case Direccion::N: return "N";
	case Direccion::NW: return "NW";
	case Direccion::W: return "W";
	case Direccion::SW: return "SW";
	case Direccion::S: return "S";
	case Direccion::SE: return "SE";
    }

}



/*!
 *  \brief  Una dirección es un vector (x,y) con x,y pertenecientes {+-1,0}
 *
 *	Solamente tenemos 8 posibles direcciones:
 *		    N, S, E, W, NE, NW, SE, SW
 *
 *	Doy otra forma de hablar: dcha, izda, arriba y abajo.
 */
class Vector_direccion{
public:
    // Constructores
  static constexpr Vector_direccion E();
  static constexpr Vector_direccion NE();
  static constexpr Vector_direccion N();
  static constexpr Vector_direccion NW();
  static constexpr Vector_direccion W();
  static constexpr Vector_direccion SW();
  static constexpr Vector_direccion S();
  static constexpr Vector_direccion SE();

  /// Gira la dirección 45 grados en sentido positivo (o negativo si es
  /// false). Sentido positivo es el definido matemáticamente para los
  /// ángulos (polares con el eje x con eje polar).
  void gira_45(bool sentido_positivo);

  /// Gira la dirección 90 grados en sentido positivo (o negativo si es
  /// false).
  void gira_90(bool sentido_positivo);

  /// Devuelve el tipo de la dirección
  Direccion direccion() const { return direccion_; }

  // Sería mejor devolver Position, pero todavía tengo el lío con las
  // coordenadas: (i,j)??? (x,y)??? 
  int x() const {return x_;}
  int y() const {return y_;}

  /// ¿a == b?
  friend bool operator==(const Vector_direccion& x, const Vector_direccion& y);


private:
    int x_, y_;	// Dirección (x,y)
    Direccion direccion_; // memorizo el tipo para poder girar rápidamente las
		    // direcciones. En el algoritmo de la muralla es lo que
		    // voy a hacer todo el rato. Quiero que sea eficiente el
		    // giro!!!
    
    // Funciones de ayuda
    // Definimos una nueva dirección.
    // Precondición: x, y solo puede valer -1, 0 ó 1.
    constexpr Vector_direccion(int x, int y, Direccion d);

    void gira_45_sentido_mas();
    void gira_45_sentido_menos();

};


inline constexpr Vector_direccion Vector_direccion::E()
{ return Vector_direccion{1, 0, Direccion::E}; }

inline constexpr Vector_direccion Vector_direccion::NE()
{ return Vector_direccion{1, 1, Direccion::NE}; }

inline constexpr Vector_direccion Vector_direccion::N()
{ return Vector_direccion{0, 1, Direccion::N}; }

inline constexpr Vector_direccion Vector_direccion::NW()
{ return Vector_direccion{-1, 1, Direccion::NW}; }

inline constexpr Vector_direccion  Vector_direccion::W()
{ return Vector_direccion{-1, 0, Direccion::W}; }

inline constexpr Vector_direccion Vector_direccion::SW()
{ return Vector_direccion{-1, -1, Direccion::SW}; }

inline constexpr Vector_direccion  Vector_direccion::S()
{ return Vector_direccion{0, -1, Direccion::S}; }

inline constexpr Vector_direccion Vector_direccion::SE()
{ return Vector_direccion{1, -1, Direccion::SE}; }


inline constexpr Vector_direccion::Vector_direccion(int x, int y, Direccion d)
    : x_{x}, y_{y}, direccion_{d}
{ }



// Gira la dirección 45 grados en sentido horario o antihorario.
inline void Vector_direccion::gira_45(bool sentido_positivo)
{
    if (sentido_positivo)
	gira_45_sentido_mas();
    else
	gira_45_sentido_menos();
}


// Gira la dirección 90 grados en sentido horario o antihorario.
inline void Vector_direccion::gira_90(bool sentido_positivo)
{
    // TODO: si se ve que uso esto, escribir gira_90_sentido_mas/menos.
    // Son más eficientes que llamar 2 veces a gira_45.
    gira_45(sentido_positivo);
    gira_45(sentido_positivo);
//    if (sentido_positivo)
//	gira_90_sentido_mas();
//    else
//	gira_90_sentido_menos();
}





/// ¿x == y?
inline bool operator==(const Vector_direccion& x, const Vector_direccion& y)
{ return x.direccion_ == y.direccion_;}

inline bool operator!=(const Vector_direccion& x, const Vector_direccion& y)
{ return !(x == y);}




inline std::ostream& operator<<(std::ostream& out, const Direccion& d)
{ return out << to_str(d); }



/*!
 *  \brief  Iterador de 4 direcciones.
 *
 *  Permiten moverse por la imagen en todas direcciones. A partir de esta
 *  clase se definen dos alias: Iterator2D y
 *  const_Iterator2D.
 *  
 *  Importante: estoy concibiendo la imagen como si la tuviera delante. Con lo
 *  que cuando decimos que se mueva a la derecha, es la derecha que yo veo.
 *
 *  Ejemplo:
 *  \code
 *	Image img;
 *	...
 *	Iterator2D p{img, Position{10,20}};
 *
 *	while (!p.esta_fuera()){
 *	    ++p;    // = p.a_la_dcha();
 *	    --p;    // = p.a_la_izda();
 *	    p.arriba();
 *	    p.abajo();
 *	}
 *
 *  \endcode 
 *
 *  Observad que al usar estos iteradores se necesitará un mapa para saber qué
 *  zonas de la imagen ya hemos recorrido o no.
 *
 */
template <typename Img>
class Iterator2D_t{
public:
    // Tipos necesarios
    using Color_t = alp::Value_type<Img>;

    // Tipos necesarios por iterator_traits
    using value_type        = Color_t;
    using pointer           = value_type*;
    using reference         = value_type&;
    using difference_type   = std::ptrdiff_t; // ???
//    using iterator_category = std::forward_iterator_tag; <-- es
//    iterator2D_tag!!!


    /// Define el iterador 2D, colocándolo en la posición 'p' sobre la imagen
    /// img0.
    Iterator2D_t(Img& img0, Position p = {0,0})
	:img_{&img0}, p_{p} {}

    Iterator2D_t& operator++() {return dcha();}
    Iterator2D_t& operator--() {return izda();}

    /// Movemos el iterador hacia la derecha. Si nos salimos de la imagen
    /// este iterador no se podrá dereferenciar. Es responsabilidad del 
    /// usuario de la clase validar que no nos hayamos salido de la imagen.
    Iterator2D_t& dcha() {++p_.j; return *this;}
    Iterator2D_t& izda() {--p_.j; return *this;}

    Iterator2D_t& arriba() {--p_.i; return *this;}
    Iterator2D_t& abajo()  {++p_.i; return *this;}


    // Como estoy experimentando y la idea es explorar la imagen con este tipo
    // de iteradores suministro estas funciones. ¿Serán útiles?
    Iterator2D_t& N() {return arriba();}
    Iterator2D_t& S() {return abajo();}
    Iterator2D_t& E() {return dcha();}
    Iterator2D_t& W() {return izda();}

    // NW = noroeste, NE = noreste, SW = suroeste, SE = sureste
    Iterator2D_t& NW() { N(); return W();}
    Iterator2D_t& NE() { N(); return E();}
    Iterator2D_t& SW() { S(); return W();}
    Iterator2D_t& SE() { S(); return E();}


    /// Movemos el iterador en la dirección indicada
    void muevete(const Vector_direccion& d);


    /// Posición que apuntamos en la imagen
    Position posicion() const {return p_;}


    Color_t& operator*() const {return (*img_)(p_);}
    Color_t* operator->() const {return &*(*this);}

    friend bool operator==(Iterator2D_t a,
                           Iterator2D_t b)
    { return a.p_ == b.p_;}

    bool esta_dentro() const { return pertenece(p_, *img_);}
    bool esta_fuera() const  {return !esta_dentro();}

    bool puedo_ir_a_la_dcha() {return p_.j < img_->cols() - 1;}
    bool puedo_ir_a_la_izda() {return p_.j > 0;}

    bool puedo_ir_arriba() {return p_.i > 0;}
    bool puedo_ir_abajo() {return p_.i < img_->rows() - 1;}


    /// Indica si el iterador puede moverse en la dirección d 
    bool puedo_moverme(const Vector_direccion& d)
    {
	auto p = *this;
	p.muevete(d);
	return p.esta_dentro();
    }

private:
    Img* img_;	    // imagen por donde nos movemos
    Position p_;    // posición a la que apuntamos
};


template <typename Img>
void Iterator2D_t<Img>::muevete(const Vector_direccion& d)
{
    p_.j += d.x();
    p_.i -= d.y();
}


template <typename I>
bool operator!=(Iterator2D_t<I> a,
                Iterator2D_t<I> b)
{
    return !(a == b);
}


using Iterator2D = Iterator2D_t<Image>;

using const_Iterator2D = Iterator2D_t<const Image>;


}// namespace

#endif


