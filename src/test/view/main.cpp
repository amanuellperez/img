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


#include "../../img_view.h"
#include "../../img_algorithm.h"
#include "../../img_draw.h"

#include <iostream>
#include <numeric>

#include <alp_test.h>


struct const_ColorXYZ_x{
    const double operator()(const img::ColorRGB& c) const 
    {return c.r + c.g;}
};

void test_subimagen()
{
    test::interfaz("Subimagen");

    img::Imagen img0{2,2};
    for_each_ij(img0, [&img0](auto i, auto j){img0(i,j) = img::ColorRGB{i, j, 0};});
    
    {
	// Subimagen.clone
	img::Subimagen sb{img0, img0.extension()};

	img::Imagen img1{0,0};
	img1 = clone(sb);
	CHECK_TRUE(img1.size2D() == img0.size2D(), "clone(size)");
	CHECK_EQUAL_CONTAINERS(img0.begin(), img0.end(), 
				img1.begin(), img1.end(),
				"clone");
    }

    {
	// const_Subimagen.clone
	img::const_Subimagen sb{img0, img0.extension()};

	img::Imagen img1{0,0};
	img1 = clone(sb);
	CHECK_TRUE(img1.size2D() == img0.size2D(), "clone(size)");
//	check_equal_containers(img0.begin(), img0.end(), 
//				img1.begin(), img1.end(),
//				"clone");
    }
}

void test_img_view()
{
    test::interfaz("img::Imagen_view");

    img::Imagen img0{4,5};	// TODO: falta probar las funciones const!!!

    for (int i = 0; i < img0.rows(); ++i)
	for (int j = 0; j < img0.cols(); ++j)
	    img0(i,j) = img::ColorRGB{i*img0.cols() + j, 0, 0};

//    alp::print2D(std::cout, img0);
    
    {// imagen_red
	auto imgred = imagen_red(img0);
	for (int i = 0; i < imgred.rows(); ++i)
	    for (int j = 0; j < imgred.cols(); ++j){
		CHECK_TRUE(imgred(i,j) == img0(i,j).r, "img::Imagen_red(i,j)");
	    }

	auto p0 = img0.begin();
	for (auto p = imgred.begin(); p != imgred.end(); ++p, ++p0)
	    CHECK_TRUE(*p == p0->r, "begin/end");



	std::iota(imgred.begin(), imgred.end(), 0);
	// Compila for range?
	for (auto c: imagen_red(img0))
	    std::cout << c << ' ';
	std::cout << '\n';
    }

    {
	const auto img1 = imagen_view(img0, const_ColorXYZ_x{});

	const_ColorXYZ_x f;
	for (int i = 0; i < img1.rows(); ++i)
	    for (int j = 0; j < img1.cols(); ++j){
		CHECK_TRUE(img1(i,j) == f(img0(i,j)), "prueba03");
	    }
    }

    {
	std::cout << "\nProbando img::Imagen_rt\n"
	     <<   "------------------\n";
    
	img::Imagen img0 = imagen_monocolor(11,11, img::ColorRGB{-1,-1,-1});

	img::Imagen_rt img_rt{img0};

	for (img::Imagen_rt::Ind r = 0; r <= img_rt.r_max(); ++r){
	    img_rt(r, 0.0) = img::ColorRGB{0,0,0};
	    img_rt(r, 45.0) = img::ColorRGB{45, 45, 45};
	    img_rt(r, 90.0) = img::ColorRGB{90, 90, 90};
	    img_rt(r, 135.0) = img::ColorRGB{135, 135, 135};
	    img_rt(r, 180.0) = img::ColorRGB{180, 180, 180};
	    img_rt(r, 225.0) = img::ColorRGB{225, 225, 225};
	    img_rt(r, 270.0) = img::ColorRGB{270, 270, 270};
	    img_rt(r, -45.0) = img::ColorRGB{-45, -45, -45};
	}

	// print2D
	auto img_red = imagen_red(img0);
	for (int i = 0; i < img_red.rows(); ++i){
	    for (int j = 0; j < img_red.cols(); ++j){
		std::cout.width(3);
		std::cout.fill(' ');
		if (img_red(i,j) == -1)
		    std::cout << "___ ";
		else{
		    std::cout << img_red(i,j) << ' ';
		}
	    }
	    std::cout << '\n';
	}
    }
}


void test_imagen_view_and_subimagen()
{
    {// pruebo que compile Matrix_view con img::Imagen
	// Matrix_view<img::Imagen, Color_red> m {img0, Color_red{}};
//	Matrix_view<Submatrix<img::Imagen>, Color_red> m {img0, Color_red{}};
//
//	auto f = m.row_begin();
    }

    img::Imagen img0{4,5};	

    for (int i = 0; i < img0.rows(); ++i)
	for (int j = 0; j < img0.cols(); ++j)
	    img0(i,j) = img::ColorRGB{i*img0.cols() + j,
				 (i*img0.cols() + j)*10,
				 (i*img0.cols() + j)*20};

    img::Subimagen sb{img0, img0.extension()};


    {
    auto img_red = imagen_red(sb);
    CHECK_TRUE(img_red.rows() == img0.rows(), "rows");
    CHECK_TRUE(img_red.cols() == img0.cols(), "cols");

    auto p = img0.begin();
    auto q = img_red.begin();
    for (; p != img0.end(); ++p, ++q)
	CHECK_TRUE(*q == p->r, "imagen_red");

    {
	auto p = img0.begin();
	for (auto f = img_red.row_begin(); f != img_red.row_end(); ++f)
	    for (auto q = f->begin(); q != f->end(); ++q, ++p)
		CHECK_TRUE(*q == p->r, "imagen_red.row_begin/end");
    }

    img_red(0,0) = 255;
    CHECK_TRUE(img0(0,0).r == 255, "img_red() = ");
    }


    {
    auto img_green = imagen_green(sb);
    CHECK_TRUE(img_green.rows() == img0.rows(), "rows");
    CHECK_TRUE(img_green.cols() == img0.cols(), "cols");

    auto p = img0.begin();
    auto q = img_green.begin();
    for (; p != img0.end(); ++p, ++q)
	CHECK_TRUE(*q == p->g, "imagen_green");

    img_green(0,0) = 255;
    CHECK_TRUE(img0(0,0).g == 255, "img_green() = ");
    }

    {
    auto img_blue = imagen_blue(sb);
    CHECK_TRUE(img_blue.rows() == img0.rows(), "rows");
    CHECK_TRUE(img_blue.cols() == img0.cols(), "cols");

    auto p = img0.begin();
    auto q = img_blue.begin();
    for (; p != img0.end(); ++p, ++q)
	CHECK_TRUE(*q == p->b, "imagen_blue");

    img_blue(0,0) = 255;
    CHECK_TRUE(img0(0,0).b == 255, "img_blue() = ");
    }

    {// const_imagen_red
//    auto img_red = const_imagen_red(sb);
//    CHECK_TRUE(img_red.rows() == img0.rows(), "rows");
//    CHECK_TRUE(img_red.cols() == img0.cols(), "cols");
//
//    auto p = img0.begin();
//    auto q = img_red.begin();
//    for (; p != img0.end(); ++p, ++q)
//	CHECK_TRUE(*q == p->r, "imagen_red");

//    img_red(0,0) = 255;   // esto no tiene que compilar
//    CHECK_TRUE(img0(0,0).r == 255, "img_red() = ");
    }
}


void test_imagen_xy()
{
    img::Imagen img0{4,5};

    for (int i = 0; i < img0.rows(); ++i)
	for (int j = 0; j < img0.cols(); ++j)
	    img0(i,j) = img::ColorRGB{i*img0.cols() + j, 0, 0};

    alp::print(std::cout, img0);

    {
	img::Imagen_xy<1,1> imgxy{img0};
	std::cout << "Imprimimos img_xy\n";
	std::cout << "-----------------\n";
	for (int y = 0; y <= imgxy.y_max(); ++y){
	    for (int x = 0; x <= imgxy.x_max(); ++x)
		std::cout << imgxy(x,y).r << ' ';

	    std::cout << '\n';
	}
    }
    {// caso const
	const img::Imagen& img1 = img0;

	img::const_Imagen_xy<1,1> imgxy{img1};
	std::cout << "Imprimimos const_img_xy\n";
	std::cout << "-----------------------\n";
	for (int y = 0; y <= imgxy.y_max(); ++y){
	    for (int x = 0; x <= imgxy.x_max(); ++x)
		std::cout << imgxy(x,y).r << ' ';

	    std::cout << '\n';
	}
    }

    {// caso genérico
	using Point = img::Imagen_xy<1,1>::Point;

	img::Imagen_xy<1,1> imgxy{img0, 1, 3};
	CHECK_TRUE(imgxy.rows()  == 4, "rows");
	CHECK_TRUE(imgxy.cols()  == 5, "cols");
	CHECK_TRUE(imgxy.x_min() == -3, "x_min");
	CHECK_TRUE(imgxy.x_max() == 1, "x_max");
	CHECK_TRUE(imgxy.y_min() == -2, "y_min");
	CHECK_TRUE(imgxy.y_max() == 1, "y_max");

	CHECK_TRUE(imgxy(0,0).r == 8, "(0,0)");
	CHECK_TRUE(imgxy(-3,-2).r == 15, "(-3,-2)");
	CHECK_TRUE(imgxy(-3,1).r == 0, "(-3,1)");
	CHECK_TRUE(imgxy(1,1).r == 4, "(1,1)");
	CHECK_TRUE(imgxy(1,-2).r == 19, "(1,-2)");

	CHECK_TRUE((upper_left_corner(imgxy) == Point{-3,1}), "upper_left_corner");
	CHECK_TRUE((upper_right_corner(imgxy) == Point{1,1}), "upper_right_corner");
	CHECK_TRUE((bottom_left_corner(imgxy) == Point{-3,-2}), "bottom_left_corner");
	CHECK_TRUE((bottom_right_corner(imgxy) == Point{1,-2}), "bottom_right_corner");

	std::cout << "\nCambiamos el origen\n";
	imgxy.origen_de_coordenadas(1,2);
	CHECK_TRUE(imgxy.x_min() == -2, "x_min");
	CHECK_TRUE(imgxy.x_max() == 2, "x_max");
	CHECK_TRUE(imgxy.y_min() == -2, "y_min");
	CHECK_TRUE(imgxy.y_max() == 1, "y_max");

	CHECK_TRUE(imgxy(0,0).r == 7, "(0,0)");
	CHECK_TRUE(imgxy(2,1).r == 4, "(2,1)");
	CHECK_TRUE(imgxy(2,-2).r == 19, "(2,-2)");
	CHECK_TRUE(imgxy(-2,-2).r == 15, "(-2,-2)");
	CHECK_TRUE(imgxy(-2,1).r == 0, "(-2,1)");

    }

}



int main()
{
try{
    test_subimagen();
    test_img_view();
    test_imagen_xy();
    test_imagen_view_and_subimagen();

}catch(std::exception& e){
    std::cerr << "EXCEPTION: " << e.what() << '\n';
    return 1;
}

}


