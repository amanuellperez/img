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


/****************************************************************************
 *
 *   - DESCRIPCION: Funciones que dependen de paquetes externos.
 *	Funciones para leer y escribir una imagen en un fichero
 *
 *   - COMENTARIOS: Se basan en el paquete CImg.
 *	    El único fichero que depende de CImg es este, no habiendo más 
 *	    dependencias.
 *
 *   - HISTORIA:
 *           alp  - 25/06/2016 Escrito
 *
 ****************************************************************************/
// Tell CImg not to use display capabilities
#undef cimg_display
#define cimg_display 0

#include <string>
#include <filesystem>


#include "CImg.h"
#include <alp_cast.h>
#include <alp_exception.h>
#include <sstream>

#include "img_imagen.h"


using namespace cimg_library;
using namespace std;
using namespace alp;


namespace img{

Imagen to_imagen(const cimg_library::CImg<unsigned char>& m)
{
    Imagen img{narrow_cast<Imagen::Ind>(m.height())
		, narrow_cast<Imagen::Ind>(m.width())};

    auto p = img.begin();

    for(int y=0 ; y != m.height(); ++y)
	for(int x = 0; x != m.width(); ++x)
    {
	(*p) = ColorRGB{*m.data(x,y,0,0), *m.data(x,y,0,1), *m.data(x,y,0,2)};
	++p;
    }
    
    return img;
}

// precondition: m solo tiene 1 canal (es blanca/negra)
static Imagen to_imagen_1canal(const cimg_library::CImg<unsigned char>& m)
{
    Imagen img{narrow_cast<Imagen::Ind>(m.height())
		, narrow_cast<Imagen::Ind>(m.width())};

    auto p = img.begin();

    for(int y=0 ; y != m.height(); ++y)
	for(int x = 0; x != m.width(); ++x)
    {
	(*p) = ColorRGB{*m.data(x,y,0,0), *m.data(x,y,0,0), *m.data(x,y,0,0)};
	++p;
    }
    
    return img;
}




// DEPENDE DE: CImg!!!
Imagen read(const std::string& name)
{
    if (!std::filesystem::is_regular_file(name))
	throw File_not_found{name};

    std::stringstream error;

    try{
    cimg_library::CImg<unsigned char> img(name.c_str());

    if (img.spectrum() == 3)
	return to_imagen(img);

    if (img.spectrum() == 1)
	return to_imagen_1canal(img);

    error << "No se trata de una imagen RGB!!!\nNo tiene 3 canales, sino ["
          << img.spectrum() << "] canal\n"
	  << "Dimensiones: ancho = [" << img.width()
          << "]; alto = [" << img.height() << "]\n";

    }catch(...)
    {
	throw File_cant_read{name};
    }

    throw std::runtime_error{error.str()};

    return Imagen{1,1};  // para que no de warning el compilador
}


void write(const Imagen& img, const std::string& name)
{
    // Todas las imagenes que uso son RGB, 3 canales!
    CImg<unsigned char> m{narrow_cast<unsigned int>(img.cols())
	    , narrow_cast<unsigned int>(img.rows()), 1, 3};

    auto p = img.begin();

    for(int y=0 ; y != m.height(); ++y)
	for(int x = 0; x != m.width(); ++x)
    {
	*m.data(x,y,0,0) = narrow_cast<unsigned char>((*p).r);
	*m.data(x,y,0,1) = narrow_cast<unsigned char>((*p).g);
	*m.data(x,y,0,2) = narrow_cast<unsigned char>((*p).b);

	++p;
    }

    m.save(name.c_str());
}

}

