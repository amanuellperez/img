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


#include "../../img_image.h"

#include <iostream>
#include <alp_test.h>


using namespace test;

template <typename Img>
void test_imagen_funciones_const(Img& img1)
{
    for (int i = 0; i < img1.rows(); ++i){
	for (int j = 0; j < img1.cols(); ++j)
            CHECK_TRUE((img1(i, j) == img::ColorRGB{i, j, i + j}), "operator(i,j)");
    }

    {
	int i = 0, j = 0;
	for (auto p = img1.begin(); p != img1.end(); ++p){
	    CHECK_TRUE(*p == img1(i,j), "begin/end");
	    ++j;
	    if (j == img1.cols()){
		++i;
		j = 0;
	    }

	}
    }

    {
	// for-range ¿compila?
	for (auto c: img1)
	    std::cout << c.r << ' ';
	std::cout << '\n';
    }

    {// iteramos por filas
	int i = 0, j = 0;
	for (auto f = img1.row_begin(); f != img1.row_end(); ++f, ++i, j = 0){
	    for (auto p = f->begin(); p != f->end(); ++p, ++j){
            CHECK_TRUE((*p == img::ColorRGB{i, j, i + j}), "begin/end/operator*");
	    }
	}
    }


}

void test_img()
{
    constexpr int num_rows = 3, num_cols = 4;
    img::Image img1{num_rows, num_cols};

    CHECK_TRUE(img1.rows() == num_rows, "rows()");
    CHECK_TRUE(img1.cols() == num_cols, "cols()");
    CHECK_TRUE(img1.size() == num_rows*num_cols, "size()");

    for (int i = 0; i < img1.rows(); ++i)
	for (int j = 0; j < img1.cols(); ++j)
	    img1(i,j) = img::ColorRGB{i, j, i + j};

    test_imagen_funciones_const<img::Image>(img1);
    test_imagen_funciones_const<const img::Image>(img1);

    {// row_cbegin
	int i = 0, j = 0;
	for (auto f = img1.row_cbegin(); f != img1.row_cend(); ++f, ++i, j = 0){
	    for (auto p = f->begin(); p != f->end(); ++p, ++j){
            CHECK_TRUE((*p == img::ColorRGB{i, j, i + j}), "row_cbegin/cend");
	    }
	}
    }

    {
	const img::Image img2 = img1;
	CHECK_EQUAL_CONTAINERS(img1.begin(), img1.end()
				, img2.begin(), img2.end()
				, "operator=");
    }


    // probamos que funcionen algoritmos STL
    // -------------------------------------
    {
	img::ColorRGB color{10, 20, 30};

	img::Image img0{4, 3};
	std::fill(img0.begin(), img0.end(), color);

	for (const auto& c: img0)
	    CHECK_TRUE(c == color, "std::fill()");

	// probamos ranges-for
	img::ColorRGB color2{50, 100, 150};
	for (auto& c: img0)
	    c = color2;

	for (const auto& c: img0)
	    CHECK_TRUE(c == color2, "ranges-for");
    }
    {
	img::Image img0{2,2};
	img0(0,0) = img::ColorRGB{1, 2, 3}; img0(0,1) = img::ColorRGB{4, 5, 6}; 
	img0(1,0) = img::ColorRGB{7, 8, 9}; img0(1,1) = img::ColorRGB{10, 11, 12}; 

	img::Image img1{img0.size2D()};

	std::copy(img0.begin(), img0.end(), img1.begin());

	CHECK_EQUAL_CONTAINERS(img0.begin(), img0.end()
			     , img1.begin(), img1.end()
			     , "std::copy()");

	// cambio el orden RGB -> BGR
//	std::transform(img0.begin(), img0.end(), img0.begin(), 
//		[](auto c){return ColorRGB{c.B(), c.G(), c.R()};});
	


    }
}


int main()
{
try{
    test_img();
}catch(std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}
}


