/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/sprite_renderer.hpp>

namespace e2d
{
    const char* factory_loader<sprite_renderer>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "tint" : { "$ref": "#/common_definitions/color" },
            "scale" : { "$ref": "#/common_definitions/v2" },
            "mode" : { "$ref": "#/definitions/modes" },
            "blending" : { "$ref": "#/definitions/blendings" },
            "filtering" : { "type" : "boolean" },
            "atlas" : { "$ref": "#/common_definitions/address" },
            "sprite" : { "$ref": "#/common_definitions/address" },
            "materials" : { "$ref": "#/definitions/materials" }
        },
        "definitions" : {
            "modes" : {
                "type" : "string",
                "enum" : [
                    "simple",
                    "sliced"
                ]
            },
            "blendings" : {
                "type" : "string",
                "enum" : [
                    "normal",
                    "additive",
                    "multiply",
                    "screen"
                ]
            },
            "materials" : {
                "type" : "object",
                "required" : [],
                "additionalProperties" : false,
                "properties" : {
                    "normal" : { "$ref": "#/common_definitions/address" },
                    "additive" : { "$ref": "#/common_definitions/address" },
                    "multiply" : { "$ref": "#/common_definitions/address" },
                    "screen" : { "$ref": "#/common_definitions/address" }
                }
            }
        }
    })json";

    bool factory_loader<sprite_renderer>::operator()(
        sprite_renderer& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("tint") ) {
            color32 tint = component.tint();
            if ( !json_utils::try_parse_value(ctx.root["tint"], tint) ) {
                the<debug>().error("SPRITE_RENDERER: Incorrect formatting of 'tint' property");
                return false;
            }
            component.tint(tint);
        }

        if ( ctx.root.HasMember("scale") ) {
            v2f scale = component.scale();
            if ( !json_utils::try_parse_value(ctx.root["scale"], scale) ) {
                the<debug>().error("SPRITE_RENDERER: Incorrect formatting of 'scale' property");
                return false;
            }
            component.scale(scale);
        }

        if ( ctx.root.HasMember("mode") ) {
            sprite_renderer::modes mode = component.mode();
            if ( !json_utils::try_parse_value(ctx.root["mode"], mode) ) {
                the<debug>().error("SPRITE_RENDERER: Incorrect formatting of 'mode' property");
                return false;
            }
            component.mode(mode);
        }

        if ( ctx.root.HasMember("blending") ) {
            sprite_renderer::blendings blending = component.blending();
            if ( !json_utils::try_parse_value(ctx.root["blending"], blending) ) {
                the<debug>().error("SPRITE_RENDERER: Incorrect formatting of 'blending' property");
                return false;
            }
            component.blending(blending);
        }

        if ( ctx.root.HasMember("filtering") ) {
            bool filtering = component.filtering();
            if ( !json_utils::try_parse_value(ctx.root["filtering"], filtering) ) {
                the<debug>().error("SPRITE_RENDERER: Incorrect formatting of 'filtering' property");
                return false;
            }
            component.filtering(filtering);
        }

        if ( ctx.root.HasMember("atlas") ) {
            auto sprite = ctx.dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(ctx.parent_address, ctx.root["atlas"].GetString()));
            if ( !sprite ) {
                the<debug>().error("SPRITE_RENDERER: Dependency 'atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["atlas"].GetString());
                return false;
            }
            component.sprite(sprite);
        }

        if ( ctx.root.HasMember("sprite") ) {
            auto sprite = ctx.dependencies.find_asset<sprite_asset>(
                path::combine(ctx.parent_address, ctx.root["sprite"].GetString()));
            if ( !sprite ) {
                the<debug>().error("SPRITE_RENDERER: Dependency 'sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["sprite"].GetString());
                return false;
            }
            component.sprite(sprite);
        }

        if ( ctx.root.HasMember("materials") ) {
            const rapidjson::Value& materials_root = ctx.root["materials"];
            flat_map<str_hash, material_asset::ptr> materials;
            materials.reserve(materials_root.MemberCount());
            for ( rapidjson::Value::ConstMemberIterator material_root = materials_root.MemberBegin();
                material_root != materials_root.MemberEnd();
                ++material_root )
            {
                str_hash material_name;
                if ( !json_utils::try_parse_value(material_root->name, material_name) ) {
                    the<debug>().error("SPRITE_RENDERER: Incorrect formatting of 'material.name' property");
                    return false;
                }

                str material_address;
                if ( !json_utils::try_parse_value(material_root->value, material_address) ) {
                    the<debug>().error("SPRITE_RENDERER: Incorrect formatting of 'material.address' property");
                    return false;
                }

                auto material = ctx.dependencies.find_asset<material_asset>(
                    path::combine(ctx.parent_address, material_address));
                if ( !material ) {
                    the<debug>().error("SPRITE_RENDERER: Dependency 'material' is not found:\n"
                        "--> Parent address: %0\n"
                        "--> Dependency address: %1",
                        ctx.parent_address,
                        material_address);
                    return false;
                }
                materials.emplace(material_name, material);
            }
            component.materials(std::move(materials));
        }

        return true;
    }

    bool factory_loader<sprite_renderer>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(ctx.parent_address, ctx.root["atlas"].GetString()));
        }

        if ( ctx.root.HasMember("sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(ctx.parent_address, ctx.root["sprite"].GetString()));
        }

        if ( ctx.root.HasMember("materials") ) {
            const rapidjson::Value& materials_root = ctx.root["materials"];
            for ( rapidjson::Value::ConstMemberIterator material_root = materials_root.MemberBegin();
                material_root != materials_root.MemberEnd();
                ++material_root )
            {
                dependencies.add_dependency<material_asset>(
                    path::combine(ctx.parent_address, material_root->value.GetString()));
            }
        }

        return true;
    }
}

namespace e2d
{
    const char* component_inspector<sprite_renderer>::title = ICON_FA_IMAGE " sprite_renderer";

    void component_inspector<sprite_renderer>::operator()(gcomponent<sprite_renderer>& c) const {
        if ( color tint = color(c->tint());
            ImGui::ColorEdit4("tint", tint.data()) )
        {
            c->tint(color32(tint));
        }

        if ( v2f scale = c->scale();
            ImGui::DragFloat2("scale", scale.data(), 0.01f) )
        {
            c->scale(scale);
        }

        if ( sprite_renderer::modes mode = c->mode();
            imgui_utils::show_enum_combo_box("mode", &mode) )
        {
            c->mode(mode);
        }

        if ( sprite_renderer::blendings blending = c->blending();
            imgui_utils::show_enum_combo_box("blending", &blending) )
        {
            c->blending(blending);
        }

        if ( bool filtering = c->filtering();
            ImGui::Checkbox("filtering", &filtering) )
        {
            c->filtering(filtering);
        }

        ///TODO(BlackMat): add 'sprite' inspector
        ///TODO(BlackMat): add 'materials' inspector
    }

    void component_inspector<sprite_renderer>::operator()(
        gcomponent<sprite_renderer>& c,
        gizmos_context& ctx) const
    {
        if ( const sprite_asset::ptr& spr_a = c->sprite() ) {
            const sprite& spr = spr_a->content();

            const b2f& outer_r = spr.outer_texrect();
            const v2f size = outer_r.size * c->scale();

            ctx.draw_wire_rect(
                size * 0.5f,
                size,
                ctx.selected() ? color32::yellow() : color32::magenta());
        }
    }
}
