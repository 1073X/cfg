#pragma once

#include <cassert>
#include <com/variant.hpp>

#include "source.hpp"

namespace miu::cfg {

class settings {
  public:
    settings(source const* src = nullptr);

    std::string_view name() const;

    auto operator!() const { return !_src; }
    operator bool() const { return !operator!(); }

    template<typename T>
    T required(std::string_view key) const {
        return get(key).template get<T>().value();
    }
    template<typename T>
    T required(uint32_t key) const {
        return get(key).template get<T>().value();
    }

    template<typename T, typename K>
    T optional(K key, T const& default_val = T { 0 }) const try {
        return required<T>(key);
    } catch (std::out_of_range const&) {
        return default_val;
    }

  private:
    com::variant get(uint32_t) const;
    com::variant get(std::string_view) const;

  private:
    source const* _src;
};

template<>
settings settings::required<settings>(std::string_view) const;
template<>
settings settings::required<settings>(uint32_t) const;

}    // namespace miu::cfg