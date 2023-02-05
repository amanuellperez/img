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


/****************************************************************************
 *
 *   - DESCRIPCION: Funciones necesarias para escalar una imagen.
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 23/07/2016 Escrito
 *
 ****************************************************************************/
#include <iostream>

#include <alp_cast.h>

#include "img_image.h"
#include "img_escala.h"

using namespace std;
using namespace alp;

namespace img{

Image Escalador::escala(int ancho, int alto)
{
    int m0    = img0_->rows();
    int n0    = img0_->cols();

    Image imge = img::escala(*img0_, ancho, alto);

    int me = imge.rows();   int ne = imge.cols();

    int M = alp::min_comun_multiplo(m0, me);
    int N = alp::min_comun_multiplo(n0, ne);

    pi0_ = int(M/m0);    pie_ = int(M/me);
    pj0_ = int(N/n0);    pje_ = int(N/ne);
    
    return imge;
}


/****************************************************************************
 *
 *   - FUNCIÓN: escala
 *
 *   - DESCRIPCIÓN: Escalamos la imagen a las dimensiones (v_ancho, v_alto)
 *	manteniendo la relación de aspecto
 *
 ****************************************************************************/
Image escala(const Image& img0, int v_ancho, int v_alto)
{
    auto ancho = img0.cols();
    auto alto = img0.rows();
    
    double ka = narrow_cast<double>(v_ancho)/narrow_cast<double>(ancho);
    double kh = narrow_cast<double>(v_alto)/narrow_cast<double>(alto);

    double k = std::min(ka, kh);

    return escala(img0, Num_filas{narrow_cast<int>(alto*k)});
}


/****************************************************************************
 *
 *   - FUNCIÓN: escala
 *
 *   - DESCRIPCIÓN: Escalamos la imagen img0 pasando a tener m filas.
 *	    Mantiene la resolución de aspecto, calculando el número
 *	    de columnas adecuado de la imagen escalada.
 *
 *   - EJEMPLO:
 *		Image img0{"01.jpg"};
 *
 *		// escalamos la imagen a 480 filas 
 *		auto img_escalada = escala(img0, Num_filas{480});
 *
 ****************************************************************************/
Image escala(const Image& img0, Num_filas nf)
{
    if(img0.rows() == nf) return img0;

    if (img0.rows() > nf) return reduce(img0, nf);
    else		    return amplia(img0, nf);
}


/****************************************************************************
 *
 *   - FUNCIÓN: reduce
 *
 *   - DESCRIPCIÓN: Ampliamos la imagen
 *
 ****************************************************************************/
namespace red{// reduce
class Indice_y_tamagno{
public:
    Indice_y_tamagno(int p0, int p1):p0_{p0}, p1_{p1}{}

    void run(int i0);

    int i1() const{return i1_;}
    int i2() const{return i2_;}

    int j1() const{return i1();}
    int j2() const{return i2();}

    int t1() const{return t1_;}
    int t2() const{return t2_;}

    bool es_valido_i2() const {return (i2_ != -1);}
    bool es_valido_j2() const {return es_valido_i2();}

private:
    int p0_, p1_;   // configuracion

    int i1_, i2_;
    int t1_, t2_;
};

inline void Indice_y_tamagno::run(int i0)
{
    int b = i0 * p0_;
    i1_   = int(b / p1_); // = cociente(b/p1)
    int r = b % p1_;      // = resto(b/p1)

    if (r != 0) {
        t1_ = p1_ - r;

        if (t1_ < p0_) {
            i2_ = i1_ + 1;
            t2_ = p0_ - t1_;

        } else { // si está dentro
            t1_ = p0_;
            i2_ = -1;
        }

    } else {
        t1_ = p0_;
        i2_ = -1; // no válido
                  //	t2_ = 0;
    }
}

inline ostream& operator<<(ostream& out, const Indice_y_tamagno& c)
{
    
    out << "i1 = " << c.i1();
    out << "; t1 = " << c.t1();
    out << "; i2 = " << c.i2();
    out << "; t2 = " << c.t2();
    return out;
}

} // namespace red


// Esta función solo la uso en escala. La defino privada a este módulo
// Inicializa la imagen a cero
// TODO: usar std::fill
static Image imagen_ceros(Image::Ind m, Image::Ind n)
{
    Image img{m, n};

    for(auto& x: img)
	x = ColorRGB{0,0,0};

    return img;
}


// POSIBLE MEJORA: si se necesita mejorar eficiencia se está recalculando
// continuamente los Indice_y_tamagno c de las columnas (cada vez que itero
// por una fila, recalculo todos estos índices y tamaños). Se podría memorizar
// en un vector al empezar el algoritmo y luego usarlo.
Image reduce(const Image& img0, Num_filas nf)
{
    // 1. dimensiones de la imagen
    int m0 = img0.rows();
    int n0 = img0.cols();

    int m1 = nf; 
    int n1 = narrow_cast<double>(m1)/narrow_cast<double>(m0)*n0;

    int M = min_comun_multiplo(m0, m1);
    int N = min_comun_multiplo(n0, n1);

    int pm0 = M / m0;
    int pn0 = N / n0;

    int pm1 = M / m1;
    int pn1 = N / n1;

    // 2. Escalamos
    Image img1 = imagen_ceros(m1, n1);

    red::Indice_y_tamagno f{pm0, pm1}; // fila
    red::Indice_y_tamagno c{pn0, pn1}; // columna

    for(int i0 = 0; i0 != m0; ++i0)
    {
	f.run(i0);
//	cout << "Fila[" << i0 << "]: " << f << endl;
	for(int j0 = 0; j0 != n0; ++j0)
	{
//	    print<Red>(cout, img1);
//	    cout << "------------------------\n";
	    c.run(j0);
//	    cout << "Columna[" << j0 << "]: " << c << endl;

	    img1(f.i1(), c.j1()) += f.t1()*c.t1()*img0(i0, j0);

	    if(c.es_valido_j2())
		img1(f.i1(), c.j2()) += f.t1()*c.t2()*img0(i0, j0);

	    if(f.es_valido_i2()){

		img1(f.i2(), c.j1()) += f.t2()*c.t1()*img0(i0, j0);

		if(c.es_valido_j2())
		    img1(f.i2(), c.j2()) += f.t2()*c.t2()*img0(i0, j0);

	    }

	}
    }

    // dividimos cada punto de img1 entre el num_pixeles que tiene de gmcm
    int num_pixeles = pm1*pn1;

    for(auto& p: img1)
	p = p/num_pixeles;

    return img1;
}


/****************************************************************************
 *
 *   - FUNCIÓN: amplia
 *
 *   - DESCRIPCIÓN: Hace un escalado de la imagen img0 (la hace más grande)
 *
 *   - NOTA: se podría fusionar la implementación de amplia y reduce en una
 *	misma función, aunque puede (??? habría que medir tiempos)  que el
 *	bucle for que hago aquí (en amplia) sea un poco más ineficiente. En
 *	reduce no hago ese bucle for.
 *
 ****************************************************************************/
namespace amp{// amplia
class Indice_y_tamagno{
public:
    Indice_y_tamagno(int p0, int p1)
	:p0_{p0}, p1_{p1}{}

    void run(int i0);

    int i(int k) const{return i1_+k;}
    int j(int k) const{return i(k);}

    // devuelve el tamaño del índice k
    int t(int k) const;

    int size() const {return size_;}

private:
    int p0_, p1_;   // configuracion

    int i1_;
    int size_;	// los índices asociados a i0 son: [i1_, i1_+size_)
    int r1_; // tamaño de la primera caja
    int c_;  // número de cajas que tiene i0 de tipo p1
    int r2_; // tamaño de la segunda caja

};

inline void Indice_y_tamagno::run(int i0)
{
    int b0 = i0*p0_;
    i1_ = int(b0/p1_);    // = cociente(b/p1)
    int incr_1 = b0%p1_;  // = resto(b/p1)
    if(incr_1 == 0) r1_ = 0;
    else	    r1_ = p1_-incr_1;

    c_ = int((p0_-r1_)/p1_); // = cociente((p0-r1)/p1)
    r2_ = (p0_-r1_)%p1_;    // = resto((p0-r1)/p1)
    
    size_ = c_;
    if(r1_ != 0) ++size_;
    if(r2_ != 0) ++size_;
}

inline int Indice_y_tamagno::t(int k) const
{
    if(r1_ != 0){
	if(k == 0) return r1_;
	if(k == size_-1){
	    if(r2_ != 0) return r2_;
	    else return p1_;
	}
	return p1_;
    }
    // if(r1 == 0)

    if(k == size_ - 1){
	// OJO: tengo que distinguir este caso por si acaso p1 fuera 
	// múltiplo de p0!!!
	if(r2_ != 0) return r2_;
	else return p1_;
    }

    return p1_;
}

//inline ostream& operator<<(ostream& out, const Indice_y_tamagno& c)
//{
//    
//    out << "i1 = " << c.i1();
//    out << "; t1 = " << c.t1();
//    out << "; i2 = " << c.i2();
//    out << "; t2 = " << c.t2();
//    return out;
//}
}

// POSIBLE MEJORA: si se necesita mejorar eficiencia se está recalculando
// continuamente los Indice_y_tamagno c de las columnas (cada vez que itero
// por una fila, recalculo todos estos índices y tamaños). Se podría memorizar
// en un vector al empezar el algoritmo y luego usarlo.
Image amplia(const Image& img0, Num_filas nf)
{
    // 1. dimensiones de la imagen
    int m0 = img0.rows(); int n0 = img0.cols();
    int m1 = nf; 
    int n1 = narrow_cast<double>(m1)/narrow_cast<double>(m0)*n0;

    int M = min_comun_multiplo(m0, m1);
    int N = min_comun_multiplo(n0, n1);

    int pm0 = M/m0; int pn0 = N/n0;
    int pm1 = M/m1; int pn1 = N/n1;

    // 2. Escalamos
    Image img1 = imagen_ceros(m1, n1);

    amp::Indice_y_tamagno f{pm0, pm1}; // fila
    amp::Indice_y_tamagno c{pn0, pn1}; // columna

    for(int i0 = 0; i0 != m0; ++i0)
    {
	f.run(i0);
//	cout << "Fila[" << i0 << "]: " << f << endl;
	for(int j0 = 0; j0 != n0; ++j0)
	{
//	    cout << "\nIndices " << Position{i0, j0};
//	    cout << "\n-------\n";
	    c.run(j0);
//	    cout << "Columna[" << j0 << "]: " << c << endl;
	    for(int ki = 0; ki != f.size(); ++ki)
		for(int kj = 0; kj != c.size(); ++kj){
		    img1(f.i(ki), c.j(kj)) += f.t(ki)*c.t(kj)*img0(i0, j0);
		    
//		    cout << Position(f.i(ki), f.j(kj)) 
//			<< "; " << Position(f.t(ki), c.t(kj)) << endl;
		}
//	    print<Red>(cout, img1);
//	    cout << "------------------------\n";
	}

    }

    // dividimos cada punto de img1 entre el num_pixeles que tiene de gmcm
    int num_pixeles = pm1*pn1;

    for(auto& p: img1)
	p = p/num_pixeles;

    return img1;
}


}// namespace img
