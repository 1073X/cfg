#pragma once

#include <com/variant.hpp>

namespace miu::cfg {

class source {
  public:
    virtual ~source() = default;
    virtual com::variant get(std::string_view) const = 0;
};

}    // namespace miu::cfg
