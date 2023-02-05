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

#include "img_image.h"


namespace cimg = cimg_library;


namespace img{

Image to_imagen(const cimg::CImg<unsigned char>& m)
{
    Image img{alp::narrow_cast<Image::Ind>(m.height())
		, alp::narrow_cast<Image::Ind>(m.width())};

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
static Image to_imagen_1canal(const cimg::CImg<unsigned char>& m)
{
    Image img{alp::narrow_cast<Image::Ind>(m.height())
		, alp::narrow_cast<Image::Ind>(m.width())};

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
Image read(const std::string& name)
{
    if (!std::filesystem::is_regular_file(name))
	throw alp::File_not_found{name};

    std::stringstream error;

    try{
    cimg::CImg<unsigned char> img(name.c_str());

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
	throw alp::File_cant_read{name};
    }

    throw std::runtime_error{error.str()};

    return Image{1,1};  // para que no de warning el compilador
}


void write(const Image& img, const std::string& name)
{
    // Todas las imagenes que uso son RGB, 3 canales!
    cimg::CImg<unsigned char> m{alp::narrow_cast<unsigned int>(img.cols())
	    , alp::narrow_cast<unsigned int>(img.rows()), 1, 3};

    auto p = img.begin();

    for(int y=0 ; y != m.height(); ++y)
	for(int x = 0; x != m.width(); ++x)
    {
	*m.data(x,y,0,0) = alp::narrow_cast<unsigned char>((*p).r);
	*m.data(x,y,0,1) = alp::narrow_cast<unsigned char>((*p).g);
	*m.data(x,y,0,2) = alp::narrow_cast<unsigned char>((*p).b);

	++p;
    }

    m.save(name.c_str());
}

}

