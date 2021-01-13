#pragma once

#include <com/variant.hpp>
#include <nlohmann/json.hpp>

namespace miu::cfg {

com::variant json2var(nlohmann::json, std::vector<std::string>*);

}
