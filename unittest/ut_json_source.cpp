#include <gtest/gtest.h>

#include "source/lib/json_source.hpp"

using miu::cfg::json_source;

TEST(ut_json_source, name) {
    json_source src { "name", nlohmann::json {} };
    EXPECT_EQ("name", src.name());
}

TEST(ut_json_source, get_by_name) {
    nlohmann::json json;
    json["item"] = 1;

    json_source src { "", json };
    EXPECT_EQ(1, src.get("item").get<int32_t>());

    EXPECT_FALSE(src.get("not_exists").get<int32_t>().has_value());
}

TEST(ut_json_source, get_by_idx) {
    nlohmann::json json;
    json[0] = 1;

    json_source src { "", json };
    EXPECT_EQ(1, src.get(0).get<int32_t>());

    EXPECT_FALSE(src.get(1).get<int32_t>().has_value());
}

TEST(ut_json_source, get_child_by_name) {
    nlohmann::json json;
    json["child2"]["item"] = 1;
    json["child"]["item"] = 1;

    json_source src { "", json };
    auto child = src.get_child("child");
    EXPECT_NE(nullptr, child);

    // return the same child
    EXPECT_EQ(child, src.get_child("child"));

    EXPECT_EQ(nullptr, src.get_child("not_exists"));
}
