/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class layout final {
    public:
        layout() = default;
    };

    class layout_item final {
    public:
        layout_item() = default;
    };
}

namespace e2d
{
    template <>
    class factory_loader<layout> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            layout& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<layout_item> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            layout_item& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<layout> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<layout>& c) const;
    };

    template <>
    class component_inspector<layout_item> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<layout_item>& c) const;
    };
}

namespace e2d
{
}
