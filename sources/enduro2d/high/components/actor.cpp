/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/actor.hpp>

namespace e2d
{
    const char* factory_loader<actor>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "translation" : { "$ref": "#/common_definitions/v2" },
            "rotation" : { "type" : "number" },
            "scale" : { "$ref": "#/common_definitions/v2" }
        }
    })json";

    bool factory_loader<actor>::operator()(
        actor& component,
        const fill_context& ctx) const
    {
        if ( !component.node() ) {
            component.node(node::create());
        }

        if ( ctx.root.HasMember("translation") ) {
            v2f translation = component.node()->translation();
            if ( !json_utils::try_parse_value(ctx.root["translation"], translation) ) {
                the<debug>().error("ACTOR: Incorrect formatting of 'translation' property");
                return false;
            }
            component.node()->translation(translation);
        }

        if ( ctx.root.HasMember("rotation") ) {
            f32 rotation = component.node()->rotation();
            if ( !json_utils::try_parse_value(ctx.root["rotation"], rotation) ) {
                the<debug>().error("ACTOR: Incorrect formatting of 'rotation' property");
                return false;
            }
            component.node()->rotation(rotation);
        }

        if ( ctx.root.HasMember("scale") ) {
            v2f scale = component.node()->scale();
            if ( !json_utils::try_parse_value(ctx.root["scale"], scale) ) {
                the<debug>().error("ACTOR: Incorrect formatting of 'scale' property");
                return false;
            }
            component.node()->scale(scale);
        }

        return true;
    }

    bool factory_loader<actor>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<actor>::title = ICON_FA_CUBE " actor";

    void component_inspector<actor>::operator()(gcomponent<actor>& c) const {
        if ( node_iptr n = c->node() ) {
            if ( v2f translation = n->translation();
                ImGui::DragFloat2("translation", translation.data(), 1.f) )
            {
                n->translation(translation);
            }

            if ( f32 rotation = n->rotation() * math::rad_to_deg<f32>();
                ImGui::DragFloat("rotation", &rotation, 0.1f) )
            {
                n->rotation(rotation * math::deg_to_rad<f32>());
            }

            if ( v2f scale = n->scale();
                ImGui::DragFloat2("scale", scale.data(), 0.01f) )
            {
                n->scale(scale);
            }
        }
    }
}
