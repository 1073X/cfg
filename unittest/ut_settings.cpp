#include <gmock/gmock.h>

#include "source/lib/settings.hpp"

using miu::cfg::source;
using miu::com::variant;
using testing::Return;

struct ut_settings : public testing::Test {
    struct mock : public source {
        MOCK_METHOD(std::string_view, name, (), (const override));
        MOCK_METHOD(variant, get, (std::string_view), (const override));
        MOCK_METHOD(source const*, get_child, (std::string_view), (const override));
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

TEST_F(ut_settings, optional) {
    EXPECT_CALL(m, get(testing::StrEq("item")))
        .WillOnce(Return(variant(123)))
        .WillOnce(Return(variant()))
        .WillOnce(Return(variant()))
        .WillOnce(Return(variant(1.2)));

    EXPECT_EQ(123, settings.optional<int32_t>("item", 321));
    EXPECT_EQ(321, settings.optional<int32_t>("item", 321));
    EXPECT_EQ(0, settings.optional<int32_t>("item"));
    EXPECT_ANY_THROW(settings.optional<int32_t>("item", 321));
}

TEST_F(ut_settings, required_child) {
    mock child_src;
    EXPECT_CALL(child_src, name()).WillRepeatedly(Return("child_name"));
    EXPECT_CALL(m, get_child(testing::_)).WillOnce(Return(&child_src)).WillOnce(Return(nullptr));

    auto child = settings.required<miu::cfg::settings>("child");
    EXPECT_EQ("child_name", child.name());

    EXPECT_ANY_THROW(settings.required<miu::cfg::settings>("child"));
}

TEST_F(ut_settings, optional_child) {
    mock child_src;
    EXPECT_CALL(child_src, name()).WillRepeatedly(Return("child_name"));
    EXPECT_CALL(m, get_child(testing::_)).WillOnce(Return(&child_src)).WillOnce(Return(nullptr));

    auto child = settings.optional<miu::cfg::settings>("child", { nullptr });
    EXPECT_EQ("child_name", child.name());

    EXPECT_NO_THROW(child = settings.optional<miu::cfg::settings>("child"));
    EXPECT_FALSE(child);
}