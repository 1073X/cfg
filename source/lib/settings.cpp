
#include "cfg/settings.hpp"

#include <com/fatal_error.hpp>

#include "source.hpp"

namespace miu::cfg {

settings::settings(source const* src)
    : _src(src) {}

std::string_view
settings::name() const {
    assert(_src);
    return _src->name();
}

com::variant
settings::get(std::string_view name) const {
    assert(_src);
    auto var = _src->get(name);
    if (com::type_id<void>::value == var.id()) {
        FATAL_ERROR<std::out_of_range>("missing required setting [", name, "]");
    }
    return var;
}

template<>
settings
settings::required<settings>(std::string_view name) const {
    assert(_src);
    auto child = _src->get_child(name);
    if (!child) {
        FATAL_ERROR<std::out_of_range>("missing child setting [", name, "]");
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
