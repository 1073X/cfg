#pragma once

#include <nlohmann/json.hpp>

#include "source.hpp"

namespace miu::cfg {

class json_source_impl;

class json_source : public source {
  public:
    json_source(std::string_view name, nlohmann::json const&);
    ~json_source() override;

    std::string name() const override;

    uint32_t size() const override;
    com::variant get(uint32_t) const override;
    source const* get_child(uint32_t) const override;

    com::variant get(std::string_view) const override;
    source const* get_child(std::string_view) const override;

  private:
    source const* fetch_child(std::string_view name, nlohmann::json const& json) const;

  private:
    json_source_impl* _impl;
};

}    // namespace miu::cfg
