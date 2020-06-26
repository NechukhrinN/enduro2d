/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "platform.hpp"

#if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_MACOSX

namespace
{
    using namespace e2d;

    class platform_internal_state_impl_macosx final : public platform_internal_state_impl {
    public:
        platform_internal_state_impl_macosx() = default;
        ~platform_internal_state_impl_macosx() noexcept final = default;
    };
}

namespace e2d
{
    platform_internal_state_impl_uptr platform_internal_state_impl::create() {
        return std::make_unique<platform_internal_state_impl_macosx>();
    }
}

#endif
