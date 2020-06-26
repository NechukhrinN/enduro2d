/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class scene final {
    public:
        scene() = default;

        scene& depth(i32 value) noexcept;
        [[nodiscard]] i32 depth() const noexcept;
    private:
        i32 depth_ = 0;
    };
}

namespace e2d
{
    template <>
    class factory_loader<scene> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            scene& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<scene> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<scene>& c) const;
    };
}

namespace e2d
{
    inline scene& scene::depth(i32 value) noexcept {
        depth_ = value;
        return *this;
    }

    inline i32 scene::depth() const noexcept {
        return depth_;
    }
}
