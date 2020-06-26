/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

#include "../assets/atlas_asset.hpp"
#include "../assets/sprite_asset.hpp"
#include "../assets/material_asset.hpp"

namespace e2d
{
    class sprite_renderer final {
    public:
        ENUM_HPP_CLASS_DECL(modes, u8,
            (simple)
            (sliced))

        ENUM_HPP_CLASS_DECL(blendings, u8,
            (normal)
            (additive)
            (multiply)
            (screen))
    public:
        sprite_renderer() = default;
        sprite_renderer(const sprite_asset::ptr& sprite);

        sprite_renderer& tint(const color32& value) noexcept;
        [[nodiscard]] const color32& tint() const noexcept;

        sprite_renderer& scale(const v2f& value) noexcept;
        [[nodiscard]] const v2f& scale() const noexcept;

        sprite_renderer& mode(modes value) noexcept;
        [[nodiscard]] modes mode() const noexcept;

        sprite_renderer& blending(blendings value) noexcept;
        [[nodiscard]] blendings blending() const noexcept;

        sprite_renderer& filtering(bool value) noexcept;
        [[nodiscard]] bool filtering() const noexcept;

        sprite_renderer& sprite(const sprite_asset::ptr& value) noexcept;
        [[nodiscard]] const sprite_asset::ptr& sprite() const noexcept;

        sprite_renderer& materials(flat_map<str_hash, material_asset::ptr> value) noexcept;
        [[nodiscard]] const flat_map<str_hash, material_asset::ptr>& materials() const noexcept;
        [[nodiscard]] material_asset::ptr find_material(str_hash name) const noexcept;
    private:
        color32 tint_ = color32::white();
        v2f scale_ = v2f::unit();
        modes mode_ = modes::simple;
        blendings blending_ = blendings::normal;
        bool filtering_ = true;
        sprite_asset::ptr sprite_;
        flat_map<str_hash, material_asset::ptr> materials_;
    };

    ENUM_HPP_REGISTER_TRAITS(sprite_renderer::modes)
    ENUM_HPP_REGISTER_TRAITS(sprite_renderer::blendings)
}

namespace e2d
{
    template <>
    class factory_loader<sprite_renderer> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            sprite_renderer& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<sprite_renderer> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<sprite_renderer>& c) const;
        void operator()(gcomponent<sprite_renderer>& c, gizmos_context& ctx) const;
    };
}

namespace e2d
{
    inline sprite_renderer::sprite_renderer(const sprite_asset::ptr& sprite)
    : sprite_(sprite) {}

    inline sprite_renderer& sprite_renderer::tint(const color32& value) noexcept {
        tint_ = value;
        return *this;
    }

    inline const color32& sprite_renderer::tint() const noexcept {
        return tint_;
    }

    inline sprite_renderer& sprite_renderer::scale(const v2f& value) noexcept {
        scale_ = value;
        return *this;
    }

    inline const v2f& sprite_renderer::scale() const noexcept {
        return scale_;
    }

    inline sprite_renderer& sprite_renderer::mode(modes value) noexcept {
        mode_ = value;
        return *this;
    }

    inline sprite_renderer::modes sprite_renderer::mode() const noexcept {
        return mode_;
    }

    inline sprite_renderer& sprite_renderer::blending(blendings value) noexcept {
        blending_ = value;
        return *this;
    }

    inline sprite_renderer::blendings sprite_renderer::blending() const noexcept {
        return blending_;
    }

    inline sprite_renderer& sprite_renderer::filtering(bool value) noexcept {
        filtering_ = value;
        return *this;
    }

    inline bool sprite_renderer::filtering() const noexcept {
        return filtering_;
    }

    inline sprite_renderer& sprite_renderer::sprite(const sprite_asset::ptr& value) noexcept {
        sprite_ = value;
        return *this;
    }

    inline const sprite_asset::ptr& sprite_renderer::sprite() const noexcept {
        return sprite_;
    }

    inline sprite_renderer& sprite_renderer::materials(flat_map<str_hash, material_asset::ptr> value) noexcept {
        materials_ = std::move(value);
        return *this;
    }

    inline const flat_map<str_hash, material_asset::ptr>& sprite_renderer::materials() const noexcept {
        return materials_;
    }

    inline material_asset::ptr sprite_renderer::find_material(str_hash name) const noexcept {
        const auto iter = materials_.find(name);
        return iter != materials_.end()
            ? iter->second
            : nullptr;
    }
}
