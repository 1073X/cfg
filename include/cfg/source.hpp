#pragma once

#include <com/variant.hpp>

namespace miu::cfg {

class source {
  public:
    virtual ~source() = default;

    virtual std::string name() const = 0;

    // array
    virtual uint32_t size() const                   = 0;
    virtual com::variant get(uint32_t) const        = 0;
    virtual source const* get_child(uint32_t) const = 0;

    // object
    virtual com::variant get(std::string_view) const        = 0;
    virtual source const* get_child(std::string_view) const = 0;
};

}    // namespace miu::cfg
