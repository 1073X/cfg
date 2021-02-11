
#include "cfg/cmd_source.hpp"

#include <meta/info.hpp>

namespace miu::cfg {

cmd_source::cmd_source(int32_t argc, const char* argv[]) {
    meta::set_category("tool");
    meta::set_type(argv[0]);
    meta::set_name(argv[0]);

    _args.emplace("_name_", argv[0]);

    auto pos = 0;
    auto i   = 1;
    while (i < argc) {
        auto key = argv[i++];

        if (key[0] != '-' && key[1] != '-') {
            _args.emplace(com::to_string(pos++), key);
        } else {
            std::vector<com::variant> vals;
            while (i < argc && argv[i][0] != '-' && argv[i][1] != '-') {
                vals.emplace_back(argv[i++]);
            }

            if (vals.empty()) {    // switch
                _args.emplace(key + 2, com::variant { true });
            } else if (vals.size() == 1) {    // key/val
                _args.emplace(key + 2, vals[0]);
            } else {    // array
                auto child = cmd_source { key, vals };
                _children.emplace(key + 2, child);
            }
        }
    }
}

cmd_source::cmd_source(std::string_view name, std::vector<com::variant> const& vals) {
    _args.emplace("_name_", name);
    for (auto i = 0U; i < vals.size(); i++) {
        auto key = com::to_string(i);
        _args.emplace(key, vals[i]);
    }
}

std::string_view cmd_source::name() const {
    return get("_name_").get<const char*>().value();
}

com::variant cmd_source::get(uint32_t idx) const {
    return get(com::to_string(idx));
}

source const* cmd_source::get_child(uint32_t) const {
    return nullptr;
}

com::variant cmd_source::get(std::string_view key) const {
    auto it = _args.find(key.data());
    if (_args.end() == it) {
        return com::variant();
    }
    return it->second;
}

source const* cmd_source::get_child(std::string_view key) const {
    auto it = _children.find(key.data());
    if (_children.end() == it) {
        return nullptr;
    }
    return &it->second;
}

}    // namespace miu::cfg
