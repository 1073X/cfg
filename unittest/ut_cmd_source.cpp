#include <gtest/gtest.h>

#include <meta/info.hpp>

#include "cfg/cmd_source.hpp"

using miu::com::variant;

TEST(ut_cmd_source, init) {
    const char* argv[] = { "app_name" };
    miu::cfg::cmd_source source { 1, argv };
    EXPECT_EQ("app_name", source.name());
}

TEST(ut_cmd_source, metainfo) {
    const char* argv[] = { "ut_cmd_source" };
    miu::cfg::cmd_source source { 1, argv };

    EXPECT_STREQ("tool", miu::meta::category());
    EXPECT_STREQ("ut_cmd_source", miu::meta::type());
    EXPECT_STREQ("ut_cmd_source", miu::meta::name());
}

TEST(ut_cmd_source, args) {
    const char* argv[] = { "", "--arg0", "1", "--arg1", "abc" };
    miu::cfg::cmd_source source { 5, argv };
    EXPECT_EQ(1, source.get("arg0").get<int32_t>());
    EXPECT_EQ("abc", source.get("arg1").get<std::string>());

    EXPECT_EQ(variant(), source.get("not_exists"));
}

TEST(ut_cmd_source, child) {
    const char* argv[] = { "", "--arg", "1", "2", "3" };
    miu::cfg::cmd_source source { 5, argv };
    auto child = source.get_child("arg");
    ASSERT_NE(nullptr, child);
    EXPECT_EQ(1, child->get(0).get<int32_t>());
    EXPECT_EQ(2, child->get(1).get<int32_t>());
    EXPECT_EQ(3, child->get(2).get<int32_t>());

    EXPECT_EQ(variant(), source.get(3));

    EXPECT_EQ(nullptr, source.get_child("not_exists"));
}

TEST(ut_cmd_source, switch) {
    const char* argv[] = { "", "--switch0", "--switch1" };
    miu::cfg::cmd_source source { 3, argv };
    EXPECT_EQ(variant(true), source.get("switch0"));
    EXPECT_EQ(variant(true), source.get("switch1"));
}

TEST(ut_cmd_source, positional) {
    const char* argv[] = { "", "pos0", "pos1" };
    miu::cfg::cmd_source source { 3, argv };
    EXPECT_EQ("pos0", source.get(0).get<std::string>());
    EXPECT_EQ("pos1", source.get(1).get<std::string>());

    EXPECT_EQ(variant(), source.get(2));
}

