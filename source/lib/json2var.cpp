
#include "json2var.hpp"

namespace miu::cfg {

com::variant
json2var(com::json json, std::vector<std::string>* strings) {
    com::variant var;
    if (json.is_boolean()) {
        var = json.get<bool>();
    } else if (json.is_number()) {
        var = json.get<int64_t>();
    } else if (json.is_string()) {
        strings->push_back(json.get<std::string>());
        var = strings->back().c_str();
    }
    return var;
}

}    // namespace miu::cfg
