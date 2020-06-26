/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "inspector_widget.hpp"

#include <enduro2d/high/editor.hpp>
#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/inspector.hpp>

namespace e2d::dbgui_widgets
{
    inspector_widget::inspector_widget() {
        desc_.first_size = v2f(300.f, 400.f);
    }

    bool inspector_widget::show() {
        if ( !modules::is_initialized<editor, inspector>() ) {
            return false;
        }

        if ( gobject go = the<editor>().selection() ) {
            the<inspector>().show_for(go);
        }

        return true;
    }

    const inspector_widget::description& inspector_widget::desc() const noexcept {
        return desc_;
    }
}
