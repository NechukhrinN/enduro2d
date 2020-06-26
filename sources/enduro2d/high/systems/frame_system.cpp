/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/frame_system.hpp>

#include <enduro2d/high/components/camera.hpp>
#include <enduro2d/high/components/disabled.hpp>

namespace
{
    using namespace e2d;

    void clear_framebuffer(render& render, window& window) {
        render.execute(render::command_block<3>()
            .add_command(render::target_command())
            .add_command(render::viewport_command(
                window.framebuffer_size().cast_to<i32>()))
            .add_command(render::clear_command()
                .color_value(color::black())));
    }

    template < typename Event >
    void for_all_cameras(ecs::registry& owner) {
        const auto comp = [](const auto& l, const auto& r) noexcept {
            return std::get<camera>(l).depth() < std::get<camera>(r).depth();
        };

        const auto func = [&owner](
            const ecs::const_entity& e,
            const camera&)
        {
            owner.process_event(Event{e});
        };

        ecsex::for_extracted_sorted_components<camera>(
            owner,
            comp,
            func,
            !ecs::exists<disabled<camera>>());
    }
}

namespace e2d
{
    //
    // frame_system::internal_state
    //

    class frame_system::internal_state final : private noncopyable {
    public:
        internal_state(engine& e, render& r, window& w)
        : engine_(e)
        , render_(r)
        , window_(w) {}
        ~internal_state() noexcept = default;

        void process_frame_update(ecs::registry& owner) {
            const f32 dt = engine_.delta_time();
            const f32 time = engine_.time();

            {
                E2D_PROFILER_SCOPE("ecs.pre_update");
                owner.process_event(systems::pre_update_event{dt,time});
            }

            {
                E2D_PROFILER_SCOPE("ecs.update");
                owner.process_event(systems::update_event{dt,time});
            }

            {
                E2D_PROFILER_SCOPE("ecs.post_update");
                owner.process_event(systems::post_update_event{dt,time});
            }
        }

        void process_frame_render(ecs::registry& owner) {
            clear_framebuffer(render_, window_);

            {
                E2D_PROFILER_SCOPE("ecs.pre_render");
                for_all_cameras<systems::pre_render_event>(owner);
            }

            {
                E2D_PROFILER_SCOPE("ecs.render");
                for_all_cameras<systems::render_event>(owner);
            }

            {
                E2D_PROFILER_SCOPE("ecs.post_render");
                for_all_cameras<systems::post_render_event>(owner);
            }
        }
    private:
        engine& engine_;
        render& render_;
        window& window_;
    };

    //
    // frame_system
    //

    frame_system::frame_system()
    : state_(new internal_state(the<engine>(), the<render>(), the<window>())) {}
    frame_system::~frame_system() noexcept = default;

    void frame_system::process(
        ecs::registry& owner,
        const ecs::after<systems::frame_update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("frame_system.process_frame_update");
        state_->process_frame_update(owner);
    }

    void frame_system::process(
        ecs::registry& owner,
        const ecs::after<systems::frame_render_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("frame_system.process_frame_render");
        state_->process_frame_render(owner);
    }
}
