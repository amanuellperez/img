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


#ifndef __IMG_COLOR_H_
#define __IMG_COLOR_H_
/****************************************************************************
 *
 *   - DESCRIPCION: Colores usados por img.
 *
 *   - COMENTARIOS: Un color lo podemos escribir en diferentes coordenadas:
 *		ColorRGB = (r,g,b)
 *
 *   - HISTORIA:
 *    Manuel Perez
 *      04/12/2015 Escrito
 *      28/01/2016 Añado sistemas de coordenadas ColorXYZ,...
 *      28/02/2016 Elimino Color (lo dejo todo como int)
 *      09/07/2017 Elimino colores que no uso.
 *      22/08/2017 Aquí solo dejo ColorRGB. El paquete img
 *	           es un paquete genérico que no sabe de VISAR.
 *	28/05/2020 Aproximado
 *	31/05/2020 Redefino intensidad.
 *	07/12/2020 std::numeric_limits<Color>
 *	01/09/2022 ncolors
 *
 ****************************************************************************/

#include <limits>
#include <alp_iterator.h>
#include <alp_math.h>
#include <limits>



namespace img{
// TODO: definir un tipo Color. Es un tipo numérico.
class Color 
{ 
public:
    // tipos de colores (añadir: intensidad, value, hue y saturate?)
    enum Tipo{red, green, blue};

};
} // namespace img


template<>
struct std::numeric_limits<img::Color>{
    static constexpr int min() noexcept {return 0;}
    static constexpr int max() noexcept {return 255;}

    static constexpr const int num_colores = 256;
};


namespace img{
inline bool is_color(int c)
{ return (alp::esta_entre(c, std::numeric_limits<img::Color>::min(), 
			     std::numeric_limits<img::Color>::max())); }


/*****************************************************************************
 * 
 *   - CLASE: ColorRGB
 *
 *   - DESCRIPCIÓN: Un color RGB es un vector (r,g,b), donde cada coordenada
 *	    es un entero. NO IMPONER RESTRICCIONES, ya que voy a operar.
 *	    Por ejemplo, voy a usar -c!!!
 *
 ***************************************************************************/
struct ColorRGB
{
    using value_type = int;
    static constexpr int ncolors = 3; // r,g,b

    value_type r, g, b;

    // TODO: este constructor por defecto lo necesito para crear una imagen
    // vacía (new ColorRGB[100]). ¿cómo puedo eliminarlo?
    // ¿Hacer friend Imagen y privado este constructor?
    // TODO: una forma de resolver este problema es obligar de forma explícita
    // al cliente a llamar a este constructor. Para evitar que se llame sin
    // querer. Hacer este constructor privado, y una función static del tipo:
    // crea(); // devuelve un puntero a un ColorRGB
    // crea(100); // puntero a un array (???) de ColorRGB
    // Si se quiere construir un ColorRGB localmente, usar nulo();
    ColorRGB(){}    // CUIDADO: no se inicializan r, g, b

    constexpr ColorRGB(int R, int G, int B) noexcept 
	: r{R}, g{G}, b{B} {}


    // operaciones (para algoritmos)
    ColorRGB& operator+=(const ColorRGB& c);
    ColorRGB& operator-=(const ColorRGB& c);

    ColorRGB operator-() const;


    // Los 12 colores del hexágono. Dependen del tamaño del color!!!
    static constexpr ColorRGB azul()	    {return ColorRGB{  0,  0, 255};}	//  0º
    static constexpr ColorRGB azul1()	    {return ColorRGB{  0,  0, 200};}
    static constexpr ColorRGB azul2()	    {return ColorRGB{  0,  0, 150};}
    static constexpr ColorRGB azul3()	    {return ColorRGB{  0,  0, 100};}

    static constexpr ColorRGB morado()	    {return ColorRGB{127,  0, 255};}	// 30º

    static constexpr ColorRGB rosa_palido() {return ColorRGB{255,  0, 255};}	// 60º
    static constexpr ColorRGB rosa()	    {return ColorRGB{255,  0, 127};}	// 90º

    static constexpr ColorRGB rojo()	    {return ColorRGB{255,  0,   0};}	//120º
    static constexpr ColorRGB rojo1()	    {return ColorRGB{200,  0,   0};}	//120º
    static constexpr ColorRGB rojo2()	    {return ColorRGB{150,  0,   0};}	//120º
    static constexpr ColorRGB rojo3()	    {return ColorRGB{100,  0,   0};}	//120º

    static constexpr ColorRGB naranja()	    {return ColorRGB{255,127,   0};}	//150º

    static constexpr ColorRGB amarillo()    {return ColorRGB{255,255,   0};}	//180º

    static constexpr ColorRGB verde_claro() {return ColorRGB{127,255,   0};}	//210º
    static constexpr ColorRGB verde_claro1() {return ColorRGB{127,200,   0};}
    static constexpr ColorRGB verde_claro2() {return ColorRGB{127,150,   0};}
    static constexpr ColorRGB verde_claro3() {return ColorRGB{127,100,   0};}

    static constexpr ColorRGB verde()	    {return ColorRGB{  0,255,   0};}	//240º
    static constexpr ColorRGB verde1()	    {return ColorRGB{  0,200,   0};}
    static constexpr ColorRGB verde2()	    {return ColorRGB{  0,150,   0};}
    static constexpr ColorRGB verde3()	    {return ColorRGB{  0,100,   0};}

    static constexpr ColorRGB turquesa()    {return ColorRGB{  0,255, 127};}	//270º

    static constexpr ColorRGB azul_cielo()  {return ColorRGB{  0,255, 255};}	//300º
    static constexpr ColorRGB azul_claro()  {return ColorRGB{  0,127, 255};}	//330º

    static constexpr ColorRGB negro(){return ColorRGB{0,0,0};}
    static constexpr ColorRGB gris(int g = 150){return ColorRGB{g,g,g};}
    static constexpr ColorRGB blanco() {return ColorRGB{255,255,255};}

};


inline ColorRGB& ColorRGB::operator+=(const ColorRGB& c)
{
    r += c.r;
    g += c.g;
    b += c.b;

    return *this;
}

inline ColorRGB& ColorRGB::operator-=(const ColorRGB& c)
{
    r -= c.r;
    g -= c.g;
    b -= c.b;

    return *this;
}

inline ColorRGB ColorRGB::operator-() const
{
    return ColorRGB{-r, -g, -b};
}

// ColorRGB es un vector, operamos con el como los vectores
// OJO: al operar podemos acabar con colores que se salen del cubo (<255 ó <
// 0)
inline ColorRGB operator+(const ColorRGB& a, const ColorRGB& b) 
{ 
    ColorRGB c{a};
    return c+=b;
}

inline ColorRGB operator-(const ColorRGB& a, const ColorRGB& b)
{
    ColorRGB c{a};
    return c-=b;
}

inline ColorRGB operator*(const ColorRGB& a, int n)
{return ColorRGB{a.r*n, a.g*n, a.b*n};}


inline ColorRGB operator*(int n, const ColorRGB& a)
{return a*n;}

inline ColorRGB operator/(const ColorRGB& a, int n)
{return ColorRGB{a.r/n, a.g/n, a.b/n};}

/// Como ando experimentando, voy a operar con los colores. El resultado
/// pudiera estar fuera del cubo de color. Esta función nos dice si 'c' es
/// un color válido o no.
inline bool is_color(const ColorRGB& c) 
{return (img::is_color(c.r) && img::is_color(c.g) && img::is_color(c.b));}

/// ¿'c' es negro?
inline bool es_negro(const ColorRGB& c) 
{return ((c.r==0)&&(c.g==0)&&(c.b==0));}



inline bool son_continuos(const ColorRGB& c1, const ColorRGB& c2)
{
    return (abs(c1.r - c2.r) <= 1)
	and (abs(c1.g - c2.g) <= 1)
	and (abs(c1.b - c2.b) <= 1);
}

inline bool son_contiguos(const ColorRGB& c1, const ColorRGB& c2)
{return son_continuos(c1,c2);}


inline bool son_discontinuos(const ColorRGB& c1, const ColorRGB& c2)
{ return !son_continuos(c1, c2); }

inline bool son_distintos_t(const ColorRGB& c1, const ColorRGB& c2)
{return c1.r != c2.r and c1.g != c2.g and c1.b != c2.b;}

inline constexpr int intensidad(const ColorRGB& c) {return (c.r +c.g +c.b)/3;}
inline constexpr int intensidad_max() {return intensidad(ColorRGB{255,255,255});}


/// Escribimos un color en formato txt
std::ostream& operator<<(std::ostream& out, const ColorRGB& c);

/// Leemos un color en formato txt
std::istream& operator>>(std::istream& in, ColorRGB& c);

inline bool operator==(const ColorRGB& a, const ColorRGB& b)
{ return ((a.r == b.r) && (a.g == b.g) && (a.b == b.b)); }

inline bool operator!=(const ColorRGB& a, const ColorRGB& b)
{return !(a == b);}


// Para sacar estadísticas ordeno los colores de una imagen. Para ello,
// necesito un operador <. Defino este comparador por pisos.
inline bool operator<(const img::ColorRGB& a, const img::ColorRGB& b)
{ return 
	(a.b < b.b) 
    or	(a.b == b.b and (	 (a.g < b.g) 
			      or (a.g == b.g and a.r < b.r))
	)
    ; }


/***************************************************************************
 *			PROPIEDADES DE LOS COLORES
 ***************************************************************************/
/*!
 *  \brief  Propiedad: Color_red{}(ColorRGB c) = c.r
 *
 *  Devuelve el color red del ColorRGB.
 *
 */
struct Color_red {
    int& operator()(ColorRGB& c) const { return c.r; }
    const int& operator()(const ColorRGB& c) const { return c.r; }
};

struct const_Color_red {
    const int& operator()(const ColorRGB& c) const { return c.r; }
};


/*!
 *  \brief  Functor: Color_green{}(ColorRGB c) = c.r
 *
 *  Devuelve el color green del ColorRGB.
 */
struct Color_green {
    int& operator()(ColorRGB& c) const { return c.g; }
    const int& operator()(const ColorRGB& c) const { return c.g; }
};

struct const_Color_green {
    const int& operator()(const ColorRGB& c) const { return c.g; }
};


/*!
 *  \brief  Propiedad: Color_blue{}(ColorRGB c) = c.r
 *
 *  Devuelve el color blue del ColorRGB.
 */
struct Color_blue {
    int& operator()(ColorRGB& c) const { return c.b; }
    const int& operator()(const ColorRGB& c) const { return c.b; }
};

struct const_Color_blue {
    const int& operator()(const ColorRGB& c) const { return c.b; }
};




/***************************************************************************
 *			    ITERADORES DE COLOR
 * ------------------------------------------------------------------------
 *
 *  En ocasiones, cuando itero por una imagen, no siempre estoy interesado
 *  en ver el ColorRGB del punto, sino solo el red o el green o el blue.
 *  Suministro estos iteradores para poder iterar por una imagen operando
 *  en cada color de forma independiente.
 *
 ***************************************************************************/
/// Syntactic sugar: convierte el iterador de color 'p' en un iterador que
/// devuelve solo el red. Versión no const.
template <typename It>
inline auto iterator_red(It p)
{
    return alp::view_iterator(p, Color_red{});
}

/// Syntactic sugar: convierte el iterador de color 'p' en un iterador que
/// devuelve solo el red. Versión const.
template <typename It>
inline auto const_iterator_red(It p)
{
    return alp::const_view_iterator(p, const_Color_red{});
}


/// Syntactic sugar: convierte el iterador de color 'p' en un iterador que
/// devuelve solo el green. Versión no const.
template <typename It>
inline auto iterator_green(It p)
{
    return alp::view_iterator(p, Color_green{});
}

/// Syntactic sugar: convierte el iterador de color 'p' en un iterador que
/// devuelve solo el green. Versión const.
template <typename It>
inline auto const_iterator_green(It p)
{
    return alp::const_view_iterator(p, const_Color_green{});
}



/// Syntactic sugar: convierte el iterador de color 'p' en un iterador que
/// devuelve solo el blue. Versión no const.
template <typename It>
inline auto iterator_blue(It p)
{
    return alp::view_iterator(p, Color_blue{});
}

/// Syntactic sugar: convierte el iterador de color 'p' en un iterador que
/// devuelve solo el blue. Versión const.
template <typename It>
inline auto const_iterator_blue(It p)
{
    return alp::const_view_iterator(p, const_Color_blue{});
}



}// namespace img


template <>
struct alp::Aproximado<img::ColorRGB>{
    using Int = img::ColorRGB::value_type;
    Aproximado<Int> aprox;

    Aproximado(Int eps0) : aprox{eps0} { }

    bool operator()(const img::ColorRGB& c1, const img::ColorRGB& c2)
    {
	return aprox(c1.r, c2.r) and
	       aprox(c1.g, c2.g) and
	       aprox(c1.b, c2.b);
    }
};


// --------------------------
// Traits de Imagen::iterator
// --------------------------
template<>
struct std::iterator_traits<img::ColorRGB*>{
    using difference_type = std::ptrdiff_t;
    using value_type      = img::ColorRGB;
    using pointer         = img::ColorRGB*;
    using reference       = img::ColorRGB&;
    using iterator_category = std::random_access_iterator_tag;
};


template<>
struct std::iterator_traits<const img::ColorRGB*>{
    using difference_type = std::ptrdiff_t;
    using value_type      = img::ColorRGB;
    using pointer         = const img::ColorRGB*;
    using reference       = const img::ColorRGB&;
    using iterator_category = std::random_access_iterator_tag;
};




#endif

