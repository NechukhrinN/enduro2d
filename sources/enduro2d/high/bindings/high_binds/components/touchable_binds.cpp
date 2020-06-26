/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/touchable.hpp>

namespace e2d::bindings::high
{
    void bind_touchable(sol::state& l) {
        l.new_usertype<gcomponent<touchable>>("touchable",
            sol::no_constructor,

            "enable", [](gcomponent<touchable>& c){
                c.component<disabled<touchable>>().remove();
            },

            "disable", [](gcomponent<touchable>& c){
                c.component<disabled<touchable>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return !c.component<disabled<touchable>>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<touchable>>().remove();
                    } else {
                        c.component<disabled<touchable>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.component<disabled<touchable>>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<touchable>>().ensure();
                    } else {
                        c.component<disabled<touchable>>().remove();
                    }
                }
            ),

            "bubbling", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c->bubbling();
                },
                [](gcomponent<touchable>& c, bool v){
                    c->bubbling(v);
                }),

            "capturing", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c->capturing();
                },
                [](gcomponent<touchable>& c, bool v){
                    c->capturing(v);
                })
        );

        //
        // events
        //

        l["touchable"].get_or_create<sol::table>()
        .new_usertype<touchable_events::mouse_evt>("mouse_evt",
            sol::no_constructor,

            "target", sol::property(
                [](const touchable_events::mouse_evt& c) -> gobject {
                    return c.target();
                }),

            "bubbling", sol::property(
                [](const touchable_events::mouse_evt& c) -> bool {
                    return c.bubbling();
                }),

            "capturing", sol::property(
                [](const touchable_events::mouse_evt& c) -> bool {
                    return c.capturing();
                }),

            "type", sol::property(
                [](const touchable_events::mouse_evt& c) -> str {
                    return str(enum_hpp::to_string_or_throw(c.type()));
                }),

            "button", sol::property(
                [](const touchable_events::mouse_evt& c) -> str {
                    return str(enum_hpp::to_string_or_throw(c.button()));
                })
        );

        l["touchable"].get_or_create<sol::table>()
        .new_usertype<touchable_events::touch_evt>("touch_evt",
            sol::no_constructor,

            "target", sol::property(
                [](const touchable_events::touch_evt& c) -> gobject {
                    return c.target();
                }),

            "bubbling", sol::property(
                [](const touchable_events::touch_evt& c) -> bool {
                    return c.bubbling();
                }),

            "capturing", sol::property(
                [](const touchable_events::touch_evt& c) -> bool {
                    return c.capturing();
                }),

            "type", sol::property(
                [](const touchable_events::touch_evt& c) -> str {
                    return str(enum_hpp::to_string_or_throw(c.type()));
                }),

            "finger", sol::property(
                [](const touchable_events::touch_evt& c) -> u32 {
                    return c.finger();
                })
        );
    }
}
