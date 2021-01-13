#include <gtest/gtest.h>

#include "source/lib/json_source.hpp"

using miu::cfg::json_source;

TEST(ut_json_source, name) {
    json_source src { "name", nlohmann::json {} };
    EXPECT_EQ("name", src.name());
}
