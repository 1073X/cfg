#include <gmock/gmock.h>

#include "cfg/settings.hpp"
#include "cfg/source.hpp"

using miu::cfg::source;
using miu::com::variant;
using testing::Return;

struct ut_settings : public testing::Test {
    struct mock : public source {
        MOCK_METHOD(std::string, name, (), (const override));

        MOCK_METHOD(variant, get_by_name, (std::string_view), (const));
        variant get(std::string_view key) const override { return get_by_name(key); }

        MOCK_METHOD(uint32_t, size, (), (const));
        MOCK_METHOD(variant, get_by_idx, (uint32_t), (const));
        variant get(uint32_t key) const override { return get_by_idx(key); }

        MOCK_METHOD(source const*, get_child_by_idx, (uint32_t), (const override));
        source const* get_child(uint32_t key) const override { return get_child_by_idx(key); }

        MOCK_METHOD(source const*, get_child_by_name, (std::string_view), (const override));
        source const* get_child(std::string_view key) const override {
            return get_child_by_name(key);
        }
    } m;

    miu::cfg::settings settings { &m };
};

TEST_F(ut_settings, size) {
    EXPECT_CALL(m, size()).WillOnce(Return(99));
    EXPECT_EQ(99U, settings.size());
}

TEST_F(ut_settings, required_name) {
    EXPECT_CALL(m, get_by_name(testing::StrEq("item")))
        .WillOnce(Return(variant(123)))
        .WillOnce(Return(variant(256)))
        .WillOnce(Return(variant()));

    EXPECT_EQ(123, settings.required<int32_t>("item"));
    EXPECT_ANY_THROW(settings.required<int8_t>("item"));
    EXPECT_ANY_THROW(settings.required<int32_t>("item"));
}

TEST_F(ut_settings, optional_name) {
    EXPECT_CALL(m, get_by_name(testing::StrEq("item")))
        .WillOnce(Return(variant(123)))
        .WillOnce(Return(variant()))
        .WillOnce(Return(variant()))
        .WillOnce(Return(variant(1.2)));

    EXPECT_EQ(123, settings.optional<int32_t>("item", 321));
    EXPECT_EQ(321, settings.optional<int32_t>("item", 321));
    EXPECT_EQ(0, settings.optional<int32_t>("item"));
    EXPECT_ANY_THROW(settings.optional<int32_t>("item", 321));
}

TEST_F(ut_settings, required_idx) {
    EXPECT_CALL(m, get_by_idx(0))
        .WillOnce(Return(variant(123)))
        .WillOnce(Return(variant(256)))
        .WillOnce(Return(variant()));

    EXPECT_EQ(123, settings.required<int32_t>(0));
    EXPECT_ANY_THROW(settings.required<int8_t>(0));
    EXPECT_ANY_THROW(settings.required<int32_t>(0));
}

TEST_F(ut_settings, optional_idx) {
    EXPECT_CALL(m, get_by_idx(2))
        .WillOnce(Return(variant(123)))
        .WillOnce(Return(variant()))
        .WillOnce(Return(variant()))
        .WillOnce(Return(variant(1.2)));

    EXPECT_EQ(123, settings.optional<int32_t>(2, 321));
    EXPECT_EQ(321, settings.optional<int32_t>(2, 321));
    EXPECT_EQ(0, settings.optional<int32_t>(2));
    EXPECT_ANY_THROW(settings.optional<int32_t>(2, 321));
}

TEST_F(ut_settings, required_child_name) {
    mock child_src;
    EXPECT_CALL(child_src, name()).WillRepeatedly(Return("child_name"));
    EXPECT_CALL(m, get_child_by_name(testing::_))
        .WillOnce(Return(&child_src))
        .WillOnce(Return(nullptr));

    auto child = settings.required<miu::cfg::settings>("child");
    EXPECT_EQ("child_name", child.name());

    EXPECT_ANY_THROW(settings.required<miu::cfg::settings>("child"));
}

TEST_F(ut_settings, required_child_idx) {
    mock child_src;
    EXPECT_CALL(m, get_child_by_idx(1)).WillOnce(Return(&child_src)).WillOnce(Return(nullptr));

    EXPECT_TRUE(settings.required<miu::cfg::settings>(1));
    EXPECT_ANY_THROW(settings.required<miu::cfg::settings>(1));
}

TEST_F(ut_settings, optional_child_name) {
    mock child_src;
    EXPECT_CALL(child_src, name()).WillRepeatedly(Return("child_name"));
    EXPECT_CALL(m, get_child_by_name(testing::_))
        .WillOnce(Return(&child_src))
        .WillOnce(Return(nullptr));

    auto child = settings.optional<miu::cfg::settings>("child");
    EXPECT_EQ("child_name", child.name());

    EXPECT_NO_THROW(child = settings.optional<miu::cfg::settings>("child"));
    EXPECT_FALSE(child);
}

TEST_F(ut_settings, optional_child_idx) {
    mock child_src;
    EXPECT_CALL(m, get_child_by_idx(0)).WillOnce(Return(&child_src)).WillOnce(Return(nullptr));

    EXPECT_TRUE(settings.optional<miu::cfg::settings>(0));
    EXPECT_FALSE(settings.optional<miu::cfg::settings>(0));
}
