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


#include "../../img_escala.h"

#include <alp_test.h>

#include <iostream>


void test_escala()
{
    test::interfaz("escala");

    img::Image img0 = img::read("../prueba.jpg");
    
    img::Image img1 = img::escala(img0, 400, 300);

    img::write(img1, "res.jpg");
}


int main()
{
try{

    test::header("img_escala.h");
    test_escala();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

    return 0;
}
