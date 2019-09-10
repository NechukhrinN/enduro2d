/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../core/_all.hpp"

#include <ecs.hpp/ecs.hpp>
#include <3rdparty/sol/sol.hpp>

namespace e2d
{
    namespace ecs
    {
        using namespace ecs_hpp;
    }
}

namespace e2d
{
    class atlas_asset;
    class binary_asset;
    class flipbook_asset;
    class font_asset;
    class image_asset;
    class json_asset;
    class material_asset;
    class mesh_asset;
    class model_asset;
    class prefab_asset;
    class script_asset;
    class shader_asset;
    class shape_asset;
    class sound_asset;
    class spine_asset;
    class sprite_asset;
    class text_asset;
    class texture_asset;
    class xml_asset;

    class actor;
    class behaviour;
    class camera;
    class flipbook_player;
    class label;
    class model_renderer;
    class renderer;
    class scene;
    class spine_player;
    class spine_player_cmd;
    class spine_player_evt;
    class sprite_renderer;

    class flipbook_system;
    class label_system;
    class render_system;
    class spine_pre_system;
    class spine_post_system;

    template < typename Asset, typename Content >
    class content_asset;
    class asset;

    class factory;
    class library;
    class asset_cache;
    class asset_group;
    class asset_dependencies;

    class atlas;
    class flipbook;
    class gobject;
    class model;
    class node;
    class prefab;
    class script;
    class spine;
    class sprite;

    class luasol;
    class starter;
    class world;
}
