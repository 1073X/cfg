#pragma once

#include <cassert>
#include <com/variant.hpp>

namespace miu::cfg {

class source;

class settings {
  public:
    settings(source const* src = nullptr);

    std::string_view name() const;

    auto operator!() const { return !_src; }
    operator bool() const { return !operator!(); }

    template<typename T>
    T required(std::string_view name) const {
        auto var = get(name);
        return var.get<T>().value();
    }

    template<typename T>
    T optional(std::string_view name, T const& default_val = T { 0 }) const try {
        return required<T>(name);
    } catch (std::out_of_range const&) {
        return default_val;
    }

  private:
    com::variant get(std::string_view) const;

  private:
    source const* _src;
};

template<>
settings settings::required<settings>(std::string_view name) const;

template<>
settings settings::optional<settings>(std::string_view name, settings const&) const;

}    // namespace miu::cfg
