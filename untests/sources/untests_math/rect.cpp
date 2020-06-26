/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("rect") {
    {
        REQUIRE(b2i::zero() == b2i(0,0,0,0));
        REQUIRE(b2i::unit() == b2i(0,0,1,1));
    }
    {
        REQUIRE(b2i().position == v2i(0,0));
        REQUIRE(b2i().size == v2i(0,0));

        REQUIRE(b2i(1,2).position == v2i(0,0));
        REQUIRE(b2i(1,2).size == v2i(1,2));

        REQUIRE(b2i(1,2,3,4).position == v2i(1,2));
        REQUIRE(b2i(1,2,3,4).size == v2i(3,4));

        REQUIRE(b2i(v2i{1,2}).position == v2i(0,0));
        REQUIRE(b2i(v2i{1,2}).size == v2i(1,2));

        REQUIRE(b2i(v2i{1,2},v2i{3,4}).position == v2i(1,2));
        REQUIRE(b2i(v2i{1,2},v2i{3,4}).size == v2i(3,4));
    }
    {
        REQUIRE(b2f(1,2,3,4).cast_to<i32>() == b2i(1,2,3,4));
    }
    {
        auto r0 = b2i(1,2,3,4);
        REQUIRE(r0.data()[0] == 1);
        REQUIRE(r0.data()[1] == 2);
        REQUIRE(r0.data()[2] == 3);
        REQUIRE(r0.data()[3] == 4);
        r0.data()[0] = 2;
        REQUIRE(r0 == b2i(2,2,3,4));
        const auto& cr0 = r0;
        REQUIRE(cr0.data()[0] == 2);
        REQUIRE(cr0.data()[1] == 2);
        REQUIRE(cr0.data()[2] == 3);
        REQUIRE(cr0.data()[3] == 4);
    }
    {
        auto r0 = b2i(1,2,3,4);
        REQUIRE(r0[0] == 1);
        REQUIRE(r0[1] == 2);
        REQUIRE(r0[2] == 3);
        REQUIRE(r0[3] == 4);
        r0[0] = 2;
        REQUIRE(r0 == b2i(2,2,3,4));
        const auto& cr0 = r0;
        REQUIRE(cr0[0] == 2);
        REQUIRE(cr0[1] == 2);
        REQUIRE(cr0[2] == 3);
        REQUIRE(cr0[3] == 4);
    }
    {
        auto r0 = b2i(1,2,3,4);
        REQUIRE(&r0 == &(r0 += 1));
        REQUIRE(r0 == b2i(2,3,3,4));
        REQUIRE(&r0 == &(r0 -= 4));
        REQUIRE(r0 == b2i(-2,-1,3,4));
        REQUIRE(&r0 == &(r0 *= 2));
        REQUIRE(r0 == b2i(-2,-1,6,8));
        REQUIRE(&r0 == &(r0 /= 3));
        REQUIRE(r0 == b2i(-2,-1,2,2));

        REQUIRE(b2i(1,2,3,4) + 2 == b2i(3,4,3,4));
        REQUIRE(b2i(1,2,3,4) - 2 == b2i(-1,0,3,4));
        REQUIRE(b2i(1,2,3,4) * 3 == b2i(1,2,9,12));
        REQUIRE(b2i(1,2,6,9) / 3 == b2i(1,2,2,3));

        REQUIRE(b2i(1,2,3,4) + v2i(1,2) == b2i(2,4,3,4));
        REQUIRE(b2i(1,2,3,4) - v2i(1,2) == b2i(0,0,3,4));
        REQUIRE(b2i(1,2,3,4) * v2i(2,3) == b2i(1,2,6,12));
        REQUIRE(b2i(1,2,6,8) / v2i(3,2) == b2i(1,2,2,4));
    }
    {
        REQUIRE(make_rect(2,1) == b2i(0,0,2,1));
        REQUIRE(make_rect(4,3,2,1) == b2i(4,3,2,1));
        REQUIRE(make_rect(v2i{2,1}) == b2i(0,0,2,1));
        REQUIRE(make_rect(v2i{4,3},v2i{2,1}) == b2i(4,3,2,1));

        REQUIRE(math::make_minmax_rect(1,2,3,4) == b2i(1,2,2,2));
        REQUIRE(math::make_minmax_rect(3,4,1,2) == b2i(1,2,2,2));

        REQUIRE(math::make_minmax_rect(v2i{1,2}, v2i{3,4}) == b2i(1,2,2,2));
        REQUIRE(math::make_minmax_rect(v2i{3,4}, v2i{1,2}) == b2i(1,2,2,2));

        REQUIRE(math::make_minmax_rect(b2i(1,2,3,4)) == b2i(1,2,3,4));
        REQUIRE(math::make_minmax_rect(b2i(1,2,-3,4)) == b2i(-2,2,3,4));
    }
    {
        auto r0 = b2i(1,2,3,4);
        auto r1 = b2i(r0);
        REQUIRE(r1 == b2i(1,2,3,4));
        r1 = b2i(4,3,2,1);
        REQUIRE(r1 == b2i(4,3,2,1));
    }
    {
        REQUIRE(b2i(1,2,3,4) == b2i(1,2,3,4));
        REQUIRE_FALSE(b2i(1,2,3,4) == b2i(1,2,4,3));
        REQUIRE_FALSE(b2i(1,2,3,4) == b2i(2,1,3,4));
        REQUIRE_FALSE(b2i(1,2,3,4) == b2i(2,1,4,3));

        REQUIRE_FALSE(b2i(1,2,3,4) != b2i(1,2,3,4));
        REQUIRE(b2i(1,2,3,4) != b2i(1,2,4,3));
        REQUIRE(b2i(1,2,3,4) != b2i(2,1,3,4));
        REQUIRE(b2i(1,2,3,4) != b2i(2,1,4,3));
    }
    {
        REQUIRE(b2i(4,4) < b2i(4,5));
        REQUIRE_FALSE(b2i(4,4) < b2i(3,4));
        REQUIRE_FALSE(b2i(4,4) < b2i(4,3));
    }
    {
        REQUIRE(math::approximately(b2i(1,2,3,4), b2i(1,2,4,5), 1));
        REQUIRE_FALSE(math::approximately(b2i(1,2,3,4), b2i(1,2,4,5)));
    }
    {
        REQUIRE(math::minimum(b2i(1,2,3,4)) == v2i(1,2));
        REQUIRE(math::minimum(b2i(1,2,-3,5)) == v2i(-2,2));
        REQUIRE(math::minimum(b2i(1,2,3,-5)) == v2i(1,-3));
        REQUIRE(math::minimum(b2i(1,2,-3,-5)) == v2i(-2,-3));

        REQUIRE(math::maximum(b2i(1,2,3,4)) == v2i(4,6));
        REQUIRE(math::maximum(b2i(1,2,-3,5)) == v2i(1,7));
        REQUIRE(math::maximum(b2i(1,2,3,-5)) == v2i(4,2));
        REQUIRE(math::maximum(b2i(1,2,-3,-5)) == v2i(1,2));

        REQUIRE(math::area(b2i(1,2,3,4)) == 12);
        REQUIRE(math::area(b2i(1,2,3,-4)) == -12);
        REQUIRE(math::area(b2i(1,2,-3,4)) == -12);
        REQUIRE(math::area(b2i(1,2,-3,-4)) == 12);

        REQUIRE(math::abs_area(b2i(1,2,3,4)) == 12);
        REQUIRE(math::abs_area(b2i(1,2,3,-4)) == 12);
        REQUIRE(math::abs_area(b2i(1,2,-3,4)) == 12);
        REQUIRE(math::abs_area(b2i(1,2,-3,-4)) == 12);

        REQUIRE(math::merged(b2i(1,2,3,4), b2i(1,2,3,4)) == b2i(1,2,3,4));
        REQUIRE(math::merged(b2i(1,2,3,4), b2i(0,1,3,4)) == b2i(0,1,4,5));
        REQUIRE(math::merged(b2i(1,2,3,4), b2i(1,2,4,5)) == b2i(1,2,4,5));
        REQUIRE(math::merged(b2i(1,2,3,4), b2i(0,1,4,5)) == b2i(0,1,4,5));

        REQUIRE(math::merged(b2i(1,2,3,4), b2i(1,2,-3,-5)) == b2i(-2,-3,6,9));
        REQUIRE(math::merged(b2i(-1,-2,3,4), b2i(1,2,-3,-5)) == b2i(-2,-3,4,5));

        REQUIRE(math::inside(b2i(1,2,3,4), v2i(1,2)));
        REQUIRE(math::inside(b2i(1,2,3,4), v2i(4,5)));
        REQUIRE_FALSE(math::inside(b2i(1,2,3,4), v2i(4,7)));
        REQUIRE_FALSE(math::inside(b2i(1,2,3,4), v2i(0,5)));

        REQUIRE(math::inside(b2i(1,2,-3,-4), v2i(1,2)));
        REQUIRE(math::inside(b2i(1,2,-3,-4), v2i(-1,-2)));
        REQUIRE_FALSE(math::inside(b2i(1,2,-3,-4), v2i(2,2)));
        REQUIRE_FALSE(math::inside(b2i(1,2,-3,-4), v2i(1,3)));
        REQUIRE_FALSE(math::inside(b2i(1,2,-3,-4), v2i(-3,2)));
        REQUIRE_FALSE(math::inside(b2i(1,2,-3,-4), v2i(1,-3)));

        REQUIRE_FALSE(math::overlaps(b2i(0,0,10,10), b2i(10,0,10,10)));
        REQUIRE_FALSE(math::overlaps(b2i(0,0,10,10), b2i(0,10,10,10)));
        REQUIRE_FALSE(math::overlaps(b2i(0,0,10,10), b2i(10,10,10,10)));
        REQUIRE(math::overlaps(b2i(0,0,10,10), b2i(9,0,10,10)));
        REQUIRE(math::overlaps(b2i(0,0,10,10), b2i(0,9,10,10)));
        REQUIRE(math::overlaps(b2i(0,0,10,10), b2i(9,9,10,10)));
        REQUIRE(math::overlaps(b2i(0,0,10,10), b2i(-9,0,10,10)));
        REQUIRE(math::overlaps(b2i(0,0,10,10), b2i(0,-9,10,10)));
        REQUIRE(math::overlaps(b2i(0,0,10,10), b2i(-9,-9,10,10)));
    }
    {
        REQUIRE(math::normalized_to_point(b2f(10.f, 20.f), v2f(0.f, 0.f)) == v2f(0.f, 0.f));
        REQUIRE(math::normalized_to_point(b2f(10.f, 20.f), v2f(0.5f, 0.5f)) == v2f(5.f, 10.f));
        REQUIRE(math::normalized_to_point(b2f(10.f, 20.f), v2f(0.25f, 0.75f)) == v2f(2.5f, 15.f));
        REQUIRE(math::normalized_to_point(b2f(10.f, 20.f), v2f(1.f, 1.f)) == v2f(10.f, 20.f));

        REQUIRE(math::normalized_to_point(b2f(1.f, 2.f, 10.f, 20.f), v2f(0.f, 0.f)) == v2f(1.f, 2.f));
        REQUIRE(math::normalized_to_point(b2f(1.f, 2.f, 10.f, 20.f), v2f(0.5f, 0.5f)) == v2f(6.f, 12.f));
        REQUIRE(math::normalized_to_point(b2f(1.f, 2.f, 10.f, 20.f), v2f(0.25f, 0.75f)) == v2f(3.5f, 17.f));
        REQUIRE(math::normalized_to_point(b2f(1.f, 2.f, 10.f, 20.f), v2f(1.f, 1.f)) == v2f(11.f, 22.f));

        REQUIRE(math::normalized_to_point(b2f(1.f, 2.f, -11.f, -22.f), v2f(0.f, 0.f)) == v2f(-10.f, -20.f));
        REQUIRE(math::normalized_to_point(b2f(1.f, 2.f, -11.f, -22.f), v2f(0.5f, 0.5f)) == v2f(-4.5f, -9.f));
        REQUIRE(math::normalized_to_point(b2f(1.f, 2.f, -11.f, -22.f), v2f(1.f, 1.f)) == v2f(1.f, 2.f));

        REQUIRE(math::normalized_to_point(b2f(0.f,0.f), v2f(0.f,0.f)) == v2f(0.f,0.f));
        REQUIRE(math::normalized_to_point(b2f(0.f,0.f), v2f(1.f,1.f)) == v2f(0.f,0.f));
        REQUIRE(math::normalized_to_point(b2f(1.f,2.f,0.f,0.f), v2f(0.f,0.f)) == v2f(1.f,2.f));
        REQUIRE(math::normalized_to_point(b2f(1.f,2.f,0.f,0.f), v2f(1.f,1.f)) == v2f(1.f,2.f));
    }
    {
        REQUIRE(math::point_to_normalized(b2f(10.f, 20.f), v2f(0.f, 0.f)) == v2f(0.f, 0.f));
        REQUIRE(math::point_to_normalized(b2f(10.f, 20.f), v2f(5.f, 10.f)) == v2f(0.5f,0.5f));
        REQUIRE(math::point_to_normalized(b2f(10.f, 20.f), v2f(2.5f, 15.f)) == v2f(0.25f, 0.75f));
        REQUIRE(math::point_to_normalized(b2f(10.f, 20.f), v2f(10.f, 20.f)) == v2f(1.f, 1.f));

        REQUIRE(math::point_to_normalized(b2f(1.f, 2.f, 10.f, 20.f), v2f(1.f, 2.f)) == v2f(0.f, 0.f));
        REQUIRE(math::point_to_normalized(b2f(1.f, 2.f, 10.f, 20.f), v2f(6.f, 12.f)) == v2f(0.5f, 0.5f));
        REQUIRE(math::point_to_normalized(b2f(1.f, 2.f, 10.f, 20.f), v2f(3.5f, 17.f)) == v2f(0.25f, 0.75f));
        REQUIRE(math::point_to_normalized(b2f(1.f, 2.f, 10.f, 20.f), v2f(11.f, 22.f)) == v2f(1.f, 1.f));

        REQUIRE(math::point_to_normalized(b2f(1.f, 2.f, -11.f, -22.f), v2f(-10.f, -20.f)) == v2f(0.f, 0.f));
        REQUIRE(math::point_to_normalized(b2f(1.f, 2.f, -11.f, -22.f), v2f(-4.5f, -9.f)) == v2f(0.5f, 0.5f));
        REQUIRE(math::point_to_normalized(b2f(1.f, 2.f, -11.f, -22.f), v2f(1.f, 2.f)) == v2f(1.f, 1.f));
    }
}
