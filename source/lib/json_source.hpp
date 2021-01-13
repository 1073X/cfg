#pragma once

#include <list>
#include <nlohmann/json.hpp>

#include "json2var.hpp"
#include "source.hpp"

namespace miu::cfg {

class json_source : public source {
  public:
    json_source(std::string_view name, nlohmann::json const& src)
        : _name(name)
        , _src(src) {}

    std::string_view name() const override { return _name; }

    com::variant get(uint32_t idx) const {
        if (_src.is_array() && idx < _src.size()) {
            return json2var(_src.at(idx));
        }
        return {};
    }

    source const* get_child(uint32_t) const override { return nullptr; }

    com::variant get(std::string_view name) const override {
        if (_src.is_object() && _src.contains(name.data())) {
            return json2var(_src.at(name.data()));
        }
        return {};
    }

    source const* get_child(std::string_view name) const override {
        if (!_src.is_object() || !_src.contains(name.data())) {
            return nullptr;
        }

        auto it = std::find_if(_children.begin(), _children.end(), [name](auto const& child) {
            return name == child.name();
        });
        if (_children.end() != it) {
            return &(*it);
        }

        _children.emplace_back(name, _src.at(name.data()));
        return &_children.back();
    }

  private:
    std::string _name;
    nlohmann::json _src;

    mutable std::list<json_source> _children;
};

}    // namespace miu::cfg
