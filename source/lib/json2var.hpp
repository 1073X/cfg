#pragma once

#include <com/variant.hpp>
#include <com/json.hpp>

namespace miu::cfg {

com::variant json2var(com::json, std::vector<std::string>*);

}
