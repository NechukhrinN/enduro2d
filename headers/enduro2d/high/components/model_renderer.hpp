/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

#include "../assets/model_asset.hpp"

namespace e2d
{
    class model_renderer final {
    public:
        model_renderer() = default;
        model_renderer(const model_asset::ptr& model);

        model_renderer& model(const model_asset::ptr& value) noexcept;
        [[nodiscard]] const model_asset::ptr& model() const noexcept;
    private:
        model_asset::ptr model_;
    };
}

namespace e2d
{
    template <>
    class factory_loader<model_renderer> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            model_renderer& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<model_renderer> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<model_renderer>& c) const;
    };
}

namespace e2d
{
    inline model_renderer::model_renderer(const model_asset::ptr& model)
    : model_(model) {}

    inline model_renderer& model_renderer::model(const model_asset::ptr& value) noexcept {
        model_ = value;
        return *this;
    }

    inline const model_asset::ptr& model_renderer::model() const noexcept {
        return model_;
    }
}
