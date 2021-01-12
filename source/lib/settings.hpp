#pragma once

#include <com/fatal_error.hpp>

#include "source.hpp"

namespace miu::cfg {

class settings {
  public:
    settings(source const* src)
        : _src(src) {}

    template<typename T>
    T required(std::string_view name) const {
        auto var = _src->get(name);
        if (com::type_id<void>::value == var.id()) {
            FATAL_ERROR("missing required setting", name);
        }
        return var.get<T>().value();
    }

  private:
    source const* _src;
};

}    // namespace miu::cfg
