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


#include "img_iterator2D.h"

namespace img{


void Vector_direccion::gira_45_sentido_mas()
{
    switch(direccion_){
	case Direccion::E: 
	    y_ = 1;
	    direccion_ = Direccion::NE;
	    break;

	case Direccion::NE:
	    x_ = 0;
	    direccion_ = Direccion::N;
	    break;

	case Direccion::N:
	    x_ = -1;
	    direccion_ = Direccion::NW;
	    break;

	case Direccion::NW:
	    y_ = 0;
	    direccion_ = Direccion::W;
	    break;

	case Direccion::W:
	    y_ = -1;
	    direccion_ = Direccion::SW;
	    break;

	case Direccion::SW:
	    x_ = 0;
	    direccion_ = Direccion::S;
	    break;

	case Direccion::S:
	    x_ = 1;
	    direccion_ = Direccion::SE;
	    break;

	case Direccion::SE:
	    y_ = 0;
	    direccion_ = Direccion::E;
	    break;
    }
}


void Vector_direccion::gira_45_sentido_menos()
{
    switch(direccion_){
	case Direccion::E: 
	    y_ = -1;
	    direccion_ = Direccion::SE;
	    break;

	case Direccion::NE:
	    y_ = 0;
	    direccion_ = Direccion::E;
	    break;

	case Direccion::N:
	    x_ = 1;
	    direccion_ = Direccion::NE;
	    break;

	case Direccion::NW:
	    x_ = 0;
	    direccion_ = Direccion::N;
	    break;

	case Direccion::W:
	    y_ = 1;
	    direccion_ = Direccion::NW;
	    break;

	case Direccion::SW:
	    y_ = 0;
	    direccion_ = Direccion::W;
	    break;

	case Direccion::S:
	    x_ = -1;
	    direccion_ = Direccion::SW;
	    break;

	case Direccion::SE:
	    x_ = 0;
	    direccion_ = Direccion::S;
	    break;
    }
}




} // namespace


