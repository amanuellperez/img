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


#include "../../img_algorithm.h"

#include <alp_test.h>
#include <iostream>

using namespace test;

void test_alg()
{
    test::interfaz("algorithm");

    img::Image img0{2,2};
    img0(0,0) = img::ColorRGB{1,1,1};
    img0(0,1) = img::ColorRGB{2,2,2};
    img0(1,0) = img::ColorRGB{3,3,3};
    img0(1,1) = img::ColorRGB{4,4,4};

    int a = 3;
    auto img1 = expande(img0, a);
    CHECK_TRUE((img1.rows() == a*img0.rows()
		and img1.cols() == a*img0.cols()), "expande.size()");

    img::Image res{3, 3};
    std::fill(res.begin(), res.end(), img0(0,0));
    img::Subimage sb{img1, img::Position{0,0}, img::Size2D{3,3}};
    CHECK_EQUAL_CONTAINERS(sb.begin(), sb.end(), res.begin(), res.end()
			, "expande()");

    std::fill(res.begin(), res.end(), img0(0,1));
    sb.extension(img::Position{0,3}, img::Size2D{3,3});
    CHECK_EQUAL_CONTAINERS(sb.begin(), sb.end(), res.begin(), res.end()
			, "expande()");

    std::fill(res.begin(), res.end(), img0(1,0));
    sb.extension(img::Position{3,0}, img::Size2D{3,3});
    CHECK_EQUAL_CONTAINERS(sb.begin(), sb.end(), res.begin(), res.end()
			, "expande()");

    std::fill(res.begin(), res.end(), img0(1,1));
    sb.extension(img::Position{3,3}, img::Size2D{3,3});
    CHECK_EQUAL_CONTAINERS(sb.begin(), sb.end(), res.begin(), res.end()
			, "expande()");

}


void test_rotate(const alp::Degree& angle, const std::string& img_name)
{
    std::cout << "\n\ntest_rotate(" << angle.value() << ") <-- MIRAR LA IMAGEN RESULTANTE\n";

    img::Image img0 = img::read(img_name);

    auto img1 = img::rotate(img0, angle);

    img::write(img1, alp::as_str() << "rotate" << angle.value() << ".jpg");
}



void test_rotate()
{
    test::interfaz("rotate");

    {
	img::Image img0{5, 10};
	std::cout << img::_rotate_dimensions(img0, alp::Degree{0}) << '\n';

        CHECK_TRUE(
            (img::_rotate_dimensions(img0, alp::Degree{0}) == img::Size2D{5, 10}),
            "rotate(0)");
        CHECK_TRUE(
            (img::_rotate_dimensions(img0, alp::Degree{90}) == img::Size2D{10, 5}),
            "rotate(90)");
        CHECK_TRUE(
            (img::_rotate_dimensions(img0, alp::Degree{180}) == img::Size2D{5, 10}),
            "rotate(180)");
        CHECK_TRUE(
            (img::_rotate_dimensions(img0, alp::Degree{270}) == img::Size2D{10, 5}),
            "rotate(270)");
    }

    for (int th = 5; th < 360; th += 5)
	test_rotate(th, "prueba.jpg");
}

void test_refence_frame_rotation(const alp::Degree& angle, 
				 int x, int y, int X_res, int Y_res)
{
    img::Reference_frame_rotation rota{angle};
    auto [X, Y] = rota(x, y);
//std::cout << "(X, Y) = (" << X << ", " << Y << ")\n";
    CHECK_TRUE(X == X_res and Y == Y_res,
               alp::as_str() << "rota(" << x << ", " << y << ")");
}

void test_refence_frame_rotation()
{
    test::interfaz("Reference_frame_rotation");

    test_refence_frame_rotation(alp::Degree{0}, 10, 10, 10, 10);
    test_refence_frame_rotation(alp::Degree{40}, 10, 10, 1, 14);
    test_refence_frame_rotation(alp::Degree{90}, 10, 10, -10, 10);
    test_refence_frame_rotation(alp::Degree{180}, 10, 10, -10, -10);
    test_refence_frame_rotation(alp::Degree{270}, 10, 10, 10, -10);
    test_refence_frame_rotation(alp::Degree{360}, 10, 10, 10, 10);
}



int main()
{
try{
    test::header("img_alg.h");
    test_alg();
    std::cout << "\n\nSi quieres probar rotate tienes que descomentarlo!!!\n\n";
    // test_rotate();
    test_refence_frame_rotation();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}
}
