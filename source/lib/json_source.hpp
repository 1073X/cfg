#pragma once

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

    // get by index
    com::variant get(uint32_t idx) const {
        if (_src.is_array() && idx < _src.size()) {
            return json2var(_src.at(idx));
        }
        return {};
    }

    source const* get_child(uint32_t) const override { return nullptr; }

    com::variant get(std::string_view key) const override {
        if (_src.is_object() && _src.contains(key.data())) {
            return json2var(_src.at(key.data()));
        }
        return {};
    }

    source const* get_child(std::string_view) const override { return nullptr; }

  private:
    std::string _name;
    nlohmann::json _src;
};

}    // namespace miu::cfg
