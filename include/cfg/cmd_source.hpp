#pragma once

#include <map>

#include "source.hpp"

namespace miu::cfg {

class cmd_source : public source {
  public:
    cmd_source(int32_t argc, const char* argv[]);
    cmd_source(std::string_view, std::vector<com::variant> const&);

    std::string name() const override;

    uint32_t size() const override;
    com::variant get(uint32_t) const override;
    source const* get_child(uint32_t) const override;

    com::variant get(std::string_view) const override;
    source const* get_child(std::string_view) const override;

  private:
    std::map<std::string, com::variant> _args;
    std::map<std::string, cmd_source> _children;
};

}    // namespace miu::cfg
