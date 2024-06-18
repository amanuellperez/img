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


#ifndef __IMG_ESCALA_H__
#define __IMG_ESCALA_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones para escalar imágenes
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *	   Manuel Perez - 26/07/2016 Escrito
 *
 ****************************************************************************/

#include "img_image.h"

namespace img{

/*****************************************************************************
 * 
 *   - CLASE: Num_filas
 *
 *   - DESCRIPCIÓN: Estructura para poder seleccionar fácilmente qué método
 *	de escalado queremos:
 *		escala(img, Num_filas{400});
 *		escala(img, Num_columnas{400});
 *		escala(img, Razon_semejanza{400});
 *
 ***************************************************************************/
// TODO: Esta idea está bien. Crear los tipos adecuados.
// m = número de filas a las que queremos escalar la imagen
// EJEMPLO: auto img = escala(img0, Num_filas{480});   // img tiene 480 filas
//class Num_filas: public alp::Nombre<int>{using Nombre::Nombre;};
//class Num_columnas: public alp::Nombre<int>{using Nombre::Nombre;};
using Num_filas = int;
using Num_columnas = int;



// escalamos la imagen a las dimensiones n_ancho, n_alto
// manteniendo la relación de aspecto
Image escala(const Image& img0, int n_ancho, int n_alto);

// escala: reduce o amplia la imagen de tal manera que el resultado 
// tenga nf filas
//// EJEMPLO: auto img = escala(img0, Num_filas{480});   // img tiene 480 filas
Image escala(const Image& img0, Num_filas nf);
Image reduce(const Image& img0, Num_filas nf);
Image amplia(const Image& img0, Num_filas nf);



/*****************************************************************************
 * 
 *   - CLASE: Escalador
 *
 *   - DESCRIPCIÓN: Clase temporal, hasta decidir si creo Image_escalada
 *	ó Region_escalada y cómo hacerlo.
 *	En esta clase voy metiendo funciones asociadas a estas clases.
 *
 *	OJO: la uso en gui_visor!!!
 *
 ***************************************************************************/
class Escalador{
public:
    // OJO: tiene que ser signed!!! Mirar uso que es  = -1!!!!
    using Ind = Image::Ind;


    Escalador():img0_{nullptr}{}
    
    Image escala(const Image& img0, int ancho, int alto)
    {img0_ = &img0; return escala(ancho, alto);}
    
    // Escala la imagen a las dimensiones indicadas
    Image escala(int ancho, int alto);


    // tipos de índices que usamos:
    //	+ global = (I, J): se refieren a la posición de un pixel en la img0
    //	+ local  = (i, j): se refieren a la posición de un pixel en la región
    //	+ local escalado = (ie, je): se refieren a la posición de un pixel
    //				     en la región escalada.
//    class Ind_global:public alp::Nombre<int>{using Nombre::Nombre;};
//    class Ind_local:public alp::Nombre<int>{using Nombre::Nombre;};
//    class Ind_local_escalado:public alp::Nombre<int>{using Nombre::Nombre;};
    using Ind_global = int;
    using Ind_local = int;
    using Ind_local_escalado = int;

    // funciones de conversión de un tipo de índice a otro
    // Convierte los índices escalados (ie,je) en índices locales.
    Ind i(Ind ie) const
    {return ie_to_i(ie, pi0_, pie_);}

    Ind j(Ind je) const
    {return ie_to_i(je, pj0_, pje_);};

    Position local_escalado_to_local(Position p) const
    {return Position{i(p.i), j(p.j)};}

private:
    const Image* img0_;	// podemos enlazar el escalador a otra imagen

    int pi0_ = -1, pj0_ = -1;    // número de pixeles que tiene img0 en img_mcm
    int pie_ = -1, pje_ = -1;    // número de pixeles que tiene imge en img_mcm

    // Funciones de ayuda
    static Ind ie_to_i(Ind ie, Ind p0, Ind pe);
};


// Convierte un índice local escalado en un índice local: ie -> i
inline int Escalador::ie_to_i(Ind ie, Ind p0, Ind pe)
{
    int i1 = int((ie*pe)/p0);
    int i2 = int(((ie+1)*pe-1)/p0);

    return (i1+i2)/2;
}




}// namespace img

#endif


