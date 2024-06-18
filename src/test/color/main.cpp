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


#include "../../img_color.h"

#include <alp_matrix.h>
#include <alp_matrix_algorithm.h>
#include <alp_test.h>
#include <iostream>

using namespace test;

void test_iterator_color()
{
    alp::Matrix<img::ColorRGB, int> img1{3, 4};

    int i = 1;
    for (auto p = iterator_red(img1.begin()); p != iterator_red(img1.end());
         ++p, ++i)
	*p = i;

    for (auto p = iterator_green(img1.begin()); p != iterator_green(img1.end());
         ++p, ++i)
	*p = i;

    for (auto p = iterator_blue(img1.begin()); p != iterator_blue(img1.end());
         ++p, ++i)
	*p = i;


    std::cout << "Imagen en green:\n";
    for (auto f = img1.row_begin(); f != img1.row_end(); ++f){
        for (auto p = const_iterator_green(f->begin());
			    p != const_iterator_green(f->end()); ++p) {
            std::cout << *p << ' ';
	    // *p = 4;	// esto no tiene que compilar.
	}
	std::cout << '\n';
    }

    std::cout << "Imagen en blue:\n";
    for (auto f = img1.row_begin(); f != img1.row_end(); ++f){
	for (auto p = const_iterator_blue(f->begin()); p != const_iterator_blue(f->end()); ++p){
	    std::cout << *p << ' ';
	    // *p = 4;	// esto no tiene que compilar.
	}
	std::cout << '\n';
    }



    std::cout << "\n\n---------------------------\n";
    std::cout << "Comprobaciones automáticas:\n";
    std::cout << "---------------------------\n";
    std::vector<int> red01 = {1,2,3,4,5,6,7,8,9,10,11,12};
    CHECK_EQUAL_CONTAINERS(	  const_iterator_red(img1.begin())
				, const_iterator_red(img1.end())
				, red01.begin()
				, red01.end()
				, "Comprobando iterator_red ... ");


    std::vector<int> green01 = {13,14,15,16,17,18,19,20,21,22,23,24};
    CHECK_EQUAL_CONTAINERS(	  const_iterator_green(img1.begin())
				, const_iterator_green(img1.end())
				, green01.begin()
				, green01.end()
				, "Comprobando iterator_green ... ");

    std::vector<int> blue01 = {25,26,27,28,29,30,31,32,33,34,35,36};
    CHECK_EQUAL_CONTAINERS(	  const_iterator_blue(img1.begin())
				, const_iterator_blue(img1.end())
				, blue01.begin()
				, blue01.end()
				, "Comprobando iterator_blue ... ");
}

struct ColorRgBg{
    std::pair<int, int> operator()(const img::ColorRGB& c) const
    { return {c.r - c.g, c.g - c.b}; }
};


template <typename It>
inline auto const_iteratorRgGb(It p)
{
    return alp::view_iterator(p, ColorRgBg{});
}


void test_iterator_transformador()
{
    alp::Matrix<img::ColorRGB, int> img1{3, 4};
    
    for (int i = 0; i < img1.rows(); ++i)
	for (int j = 0; j < img1.cols(); ++j)
	    img1(i,j) = img::ColorRGB{img1.cols() * i + j, i,j};


    alp::print(std::cout, img1);
    

    for (auto f = img1.row_begin(); f != img1.row_end(); ++f)
    {
        for (auto p = const_iteratorRgGb(f->begin());
    			 p != const_iteratorRgGb(f->end()); ++p) 
	{
            auto c = *p;
	    std::cout << "(" << c.first <<  ", " << c.second << "); ";
	}
	std::cout << '\n';
    }

}


void test_colorIO()
{
    img::ColorRGB c1{1,2,3};
    std::stringstream s;
    s << c1;
    CHECK_TRUE((s.str() == "(001, 002, 003)"), "operator<<");

    img::ColorRGB c2;
    s >> c2;
    CHECK_TRUE(!(!s), "operator>>(ok)");
    CHECK_TRUE(c1 == c2, "operator>>");

    std::stringstream s2;
    s2 << "(1, 2, 3";
    s2 >> c2;
    CHECK_TRUE(!s2, "operator>>(failbit)");
    s2.clear();
}



void test_color()
{
    img::ColorRGB c{1,2,3};
    
    {
    auto d = -c;
    CHECK_TRUE(d.r == -1 and d.g == -2 and d.b == -3
		, "operator-()");
    }

    test_colorIO();
}



int main()
{
try{
    test::header("img_color.h");

    test_iterator_color();
    test_color();
    test_iterator_transformador();
}catch(std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}
}


