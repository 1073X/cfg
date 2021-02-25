
#include "cfg/settings.hpp"

#include <cassert>
#include <com/fatal_error.hpp>

namespace miu::cfg {

settings::settings(source const* src)
    : _src(src) {
}

com::variant settings::get(uint32_t i) const {
    assert(_src);
    auto var = _src->get(i);
    if (com::type_id<void>::value == var.id()) {
        FATAL_ERROR<std::out_of_range>("missing required setting [", i, "]");
    }
    return var;
}

com::variant settings::get(std::string_view name) const {
    assert(_src);
    auto var = _src->get(name);
    if (com::type_id<void>::value == var.id()) {
        FATAL_ERROR<std::out_of_range>("missing required setting [", name, "]");
    }
    return var;
}

template<>
settings settings::required<settings>(std::string_view key) const {
    assert(_src);
    auto child = _src->get_child(key);
    if (!child) {
        FATAL_ERROR<std::out_of_range>("missing child setting [", key, "]");
    }
    return { child };
}

template<>
settings settings::required<settings>(uint32_t key) const {
    assert(_src);
    auto child = _src->get_child(key);
    if (!child) {
        FATAL_ERROR<std::out_of_range>("missing child setting [", key, "]");
    }
    return { child };
}

}    // namespace miu::cfg
