#pragma once

#include <com/strcat.hpp>
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
        if (!_src.is_array() || idx >= _src.size()) {
            return {};
        }

        return json2var(_src.at(idx));
    }

    source const* get_child(uint32_t idx) const override {
        if (!_src.is_array() || idx >= _src.size()) {
            return nullptr;
        }

        auto name = com::strcat { _name, idx }.str();
        return fetch_child(name, _src.at(idx));
    }

    com::variant get(std::string_view name) const override {
        if (!_src.is_object() || !_src.contains(name.data())) {
            return {};
        }

        return json2var(_src.at(name.data()));
    }

    source const* get_child(std::string_view name) const override {
        if (!_src.is_object() || !_src.contains(name.data())) {
            return nullptr;
        }

        return fetch_child(name, _src.at(name.data()));
    }

  private:
    source const* fetch_child(std::string_view name, nlohmann::json const& json) const {
        for (auto const& child : _children) {
            if (name == child.name()) {
                return &child;
            }
        }

        _children.emplace_back(name, json);
        return &_children.back();
    }

  private:
    std::string _name;
    nlohmann::json _src;

    mutable std::list<json_source> _children;
};

}    // namespace miu::cfg
