/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_high.hpp"
using namespace e2d;

namespace
{
    class safe_starter_initializer final : private noncopyable {
    public:
        safe_starter_initializer() {
            modules::initialize<starter>(0, nullptr,
                starter::parameters(
                    engine::parameters("luasol_untests", "enduro2d")
                        .without_graphics(true)));
        }

        ~safe_starter_initializer() noexcept {
            modules::shutdown<starter>();
        }
    };
}

TEST_CASE("luasol") {
    safe_starter_initializer initializer;
    luasol& l = the<luasol>();

    SECTION("vec2/vec3/vec4") {
        v2f r0 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local v = v2f.new(1,2)
                return v2f.new((v + v + 2).y)
            )lua");
        });
        v3f r1 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local v = v3f.new(1,2,3)
                return v3f.new((v + v + 2).y)
            )lua");
        });
        v4f r2 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local v = v4f.new(1,2,3,4)
                return v4f.new((v + v + 2).y)
            )lua");
        });
        REQUIRE(r0 == v2f(6));
        REQUIRE(r1 == v3f(6));
        REQUIRE(r2 == v4f(6));
    }

    SECTION("mat2/mat2/mat3") {
        std::pair<m2f, bool> r0 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local m = m2f.make_scale(2,3)
                local rm, s = m2f.inversed(m)
                return rm * m2f.identity(), s
            )lua");
        });
        std::pair<m3f, bool> r1 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local m = m3f.make_rotation(45,2,3,4)
                local rm, s = m3f.inversed(m)
                return rm * m3f.identity(), s
            )lua");
        });
        std::pair<m4f, bool> r2 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local m = m4f.make_translation(2,3,4)
                local rm, s = m4f.inversed(m)
                return rm * m4f.identity(), s
            )lua");
        });
        REQUIRE(r0.second);
        REQUIRE(r0.first == math::inversed(math::make_scale_matrix2(2.f,3.f)).first);
        REQUIRE(r1.second);
        REQUIRE(r1.first == math::inversed(math::make_rotation_matrix3(radf(45.f),2.f,3.f,4.f)).first);
        REQUIRE(r2.second);
        REQUIRE(r2.first == math::inversed(math::make_translation_matrix4(2.f,3.f,4.f)).first);
    }

    SECTION("rect/aabb") {
        bool r0 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local b = b2f.unit() * 2
                return b:inside(v2f.new(1.5,1.5))
            )lua");
        });
        REQUIRE(r0);
        bool r1 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local b = b3f.unit() * 2
                return b:overlaps(b3f.new(1.5,1.5,1.5,2,2,2))
            )lua");
        });
        REQUIRE(r1);
    }

    SECTION("trs2/trs3") {
        f32 r0 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local t = t2f.make_rotation(0.5)
                return t.rotation
            )lua");
        });
        REQUIRE(math::approximately(r0, 0.5f));
        v3f r1 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local t = t3f.make_translation(v3f.new(1,2,3))
                return t.translation
            )lua");
        });
        REQUIRE(r1 == v3f(1,2,3));
    }

    SECTION("color/color32") {
        color r0 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                return color.white() * 0.5
            )lua");
        });
        REQUIRE(r0 == color(0.5f,0.5f,0.5f,0.5f));
        color32 r1 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                return color32.white() - 1
            )lua");
        });
        REQUIRE(r1 == color32(254,254,254,254));
    }

    SECTION("str_hash") {
        str_hash r0 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local s = str_hash.new("hello")
                s:clear()
                return s
            )lua");
        });
        REQUIRE(r0.empty());
        u32 r1 = l.with_state([](sol::state& lua){
            return lua.script(R"lua(
                local s = str_hash.new("hello")
                return s.hash
            )lua");
        });
        REQUIRE(r1 == str_hash("hello").hash());
    }
}
