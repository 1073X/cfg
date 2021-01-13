#include <gtest/gtest.h>

#include "source/lib/json2var.hpp"

using miu::cfg::json2var;
using miu::com::type_id;

TEST(ut_json2var, boolean) {
    nlohmann::json json { { "item", false } };
    auto var = json2var(json["item"], nullptr);
    EXPECT_EQ(type_id<bool>::value, var.id());
    EXPECT_FALSE(var.get<bool>().value());
}

TEST(ut_json2var, number) {
    nlohmann::json json { { "item", 1 } };
    auto var = json2var(json["item"], nullptr);
    EXPECT_EQ(type_id<int64_t>::value, var.id());
    EXPECT_EQ(1, var.get<int64_t>().value());
}

TEST(ut_json2var, string) {
    std::vector<std::string> strings;

    nlohmann::json json { { "item", "0123456789abcdef" } };
    auto var = json2var(json["item"], &strings);
    EXPECT_EQ(type_id<const char*>::value, var.id());
    EXPECT_EQ("0123456789abcdef", var.get<std::string>().value());
}
