#include <gmock/gmock.h>

#include "source/lib/settings.hpp"

using miu::com::variant;
using testing::Return;

struct ut_settings : public testing::Test {
    struct mock : public miu::cfg::source {
        MOCK_METHOD(variant, get, (std::string_view), (const override));
    } m;

    miu::cfg::settings settings { &m };
};

TEST_F(ut_settings, required) {
    EXPECT_CALL(m, get(testing::StrEq("item")))
        .WillOnce(Return(variant(123)))
        .WillOnce(Return(variant(256)))
        .WillOnce(Return(variant()));

    EXPECT_EQ(123, settings.required<int32_t>("item"));
    EXPECT_ANY_THROW(settings.required<int8_t>("item"));
    EXPECT_ANY_THROW(settings.required<int32_t>("item"));
}
