#include <gtest/gtest.h>

#include <iostream>

namespace miu::cfg {
extern std::string_view version();
extern std::string_view build_info();
}    // namespace miu::cfg

TEST(ut_version, version) {
    std::cout << miu::cfg::version() << std::endl;
    std::cout << miu::cfg::build_info() << std::endl;
}
