#include <gtest/gtest.h>

#include <meta/info.hpp>
#include <time/daytime.hpp>
#include <time/stamp.hpp>

#include "cfg/json_source.hpp"

using miu::cfg::json_source;

TEST(ut_json_source, name) {
    json_source src { "name", miu::com::json {} };
    EXPECT_EQ("name", src.name());
}

TEST(ut_json_source, get_by_name) {
    using miu::time::daytime;
    using miu::time::stamp;

    miu::com::json json;
    json["item"]  = 1;
    json["time"]  = "20210113 17:28:30.001";
    json["time2"] = "17:28:30.001";

    json_source src { "", json };
    EXPECT_EQ(0U, src.size());
    EXPECT_EQ(1, src.get("item").get<int32_t>());
    EXPECT_EQ(stamp(2021, 1, 13, 17, 28, 30, 1), src.get("time").get<stamp>());
    EXPECT_EQ(daytime(17, 28, 30, 1), src.get("time2").get<daytime>());

    EXPECT_FALSE(src.get("not_exists").get<int32_t>().has_value());
}

TEST(ut_json_source, get_by_idx) {
    miu::com::json json;
    json[0] = 1;
    json[1] = 2;

    json_source src { "", json };
    EXPECT_EQ(2U, src.size());
    EXPECT_EQ(1, src.get(0).get<int32_t>());
    EXPECT_EQ(2, src.get(1).get<int32_t>());

    EXPECT_FALSE(src.get(2).get<int32_t>().has_value());
}

TEST(ut_json_source, get_child_by_name) {
    miu::com::json json;
    json["child2"]["item"] = 1;
    json["child"]["item"]  = 1;

    json_source src { "", json };
    auto child = src.get_child("child");
    EXPECT_NE(nullptr, child);

    // return the same child
    EXPECT_EQ(child, src.get_child("child"));

    EXPECT_EQ(nullptr, src.get_child("not_exists"));
}

TEST(ut_json_source, get_child_by_idx) {
    miu::com::json json;
    json[0]["item"] = 0;
    json[1]["item"] = 1;

    json_source src { "name", json };
    auto child = src.get_child(0);
    ASSERT_NE(nullptr, child);
    EXPECT_EQ("name.0", child->name());

    // return the name child
    EXPECT_EQ(child, src.get_child(0));

    EXPECT_EQ(nullptr, src.get_child(2));
}

