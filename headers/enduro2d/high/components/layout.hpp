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
        ENUM_HPP_CLASS_DECL(modes, u8,
            (vertical)
            (horizontal))

        ENUM_HPP_CLASS_DECL(valigns, u8,
            (top)
            (center)
            (bottom))

        ENUM_HPP_CLASS_DECL(haligns, u8,
            (left)
            (center)
            (right))
    public:
        layout() = default;

        layout& mode(modes value) noexcept;
        [[nodiscard]] modes mode() const noexcept;

        layout& valign(valigns value) noexcept;
        [[nodiscard]] valigns valign() const noexcept;

        layout& halign(haligns value) noexcept;
        [[nodiscard]] haligns halign() const noexcept;

        layout& spacing(f32 value) noexcept;
        [[nodiscard]] f32 spacing() const noexcept;
    private:
        modes mode_ = modes::vertical;
        valigns valign_ = valigns::center;
        haligns halign_ = haligns::center;
        f32 spacing_ = 0.f;
    };

    class layout_item final {
    public:
        layout_item() = default;
    };
}

ENUM_HPP_REGISTER_TRAITS(e2d::layout::modes)
ENUM_HPP_REGISTER_TRAITS(e2d::layout::haligns)
ENUM_HPP_REGISTER_TRAITS(e2d::layout::valigns)

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
    inline layout& layout::mode(modes value) noexcept {
        mode_ = value;
        return *this;
    }

    inline layout::modes layout::mode() const noexcept {
        return mode_;
    }

    inline layout& layout::valign(valigns value) noexcept {
        valign_ = value;
        return *this;
    }

    inline layout::valigns layout::valign() const noexcept {
        return valign_;
    }

    inline layout& layout::halign(haligns value) noexcept {
        halign_ = value;
        return *this;
    }

    inline layout::haligns layout::halign() const noexcept {
        return halign_;
    }

    inline layout& layout::spacing(f32 value) noexcept {
        spacing_ = value;
        return *this;
    }

    inline f32 layout::spacing() const noexcept {
        return spacing_;
    }
}
