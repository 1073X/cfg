
#include "cfg/cmd_source.hpp"

#include <filesystem>
#include <meta/info.hpp>

namespace miu::cfg {

cmd_source::cmd_source(int32_t argc, const char* argv[]) {
    meta::set_category("tool");

    std::string cmd_name = std::filesystem::path(argv[0]).filename();
    meta::set_type(cmd_name);
    meta::set_name(cmd_name);

    _args.emplace("_name_", cmd_name);

    auto pos = 1;

    // positional
    while (pos < argc && argv[pos][0] != '-' && argv[pos][1] != '-') {
        _args.emplace(com::to_string(pos - 1), argv[pos]);
        pos++;
    }
    _args.emplace("_size_", pos - 1);

    // named
    while (pos < argc) {
        auto arg = argv[pos++];

        std::vector<com::variant> vals;
        while (pos < argc && argv[pos][0] != '-' && argv[pos][1] != '-') {
            vals.emplace_back(argv[pos++]);
        }

        if (vals.empty()) {    // switch
            vals.emplace_back(true);
        }

        auto child = cmd_source { arg, vals };
        _children.emplace(arg + 2, child);
    }
}

cmd_source::cmd_source(std::string_view name, std::vector<com::variant> const& vals) {
    _args.emplace("_name_", name);
    _args.emplace("_size_", vals.size());
    for (auto i = 0U; i < vals.size(); i++) {
        auto key = com::to_string(i);
        _args.emplace(key, vals[i]);
    }
}

std::string cmd_source::name() const {
    auto it = _args.find("_name_");
    return it->second.get<std::string>().value();
}

uint32_t cmd_source::size() const {
    auto it = _args.find("_size_");
    return it->second.get<uint32_t>().value();
}

com::variant cmd_source::get(uint32_t idx) const {
    auto it = _args.find(std::to_string(idx));
    if (_args.end() == it) {
        return {};
    }
    return it->second;
}

source const* cmd_source::get_child(uint32_t) const {
    return nullptr;
}

com::variant cmd_source::get(std::string_view key) const {
    auto child = get_child(key);
    if (!child) {
        return com::variant();
    }
    return child->get(0);
}

source const* cmd_source::get_child(std::string_view key) const {
    auto it = _children.find(key.data());
    if (_children.end() == it) {
        return nullptr;
    }
    return &it->second;
}

}    // namespace miu::cfg
