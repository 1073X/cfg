
#include "json2var.hpp"

namespace miu::cfg {

com::variant
json2var(nlohmann::json json) {
    com::variant var;
    if (json.is_boolean()) {
        var = json.get<bool>();
    } else if (json.is_number()) {
        var = json.get<int64_t>();
    } else if (json.is_string()) {
        var = json.get<std::string>();
    }
    return var;
}

}    // namespace miu::cfg
