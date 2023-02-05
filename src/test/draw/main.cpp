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


#include "../../img_draw.h"

#include <alp_test.h>

#include <iostream>


using namespace test;

void test_draw()
{
    test::interfaz("draw");
    
    {
	img::Image img0 = img::imagen_monocolor(3, 4, img::ColorRGB::rojo());
    
	CHECK_TRUE((img0.rows() == 3 and img0.cols() == 4), "img_monocolor(size)");

	for (const auto& c: img0)
	    CHECK_TRUE(c == img::ColorRGB::rojo(), "img_monocolor()");
    }

    {
	img::Image img0 = img::imagen_monocolor(img::Size2D{3, 4}, img::ColorRGB::rojo());
    
	CHECK_TRUE((img0.rows() == 3 and img0.cols() == 4), "img_monocolor(size)");

	for (const auto& c: img0)
	    CHECK_TRUE(c == img::ColorRGB::rojo(), "img_monocolor()");
    }
    

}


int main()
{
try{

    test::header("img_draw.h");
    test_draw();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

    return 0;
}
