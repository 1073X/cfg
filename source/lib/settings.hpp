#pragma once

#include <com/fatal_error.hpp>

#include "source.hpp"

namespace miu::cfg {

class settings {
  public:
    settings(source const* src = nullptr)
        : _src(src) {}

    auto name() const {
        assert(_src);
        return _src->name();
    }

    auto operator!() const { return !_src; }
    operator bool() const { return !operator!(); }

    template<typename T>
    T required(std::string_view name) const {
        assert(_src);
        auto var = _src->get(name);
        if (com::type_id<void>::value == var.id()) {
            FATAL_ERROR("missing required setting", name);
        }
        return var.get<T>().value();
    }

    template<typename T>
    T optional(std::string_view name, T const& default_val = T { 0 }) const {
        assert(_src);
        auto var = _src->get(name);
        if (com::type_id<void>::value == var.id()) {
            return default_val;
        }
        return var.get<T>().value();
    }

  private:
    source const* _src;
};

template<>
settings
settings::required<settings>(std::string_view name) const {
    assert(_src);
    auto child = _src->get_child(name);
    if (!child) {
        FATAL_ERROR("missing child setting", name);
    }
    return { child };
}

template<>
settings
settings::optional<settings>(std::string_view name, settings const& default_val) const {
    assert(_src);
    auto child = _src->get_child(name);
    if (!child) {
        return default_val;
    }
    return { child };
}

}    // namespace miu::cfg
