
#include "cfg/json_source.hpp"

#include <com/strcat.hpp>
#include <list>
#include <meta/info.hpp>
#include <utility>

#include "cfg/settings.hpp"

#include "json2var.hpp"

namespace miu::cfg {

class json_source_impl {
  public:
    json_source_impl(std::string_view name, nlohmann::json src)
        : name(name)
        , src(std::move(src)) {}

    std::string name;
    nlohmann::json src;

    mutable std::vector<std::string> strings;
    mutable std::list<json_source> children;
};

json_source::json_source(std::string_view name, nlohmann::json const& src)
    : _impl(new json_source_impl { name, src }) {
    if (src.contains("com")) {
        auto com_src = json_source { "com", src["com"] };
        auto com_set = settings { &com_src };

        auto cate = com_set.optional<std::string>("category", meta::category());
        auto type = com_set.optional<std::string>("type", meta::type());
        auto name = com_set.optional<std::string>("name", meta::name());

        meta::set_category(cate);
        meta::set_type(type);
        meta::set_name(name);
    }
}

json_source::~json_source() {
    delete _impl;
}

std::string json_source::name() const {
    return _impl->name;
}

com::variant json_source::get(uint32_t idx) const {
    if (!_impl->src.is_array() || idx >= _impl->src.size()) {
        return {};
    }

    return json2var(_impl->src.at(idx), &_impl->strings);
}

source const* json_source::get_child(uint32_t idx) const {
    if (!_impl->src.is_array() || idx >= _impl->src.size()) {
        return nullptr;
    }

    auto name = com::strcat { _impl->name, idx }.str();
    return fetch_child(name, _impl->src.at(idx));
}

com::variant json_source::get(std::string_view name) const {
    if (!_impl->src.is_object() || !_impl->src.contains(name.data())) {
        return {};
    }

    return json2var(_impl->src.at(name.data()), &_impl->strings);
}

source const* json_source::get_child(std::string_view name) const {
    if (!_impl->src.is_object() || !_impl->src.contains(name.data())) {
        return nullptr;
    }

    return fetch_child(name, _impl->src.at(name.data()));
}

source const* json_source::fetch_child(std::string_view name, nlohmann::json const& json) const {
    for (auto const& child : _impl->children) {
        if (name == child.name()) {
            return &child;
        }
    }

    _impl->children.emplace_back(name, json);
    return &_impl->children.back();
}

}    // namespace miu::cfg
