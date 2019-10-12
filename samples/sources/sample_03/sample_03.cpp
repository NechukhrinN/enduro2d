/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

namespace
{
    struct rotator {
        v3f axis;
    };

    class game_system final : public ecs::system {
    public:
        void process(ecs::registry& owner) override {
            E2D_UNUSED(owner);
            const keyboard& k = the<input>().keyboard();

            if ( k.is_key_just_released(keyboard_key::f12) ) {
                the<dbgui>().toggle_visible(!the<dbgui>().visible());
            }

            if ( k.is_key_just_released(keyboard_key::escape) ) {
                the<window>().set_should_close(true);
            }

            if ( k.is_key_pressed(keyboard_key::lsuper) && k.is_key_just_released(keyboard_key::enter) ) {
                the<window>().toggle_fullscreen(!the<window>().fullscreen());
            }
        }
    };

    class camera_system final : public ecs::system {
    public:
        void process(ecs::registry& owner) override {
            owner.for_joined_components<camera>(
            [](const ecs::const_entity&, camera& cam){
                if ( !cam.target() ) {
                    cam.viewport(
                        the<window>().real_size());
                    cam.projection(math::make_orthogonal_lh_matrix4(
                        the<window>().real_size().cast_to<f32>(), 0.f, 1000.f));
                }
            });
        }
    };

    class rotator_system final : public ecs::system {
    public:
        void process(ecs::registry& owner) override {
            const f32 time = the<engine>().time();
            owner.for_joined_components<rotator, actor>(
                [&time](const ecs::const_entity&, const rotator& rot, actor& act){
                    const node_iptr node = act.node();
                    if ( node ) {
                        const q4f q = math::make_quat_from_axis_angle(make_rad(time), rot.axis);
                        node->rotation(q);
                    }
                });
        }
    };

    class game final : public starter::application {
    public:
        bool initialize() final {
            return create_scene()
                && create_camera()
                && create_systems();
        }
    private:
        bool create_scene() {
            auto model_res = the<library>().load_asset<model_asset>("models/gnome/gnome_model.json");
            auto model_mat = the<library>().load_asset<material_asset>("models/gnome/gnome_material.json");
            auto sprite_res = the<library>().load_asset<sprite_asset>("sprites/ship_sprite.json");
            auto sprite_mat = the<library>().load_asset<material_asset>("materials/sprite_material_normal.json");
            auto flipbook_res = the<library>().load_asset<flipbook_asset>("sprites/cube_flipbook.json");

            if ( !model_res || !model_mat || !sprite_res || !sprite_mat || !flipbook_res ) {
                return false;
            }

            auto scene_i = the<world>().instantiate();
            scene_i.component<scene>().assign();

            {
                prefab prefab;
                prefab.prototype()
                    .component<rotator>(rotator{v3f::unit_y()})
                    .component<renderer>(renderer().materials({model_mat}))
                    .component<model_renderer>(model_res);

                the<world>().instantiate(
                    prefab,
                    scene_i,
                    make_trs3(v3f{0,50.f,0}, q4f::identity(), v3f{20.f}));
            }

            {
                prefab prefab;
                prefab.prototype()
                    .component<rotator>(rotator{v3f::unit_z()})
                    .component<renderer>()
                    .component<sprite_renderer>(sprite_renderer(sprite_res)
                        .materials({{"normal", sprite_mat}}));

                the<world>().instantiate(
                    prefab,
                    scene_i,
                    math::make_translation_trs3(v3f{0,-50.f,0}));
            }

            {
                prefab prefab_a;
                prefab_a.prototype()
                    .component<rotator>(rotator{v3f::unit_z()})
                    .component<renderer>()
                    .component<sprite_renderer>(sprite_renderer()
                        .filtering(false)
                        .materials({{"normal", sprite_mat}}))
                    .component<flipbook_player>(flipbook_player(flipbook_res)
                        .play("idle")
                        .looped(true));

                for ( std::size_t i = 0; i < 2; ++i )
                for ( std::size_t j = 0; j < 5; ++j ) {
                    t3f trans{
                        {-80.f + j * 40.f, -200.f + i * 40.f, 0},
                        q4f::identity(),
                        {2.f,2.f,1.f}};
                    gobject inst = the<world>().instantiate(prefab_a, scene_i, trans);

                    prefab prefab_b = prefab_a;
                    prefab_b.prototype()
                        .component<rotator>(rotator{v3f::unit_z()})
                        .component<actor>(node::create(make_trs3(
                            v3f{20.f,0.f,0.f},
                            q4f::identity(),
                            v3f{0.3f,0.3f,3.f})));

                    the<world>().instantiate(prefab_b, inst);
                }
            }

            return true;
        }

        bool create_camera() {
            auto camera_i = the<world>().instantiate();
            camera_i.component<camera>().assign(camera()
                .background({1.f, 0.4f, 0.f, 1.f}));
            camera_i.component<actor>().assign(node::create(camera_i));
            return true;
        }

        bool create_systems() {
            ecs::registry_filler(the<world>().registry())
                .system<game_system>(world::priority_update)
                .system<rotator_system>(world::priority_update)
                .system<camera_system>(world::priority_pre_render);
            return true;
        }
    };
}

int e2d_main(int argc, char *argv[]) {
    const auto starter_params = starter::parameters(
        engine::parameters("sample_03", "enduro2d")
            .timer_params(engine::timer_parameters()
                .maximal_framerate(100)));
    modules::initialize<starter>(argc, argv, starter_params).start<game>();
    modules::shutdown<starter>();
    return 0;
}
