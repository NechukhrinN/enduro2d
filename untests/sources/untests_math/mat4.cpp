/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("mat4") {
    {
        {
            auto m = m4i::identity();
            REQUIRE(m.rows[0][0] == 1);
            REQUIRE(m[2][2] == 1);
            REQUIRE((m[0][0] == m[1][1] && m[1][1] == m[2][2] && m[2][2] == m[3][3]));
            REQUIRE((m.data()[0] == m.data()[5] && m.data()[5] == m.data()[10]));
            REQUIRE((m.data()[10] == m.data()[15] && m.data()[15] == 1));
            REQUIRE((m[0][1] == 0 && m[0][2] == 0 && m[0][3] == 0));
            REQUIRE((m[1][0] == 0 && m[1][2] == 0 && m[1][3] == 0));
            REQUIRE((m.data()[1] == 0 && m.data()[2] == 0 && m.data()[3] == 0 && m.data()[4] == 0));
            REQUIRE((m.data()[6] == 0 && m.data()[7] == 0 && m.data()[8] == 0 && m.data()[9] == 0));
            m[0] = v4i::unit(); m[1] = v4i::unit() * 2;
            REQUIRE((m[0] == v4i::unit() && m[1] == v4i::unit() * 2));
            m[1][2] = 3; m[2][1] = 4;
            REQUIRE((m.rows[1][2] == 3 && m.rows[2][1] == 4));
            REQUIRE((m.data()[1*4+2] == 3 && m.data()[2*4+1] == 4));
        }
        REQUIRE(math::make_scale_matrix4(1,1,1) == math::make_scale_matrix4(v3i::unit()));
        REQUIRE(math::make_scale_matrix4(v2i(1,1),1) == math::make_scale_matrix4(v3i::unit()));

        REQUIRE(math::make_translation_matrix4(1,1,1) == math::make_translation_matrix4(v3i::unit()));
        REQUIRE(math::make_translation_matrix4(v2i(1,1),1) == math::make_translation_matrix4(v3i::unit()));

        REQUIRE(math::make_rotation_matrix4(make_rad(1.f),1.f,1.f,1.f) == math::make_rotation_matrix4(make_rad(1.f), v3f::unit()));
        REQUIRE(math::make_rotation_matrix4(make_rad(1.f),v3f(1.f,1.f,1.f)) == math::make_rotation_matrix4(make_rad(1.f), v3f::unit()));

        REQUIRE(math::make_orthographic_lh_matrix4(640.f, 480.f, 0.f, 1.f) == math::make_orthographic_lh_matrix4(v2f(640,480), 0.f, 1.f));
        REQUIRE(math::make_orthographic_lh_matrix4(-320.f, 320.f, -240.f, 240.f, 0.f, 1.f) == math::make_orthographic_lh_matrix4(-v2f(320,240), v2f(320,240), 0.f, 1.f));
        REQUIRE(math::make_orthographic_lh_matrix4(640.0, 480.0, 0.0, 1.0) == math::make_orthographic_lh_matrix4(v2d(640,480), 0.0, 1.0));
        REQUIRE(math::make_orthographic_lh_matrix4(-320.0, 320.0, -240.0, 240.0, 0.0, 1.0) == math::make_orthographic_lh_matrix4(-v2d(320,240), v2d(320,240), 0.0, 1.0));

        REQUIRE(math::make_orthographic_rh_matrix4(640.f, 480.f, 0.f, 1.f) == math::make_orthographic_rh_matrix4(v2f(640,480), 0.f, 1.f));
        REQUIRE(math::make_orthographic_rh_matrix4(-320.f, 320.f, -240.f, 240.f, 0.f, 1.f) == math::make_orthographic_rh_matrix4(-v2f(320,240), v2f(320,240), 0.f, 1.f));
        REQUIRE(math::make_orthographic_rh_matrix4(640.0, 480.0, 0.0, 1.0) == math::make_orthographic_rh_matrix4(v2d(640,480), 0.0, 1.0));
        REQUIRE(math::make_orthographic_rh_matrix4(-320.0, 320.0, -240.0, 240.0, 0.0, 1.0) == math::make_orthographic_rh_matrix4(-v2d(320,240), v2d(320,240), 0.0, 1.0));
    }
    {
        REQUIRE(m4f(m4f::identity()) == m4f::identity());
        m4f m0(m4f::identity());
        m0 = m4f::zero();
        REQUIRE(m0 == m4f::zero());

        REQUIRE(-m4f::identity() == math::make_scale_matrix4(1.f, 1.f, 1.f) * -1.f);

        REQUIRE(math::make_scale_matrix4(1.1f, 2.2f, 3.3f).cast_to<i32>() ==
                math::make_scale_matrix4(1, 2, 3));
    }
    {
        REQUIRE(m4f() == m4f(1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1));

        REQUIRE(math::make_scale_matrix4(2.f,3.f,4.f) + 2.f == m4f(4, 2, 2, 2,
                                                                   2, 5, 2, 2,
                                                                   2, 2, 6, 2,
                                                                   2, 2, 2, 3));

        REQUIRE(math::make_scale_matrix4(2, 3, 4) * 2 == m4i(4, 0, 0, 0,
                                                             0, 6, 0, 0,
                                                             0, 0, 8, 0,
                                                             0, 0, 0, 2));

        m4f m0 = math::make_scale_matrix4(2.f, 3.f, 4.f);
        m0 += 2.f;
        REQUIRE(m0 == math::make_scale_matrix4(2.f, 3.f, 4.f) + 2.f);

        m4f m2 = math::make_scale_matrix4(2.f, 3.f, 4.f);
        m2 *= 2.f;
        REQUIRE(m2 == math::make_scale_matrix4(2.f, 3.f, 4.f) * 2.f);
    }
    {
        REQUIRE(math::make_scale_matrix4(2, 3, 4) +
                math::make_scale_matrix4(2, 3, 4) ==
                math::make_scale_matrix4(2, 3, 4) * 2);

        REQUIRE(math::make_scale_matrix4(2, 3, 4) *
                math::make_scale_matrix4(2, 3, 4) ==
                math::make_scale_matrix4(4, 9, 16));

        m4f m0 = math::make_scale_matrix4(2.f, 3.f, 4.f);
        m0 += math::make_scale_matrix4(2.f, 3.f, 4.f);
        REQUIRE(m0 == math::make_scale_matrix4(2, 3, 4).cast_to<f32>() * 2.f);

        m4f m2 = math::make_scale_matrix4(2.f, 3.f, 4.f);
        m2 *= math::make_scale_matrix4(2.f, 3.f, 4.f);
        REQUIRE(m2 == math::make_scale_matrix4(4, 9, 16).cast_to<f32>());
    }
    {
        REQUIRE(v4f(2,3,4,5) * math::make_scale_matrix4(3.f,4.f,5.f) == v4f(6.f,12.f,20.f,5.f));
        REQUIRE(v4f(2,3,4,1) * math::make_translation_matrix4(3.f,4.f,5.f) == v4f(2+3,3+4,4+5,1));
        REQUIRE(v4d(2,3,4,2) * math::make_translation_matrix4(3.0,4.0,5.0) == v4d(2+3*2,3+4*2,4+5*2,2));
        REQUIRE(math::approximately(
            v4f(20,30,10,1) * math::make_rotation_matrix4(math::half_pi<f32>(), v3f::unit_z()),
            v4f(-30.f,20.f,10,1)));
    }
    {
        auto m = math::make_scale_matrix4(2.f,3.f,4.f);
        m[3][3] = 0.5f;
        REQUIRE(v4f(2,3,4,0) * m == v4f(4,9,16,0));
        REQUIRE(v4f(2,3,4,1) * m == v4f(4,9,16,0.5));
    }
    {
        REQUIRE(math::transposed(m4f::identity()) == m4f::identity());
        REQUIRE(math::transposed(m4i( 1, 2, 3, 4,
                                      5, 6, 7, 8,
                                      9,10,11,12,
                                     13,14,15,16)) == m4i(1,5, 9,13,
                                                          2,6,10,14,
                                                          3,7,11,15,
                                                          4,8,12,16));
    }
    {
        REQUIRE(
            math::inversed(math::make_scale_matrix4(2.f,4.f)).first ==
            math::make_scale_matrix4(0.5f, 0.25f));
        REQUIRE(
            math::inversed(math::make_translation_matrix4(10.f, 20.f)).first ==
            math::make_translation_matrix4(-10.f, -20.f));

        auto m0 = math::make_scale_matrix4(2.f,4.f,5.f) *
                  math::make_rotation_matrix4(make_rad(1.f), v3f::unit_z()) *
                  math::make_translation_matrix4(30.f, 40.f);
        REQUIRE(math::inversed(m0).first * m0 == m4f::identity());
        REQUIRE(m0 * math::inversed(m0).first == m4f::identity());

        REQUIRE(math::inversed(m4f::zero()).second == false);
        REQUIRE(math::inversed(m4f::identity()).second == true);
    }
    {
        REQUIRE(math::make_trs_matrix4(math::make_translation_trs2(v2f(1.f,2.f)))
            == math::make_translation_matrix4(1.f,2.f));
        REQUIRE(math::make_trs_matrix4(math::make_translation_trs3(v3f(1.f,2.f,3.f)))
            == math::make_translation_matrix4(1.f,2.f,3.f));

        REQUIRE(math::make_trs_matrix4(math::make_rotation_trs2(3.f))
            == math::make_rotation_matrix4(make_rad(3.f), v3f::unit_z()));

        REQUIRE(math::make_trs_matrix4(math::make_scale_trs2(v2f(1.f,2.f)))
            == math::make_scale_matrix4(1.f,2.f));
        REQUIRE(math::make_trs_matrix4(math::make_scale_trs3(v3f(1.f,2.f,3.f)))
            == math::make_scale_matrix4(1.f,2.f,3.f));

        auto m0 = math::make_scale_matrix4(2.f,4.f,5.f) *
                  math::make_rotation_matrix4(make_rad(1.f), v3f::unit_z()) *
                  math::make_translation_matrix4(30.f, 40.f, 50.f);
        REQUIRE(m0 == math::make_trs_matrix4(make_trs3(
            v3f(30.f, 40.f, 50.f),
            v3f::unit_z(),
            v3f(2.f,4.f,5.f))));
    }
    {
        -m4i::identity();
        m4i::identity() + 2;
        m4i::identity() * 2;
        m4i::identity() + m4i::identity();
        m4i::identity() * m4i::identity();
        v4i::unit() * m4i::identity();
        REQUIRE(m4i::identity() == m4i::identity());
        REQUIRE_FALSE(m4i::identity() != m4i::identity());
    }
    {
        m4f projection = math::make_perspective_lh_matrix4(make_deg(80.f), 4.f / 3.f, 0.01f, 1.f);
        const m4f inv_projection = math::inversed(projection).first;
        b2f viewport = make_rect(800.f, 600.f);

        const v3f p1 = v3f(0.f,0.f,0.01f);
        const v3f p2 = v3f(+1.f,0.f,0.01f);
        const v3f p3 = v3f(-1.f,0.f,0.01f);

        REQUIRE(math::project(p1, projection, viewport).second);
        REQUIRE(math::project(p2, projection, viewport).second);
        REQUIRE(math::project(p3, projection, viewport).second);

        REQUIRE(math::impl::project_zo(p1, projection, viewport).second);
        REQUIRE(math::impl::project_zo(p2, projection, viewport).second);
        REQUIRE(math::impl::project_zo(p3, projection, viewport).second);

        REQUIRE(math::impl::project_no(p1, projection, viewport).second);
        REQUIRE(math::impl::project_no(p2, projection, viewport).second);
        REQUIRE(math::impl::project_no(p3, projection, viewport).second);

        REQUIRE(math::unproject(
            math::project(p1, projection, viewport).first,
            inv_projection,
            viewport).second);

        REQUIRE(math::unproject(
            math::project(p2, projection, viewport).first,
            inv_projection,
            viewport).second);

        REQUIRE(math::unproject(
            math::project(p3, projection, viewport).first,
            inv_projection,
            viewport).second);

        REQUIRE(math::unproject(
            math::project(p1, projection, viewport).first,
            inv_projection,
            viewport).first == v3f(p1));

        REQUIRE(math::unproject(
            math::project(p2, projection, viewport).first,
            inv_projection,
            viewport).first == v3f(p2));

        REQUIRE(math::unproject(
            math::project(p3, projection, viewport).first,
            inv_projection,
            viewport).first == v3f(p3));
    }
    {
        m4f projection = math::make_orthographic_lh_matrix4(-200.f, 200.f, -300.f, 300.f, 0.f, 1.f);
        m4f model = math::make_scale_matrix4(2.f, 2.f);
        b2f viewport = make_rect(800.f, 600.f);

        const v3f p1 = v3f(0.f,0.f,0.f);
        const v3f p2 = v3f(+1.f,0.f,0.f);
        const v3f p3 = v3f(-1.f,0.f,0.f);

        REQUIRE(math::project(p1, model * projection, viewport).second);
        REQUIRE(math::project(p2, model * projection, viewport).second);
        REQUIRE(math::project(p3, model * projection, viewport).second);

        m4f projection_zo = math::impl::make_orthographic_lh_zo_matrix4(-200.f, 200.f, -150.f, 150.f, 0.f, 1.f);
        REQUIRE(math::impl::project_zo(p1, model * projection_zo, viewport).second);
        REQUIRE(math::impl::project_zo(p2, model * projection_zo, viewport).second);
        REQUIRE(math::impl::project_zo(p3, model * projection_zo, viewport).second);

        m4f projection_no = math::impl::make_orthographic_lh_no_matrix4(-200.f, 200.f, -150.f, 150.f, 0.f, 1.f);
        REQUIRE(math::impl::project_no(p1, model * projection_no, viewport).second);
        REQUIRE(math::impl::project_no(p2, model * projection_no, viewport).second);
        REQUIRE(math::impl::project_no(p3, model * projection_no, viewport).second);

        REQUIRE(math::approximately(
            math::project(p1, model * projection, viewport).first,
            v3f(400.f, 300.f, 0.f),
            0.01f));
        REQUIRE(math::approximately(
            math::project(p2, model * projection, viewport).first,
            v3f(404.f, 300.f, 0.f),
            0.01f));
        REQUIRE(math::approximately(
            math::project(p3, model * projection, viewport).first,
            v3f(396.f, 300.f, 0.f),
            0.01f));

        REQUIRE(math::approximately(
            math::impl::project_zo(p1, model * projection_zo, viewport).first,
            v3f(400.f, 300.f, 0.f),
            0.01f));
        REQUIRE(math::approximately(
            math::impl::project_zo(p2, model * projection_zo, viewport).first,
            v3f(404.f, 300.f, 0.f),
            0.01f));
        REQUIRE(math::approximately(
            math::impl::project_zo(p3, model * projection_zo, viewport).first,
            v3f(396.f, 300.f, 0.f),
            0.01f));

        REQUIRE(math::approximately(
            math::impl::project_no(p1, model * projection_no, viewport).first,
            v3f(400.f, 300.f, 0.f),
            0.01f));
        REQUIRE(math::approximately(
            math::impl::project_no(p2, model * projection_no, viewport).first,
            v3f(404.f, 300.f, 0.f),
            0.01f));
        REQUIRE(math::approximately(
            math::impl::project_no(p3, model * projection_no, viewport).first,
            v3f(396.f, 300.f, 0.f),
            0.01f));

        const m4f inv_project = math::inversed(model * projection).first;
        REQUIRE(math::inversed(model * projection).second);

        REQUIRE(math::unproject(
            math::project(p1, inv_project, viewport).first,
            model * projection,
            viewport).second);

        REQUIRE(math::unproject(
            math::project(p2, inv_project, viewport).first,
            model * projection,
            viewport).second);

        REQUIRE(math::unproject(
            math::project(p3, inv_project, viewport).first,
            model * projection,
            viewport).second);

        REQUIRE(math::unproject(
            math::project(p1, inv_project, viewport).first,
            model * projection,
            viewport).first == v3f(p1));

        REQUIRE(math::unproject(
            math::project(p2, inv_project, viewport).first,
            model * projection,
            viewport).first == v3f(p2));

        REQUIRE(math::unproject(
            math::project(p3, inv_project, viewport).first,
            model * projection,
            viewport).first == v3f(p3));
    }
}
