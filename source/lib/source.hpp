#pragma once

#include <com/variant.hpp>

namespace miu::cfg {

class source {
  public:
    virtual ~source() = default;
    virtual std::string_view name() const = 0;
    virtual com::variant get(std::string_view) const = 0;
    virtual source const* get_child(std::string_view) const = 0;
};

}    // namespace miu::cfg
