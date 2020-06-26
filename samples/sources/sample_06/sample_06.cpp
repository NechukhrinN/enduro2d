/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

namespace
{
    class game_system final : public systems::update_system {
    public:
        void process(
            ecs::registry& owner,
            const systems::update_event& event) override
        {
            E2D_UNUSED(owner, event);
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

            // use keys R, J, G to start animations
            const bool roar = k.is_key_just_pressed(keyboard_key::r);
            const bool jump = k.is_key_just_pressed(keyboard_key::j);
            const bool gun_grab = k.is_key_just_pressed(keyboard_key::g);

            if ( roar || jump || gun_grab ) {
                owner.for_each_component<spine_player>([
                    roar, jump, gun_grab
                ](ecs::entity e, const spine_player& p) {
                    if ( roar && p.has_animation("roar") ) {
                        e.ensure_component<commands<spine_player_commands::command>>()
                            .add(spine_player_commands::set_anim_cmd(0, "roar")
                                .complete_message("to_walk"));
                    } else if ( jump && p.has_animation("jump") ) {
                        e.ensure_component<commands<spine_player_commands::command>>()
                            .add(spine_player_commands::set_anim_cmd(0, "jump")
                                .complete_message("to_walk"));
                    } else if ( gun_grab && p.has_animation("gun-grab") ) {
                        e.ensure_component<commands<spine_player_commands::command>>()
                            .add(spine_player_commands::set_anim_cmd(1, "gun-grab"))
                            .add(spine_player_commands::add_anim_cmd(1, "gun-holster").delay(3.f));
                    }
                });
            }

            owner.for_joined_components<events<spine_player_events::event>>([
            ](ecs::entity e, const events<spine_player_events::event>& pe) {
                for ( const auto& evt : pe.get() ) {
                    if ( auto complete = std::get_if<spine_player_events::complete_evt>(&evt);
                        complete && complete->message() == "to_walk" )
                    {
                        e.ensure_component<commands<spine_player_commands::command>>()
                            .add(spine_player_commands::add_anim_cmd(0, "walk")
                                .loop(true));
                    }
                }
            });
        }
    };

    class game final : public starter::application {
    public:
        bool initialize() final {
            return create_scene()
                && create_systems();
        }
    private:
        bool create_scene() {
            auto scene_prefab_res = the<library>().load_asset<prefab_asset>("scenes/sample_06.json");
            auto scene_go = scene_prefab_res
                ? the<world>().instantiate(scene_prefab_res->content())
                : gobject();
            return scene_go.valid();
        }

        bool create_systems() {
            ecs::registry_filler(the<world>().registry())
            .feature<struct game_feature>(ecs::feature()
                .add_system<game_system>());
            return true;
        }
    };
}

int e2d_main(int argc, char *argv[]) {
    const auto starter_params = starter::parameters(
        engine::parameters("sample_06", "enduro2d")
            .window_params(engine::window_parameters()
                .size({1024, 768}))
            .timer_params(engine::timer_parameters()
                .maximal_framerate(100)));
    modules::initialize<starter>(argc, argv, starter_params).start<game>();
    modules::shutdown<starter>();
    return 0;
}
