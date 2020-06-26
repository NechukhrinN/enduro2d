/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/node.hpp>

namespace e2d
{
    node::node(gobject owner)
    : owner_(std::move(owner)) {}

    node::~node() noexcept {
        E2D_ASSERT(!parent_);
        remove_all_children();
    }

    node_iptr node::create() {
        return node_iptr(new node());
    }

    node_iptr node::create(const t2f& transform) {
        node_iptr child = create();
        child->transform(transform);
        return child;
    }

    node_iptr node::create(const node_iptr& parent) {
        node_iptr child = create();
        if ( parent ) {
            parent->add_child(child);
        }
        return child;
    }

    node_iptr node::create(const node_iptr& parent, const t2f& transform) {
        node_iptr child = create(parent);
        child->transform(transform);
        return child;
    }

    node_iptr node::create(gobject owner) {
        return node_iptr(new node(std::move(owner)));
    }

    node_iptr node::create(gobject owner, const t2f& transform) {
        node_iptr child = create(std::move(owner));
        child->transform(transform);
        return child;
    }

    node_iptr node::create(gobject owner, const node_iptr& parent) {
        node_iptr child = create(std::move(owner));
        if ( parent ) {
            parent->add_child(child);
        }
        return child;
    }

    node_iptr node::create(gobject owner, const node_iptr& parent, const t2f& transform) {
        node_iptr child = create(std::move(owner), parent);
        child->transform(transform);
        return child;
    }

    void node::owner(gobject owner) noexcept {
        owner_ = std::move(owner);
    }

    gobject node::owner() const noexcept {
        return owner_;
    }

    void node::transform(const t2f& transform) noexcept {
        transform_ = transform;
        mark_dirty_local_matrix_();
    }

    const t2f& node::transform() const noexcept {
        return transform_;
    }

    void node::translation(const v2f& translation) noexcept {
        transform_.translation = translation;
        mark_dirty_local_matrix_();
    }

    const v2f& node::translation() const noexcept {
        return transform_.translation;
    }

    void node::rotation(f32 rotation) noexcept {
        transform_.rotation = rotation;
        mark_dirty_local_matrix_();
    }

    f32 node::rotation() const noexcept {
        return transform_.rotation;
    }

    void node::scale(const v2f& scale) noexcept {
        transform_.scale = scale;
        mark_dirty_local_matrix_();
    }

    const v2f& node::scale() const noexcept {
        return transform_.scale;
    }

    const m4f& node::local_matrix() const noexcept {
        if ( math::check_and_clear_any_flags(flags_, fm_dirty_local_matrix) ) {
            update_local_matrix_();
        }
        return local_matrix_;
    }

    const m4f& node::world_matrix() const noexcept {
        if ( math::check_and_clear_any_flags(flags_, fm_dirty_world_matrix) ) {
            update_world_matrix_();
        }
        return world_matrix_;
    }

    v4f node::local_to_world(const v4f& local) const noexcept {
        return local * world_matrix();
    }

    v4f node::world_to_local(const v4f& world) const noexcept {
        return world * math::inversed(world_matrix()).first;
    }

    node_iptr node::root() noexcept {
        node* n = this;
        while ( n->parent_ ) {
            n = n->parent_;
        }
        return node_iptr(n);
    }

    const_node_iptr node::root() const noexcept {
        const node* n = this;
        while ( n->parent_ ) {
            n = n->parent_;
        }
        return const_node_iptr(n);
    }

    node_iptr node::parent() noexcept {
        return node_iptr(parent_);
    }

    const_node_iptr node::parent() const noexcept {
        return const_node_iptr(parent_);
    }

    bool node::has_parent() const noexcept {
        return !!parent_;
    }

    bool node::has_parent_recursive(
        const const_node_iptr& parent) const noexcept
    {
        const node* p = parent_;
        while ( p && p != parent ) {
            p = p->parent_;
        }
        return !!p;
    }

    bool node::has_children() const noexcept {
        return !children_.empty();
    }

    bool node::has_child_recursive(
        const const_node_iptr& child) const noexcept
    {
        return child
            ? child->has_parent_recursive(this)
            : false;
    }

    bool node::remove_from_parent() noexcept {
        return parent_
            ? parent_->remove_child(this)
            : false;
    }

    std::size_t node::remove_all_children() noexcept {
        std::size_t count{0u};
        while ( !children_.empty() ) {
            ++count;
            children_.back().remove_from_parent();
        }
        return count;
    }

    std::size_t node::child_count() const noexcept {
        return children_.size();
    }

    std::size_t node::child_count_recursive() const noexcept {
        std::size_t count{0u};
        for ( const node& child : children_ ) {
            count += 1u + child.child_count_recursive();
        }
        return count;
    }

    bool node::add_child(
        const node_iptr& child) noexcept
    {
        return add_child_to_front(child);
    }

    bool node::add_child_at(
        const node_iptr& child,
        std::size_t index) noexcept
    {
        return index == 0u
            ? add_child_to_back(child)
            : add_child_after(child_at(index - 1u), child);
    }

    bool node::add_child_to_back(
        const node_iptr& child) noexcept
    {
        if ( !child || child == this ) {
            return false;
        }

        if ( child == first_child() ) {
            return true;
        }

        intrusive_ptr_add_ref(child.get());
        child->remove_from_parent();
        children_.push_front(*child);
        child->parent_ = this;
        child->mark_dirty_world_matrix_();
        return true;
    }

    bool node::add_child_to_front(
        const node_iptr& child) noexcept
    {
        if ( !child || child == this ) {
            return false;
        }

        if ( child == last_child() ) {
            return true;
        }

        intrusive_ptr_add_ref(child.get());
        child->remove_from_parent();
        children_.push_back(*child);
        child->parent_ = this;
        child->mark_dirty_world_matrix_();
        return true;
    }

    bool node::add_child_before(
        const node_iptr& before,
        const node_iptr& child) noexcept
    {
        if ( !before || !child || before->parent_ != this ) {
            return false;
        }

        if ( before == child || before->prev_sibling() == child ) {
            return true;
        }

        intrusive_ptr_add_ref(child.get());
        child->remove_from_parent();
        children_.insert(
            node_children::iterator_to(*before),
            *child);
        child->parent_ = this;
        child->mark_dirty_world_matrix_();
        return true;
    }

    bool node::add_child_after(
        const node_iptr& after,
        const node_iptr& child) noexcept
    {
        if ( !after || !child || after->parent_ != this ) {
            return false;
        }

        if ( after == child || after->next_sibling() == child ) {
            return true;
        }

        intrusive_ptr_add_ref(child.get());
        child->remove_from_parent();
        children_.insert(
            ++node_children::iterator_to(*after),
            *child);
        child->parent_ = this;
        child->mark_dirty_world_matrix_();
        return true;
    }

    bool node::add_sibling_before(const node_iptr& sibling) noexcept {
        return parent_
            ? parent_->add_child_before(this, sibling)
            : false;
    }

    bool node::add_sibling_after(const node_iptr& sibling) noexcept {
        return parent_
            ? parent_->add_child_after(this, sibling)
            : false;
    }

    bool node::remove_child(const node_iptr& child) noexcept {
        if ( !child || child->parent_ != this ) {
            return false;
        }
        children_.erase_and_dispose(
            node_children::iterator_to(*child),
            [](node* n){
                n->parent_ = nullptr;
                n->mark_dirty_world_matrix_();
                intrusive_ptr_release(n);
            });
        return true;
    }

    node_iptr node::remove_child_at(std::size_t index) noexcept {
        node_iptr child = child_at(index);
        return remove_child(child)
            ? child
            : node_iptr();
    }

    bool node::swap_children(
        const node_iptr& child_l,
        const node_iptr& child_r) noexcept
    {
        if ( !child_l || !child_r ) {
            return false;
        }

        if ( child_l->parent_ != this || child_r->parent_ != this ) {
            return false;
        }

        node_children::iterator_swap(
            node_children::iterator_to(*child_l),
            node_children::iterator_to(*child_r));

        return true;
    }

    bool node::swap_children_at(
        std::size_t child_l,
        std::size_t child_r) noexcept
    {
        return swap_children(
            child_at(child_l),
            child_at(child_r));
    }

    bool node::send_backward() noexcept {
        node_iptr prev = prev_sibling();
        return prev
            ? prev->add_sibling_before(this)
            : false;
    }

    bool node::bring_to_back() noexcept {
        return parent_
            ? parent_->add_child_to_back(this)
            : false;
    }

    bool node::send_forward() noexcept {
        node_iptr next = next_sibling();
        return next
            ? next->add_sibling_after(this)
            : false;
    }

    bool node::bring_to_front() noexcept {
        return parent_
            ? parent_->add_child_to_front(this)
            : false;
    }

    node_iptr node::first_child() noexcept {
        return !children_.empty()
            ? node_iptr(&children_.front())
            : nullptr;
    }

    const_node_iptr node::first_child() const noexcept {
        return !children_.empty()
            ? const_node_iptr(&children_.front())
            : nullptr;
    }

    node_iptr node::last_child() noexcept {
        return !children_.empty()
            ? node_iptr(&children_.back())
            : nullptr;
    }

    const_node_iptr node::last_child() const noexcept {
        return !children_.empty()
            ? const_node_iptr(&children_.back())
            : nullptr;
    }

    node_iptr node::prev_sibling() noexcept {
        if ( !parent_ ) {
            return nullptr;
        }
        auto iter = node_children::iterator_to(*this);
        if ( iter == parent_->children_.begin() ) {
            return nullptr;
        }
        --iter;
        return node_iptr(&*iter);
    }

    const_node_iptr node::prev_sibling() const noexcept {
        if ( !parent_ ) {
            return nullptr;
        }
        auto iter = node_children::iterator_to(*this);
        if ( iter == parent_->children_.begin() ) {
            return nullptr;
        }
        --iter;
        return const_node_iptr(&*iter);
    }

    node_iptr node::next_sibling() noexcept {
        if ( !parent_ ) {
            return nullptr;
        }
        auto iter = node_children::iterator_to(*this);
        if ( ++iter == parent_->children_.end() ) {
            return nullptr;
        }
        return node_iptr(&*iter);
    }

    const_node_iptr node::next_sibling() const noexcept {
        if ( !parent_ ) {
            return nullptr;
        }
        auto iter = node_children::iterator_to(*this);
        if ( ++iter == parent_->children_.end() ) {
            return nullptr;
        }
        return const_node_iptr(&*iter);
    }

    node_iptr node::child_at(std::size_t index) noexcept {
        node_iptr child = first_child();
        for ( std::size_t i = 0; i < index && child; ++i ) {
            child = child->next_sibling();
        }
        return child;
    }

    const_node_iptr node::child_at(std::size_t index) const noexcept {
        const_node_iptr child = first_child();
        for ( std::size_t i = 0; i < index && child; ++i ) {
            child = child->next_sibling();
        }
        return child;
    }

    std::pair<std::size_t, bool> node::child_index(
        const const_node_iptr& child) const noexcept
    {
        if ( !child || child->parent_ != this ) {
            return {std::size_t(-1), false};
        }

        const auto distance = std::distance(
            children_.begin(),
            node_children::iterator_to(*child));

        return {math::numeric_cast<std::size_t>(distance), true};
    }
}

namespace e2d
{
    void node::mark_dirty_local_matrix_() noexcept {
        if ( math::check_and_set_any_flags(flags_, fm_dirty_local_matrix) ) {
            mark_dirty_world_matrix_();
        }
    }

    void node::mark_dirty_world_matrix_() noexcept {
        if ( math::check_and_set_any_flags(flags_, fm_dirty_world_matrix) ) {
            for ( node& child : children_ ) {
                child.mark_dirty_world_matrix_();
            }
        }
    }

    void node::update_local_matrix_() const noexcept {
        local_matrix_ = math::make_trs_matrix4(transform_);
    }

    void node::update_world_matrix_() const noexcept {
        world_matrix_ = parent_
            ? local_matrix() * parent_->world_matrix()
            : local_matrix();
    }
}

namespace e2d::nodes
{
    options& options::reversed(bool value) noexcept {
        if ( value != reversed() ) {
            math::flip_flags_inplace(flags_, fm_reversed);
        }
        return *this;
    }

    options& options::recursive(bool value) noexcept {
        if ( value != recursive() ) {
            math::flip_flags_inplace(flags_, fm_recursive);
        }
        return *this;
    }

    options& options::include_root(bool value) noexcept {
        if ( value != include_root() ) {
            math::flip_flags_inplace(flags_, fm_include_root);
        }
        return *this;
    }

    bool options::reversed() const noexcept {
        return math::check_any_flags(flags_, fm_reversed);
    }

    bool options::recursive() const noexcept {
        return math::check_any_flags(flags_, fm_recursive);
    }

    bool options::include_root() const noexcept {
        return math::check_any_flags(flags_, fm_include_root);
    }
}
