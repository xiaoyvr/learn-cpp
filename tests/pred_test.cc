#include <gtest/gtest.h>
#include "enums.h"
#include "testinghelper.h"

using namespace testing;
using namespace testing::naming;
using namespace pred::vec;

namespace TestVecBasic {
  using param_t = std::tuple<std::vector<int>, pred_t<int>, pred_t<int>>;
  class WhenCheckVecOperations : public TestWithParam<param_t> {};
  INSTANTIATE_TEST_SUITE_P(Vec, WhenCheckVecOperations, Values(
    param_t{{0, 1, 3}, contains(1), contains(2)},
    param_t{{0, 1, 3}, !contains(2), !contains(1)},
    param_t{{0, 1, 3}, includes_all(1, 3), includes_all<int>({1, 4})},
    param_t{{0, 1, 3}, !includes_all(1, 2), !includes_all<int>({1, 3})},
    param_t{{0, 1, 3}, excludes_all(2,4,6), excludes_all<int>({1, 4, 6})},
    param_t{{0, 1, 3}, !excludes_all(3,4), !excludes_all<int>({2, 4, 6})}),
    ([](const auto& info){ 
      auto & [v, pred, npred] = info.param;
      return pred + "_and_not_" + npred + "_given_" + v;
      })
  );
  TEST_P(WhenCheckVecOperations, should_) {
    auto & [v, pred, npred] = GetParam();
    ASSERT_TRUE(pred(v)) << std::string(pred) << " failed";
    ASSERT_FALSE(npred(v)) << std::string(npred) << " failed";
  }
}

namespace TestEnum {
  using param_t = std::tuple<std::vector<Color>, pred_t<Color>, pred_t<Color>>;
  class WhenCheckEnumContains : public TestWithParam<param_t> {};
  INSTANTIATE_TEST_SUITE_P(Vec, WhenCheckEnumContains, Values(
    param_t{{Color::Blue,Color::Red}, contains(Color::Red), contains(Color::Yellow)},
    param_t{{Color::Blue,Color::Red}, !contains(Color::Yellow), !contains(Color::Red)}), 
    ([](const auto& info){ auto & [v, c, nc] = info.param;
      return c + "_and_not_" + nc + "_given_" + v;
  }));
  TEST_P(WhenCheckEnumContains, should_) {
    auto & [tobeTested, pred, npred] = GetParam();
    ASSERT_TRUE(pred(tobeTested)) << std::string(pred) << " failed";
    ASSERT_FALSE(npred(tobeTested)) << std::string(npred) << " failed";
  }
}

namespace TestUnderlyingContains {
  using param_t = std::tuple<std::vector<int>, pred_u_t<Color>, pred_u_t<Color>>;
  class WhenCheckUnderlyingContains : public TestWithParam<param_t> {};
  INSTANTIATE_TEST_SUITE_P(Vec, WhenCheckUnderlyingContains, Values(
    param_t{{0, 1}, contains_u(Color::Red), contains_u(Color::Yellow)},
    param_t{{0, 1}, !contains_u(Color::Yellow), !contains_u(Color::Red)}), 
    ([](const auto& info){ auto & [v, c, nc] = info.param;
      return c + "_and_not_" + nc + "_given_" + v;
  }));
  TEST_P(WhenCheckUnderlyingContains, should_) {
    auto & [tobeTested, pred, npred] = GetParam();
    ASSERT_TRUE(pred(tobeTested)) << std::string(pred) << " failed";
    ASSERT_FALSE(npred(tobeTested)) << std::string(npred) << " failed";
  }
}

namespace TestUnnamedEnum {
  using param_t = std::tuple<std::vector<Direction>, pred_t<Direction>>;
  class WhenCheckUnnamed : public TestWithParam<std::tuple<std::vector<Direction>, pred_t<Direction>>> {};
  INSTANTIATE_TEST_SUITE_P(Vec, WhenCheckUnnamed, Values(
    param_t{{Direction::Up, Direction::Bottom, Direction::Left}, contains(Direction::Up)},
    param_t{{Direction::Up, Direction::Bottom, Direction::Left}, !contains(Direction::Right)}), 
    ([](const auto& info){ auto & [v, c] = info.param;
      return c + "_given_" + v;
  }));
  TEST_P(WhenCheckUnnamed, should_) {
    auto & [v, pred] = GetParam();
    ASSERT_TRUE(pred(v)) << std::string(pred) << " failed";
  }

  using paramu_t = std::tuple<std::vector<int>, pred_u_t<Direction>>;
  class WhenCheckUnnamedUnderlying : public TestWithParam<paramu_t> {};
  INSTANTIATE_TEST_SUITE_P(Vec, WhenCheckUnnamedUnderlying, Values(
    paramu_t{{0, 2, 3}, contains_u(Direction::Up)},
    paramu_t{{0, 2, 3}, !contains_u(Direction::Right)}), 
    ([](const auto& info){ auto & [v, c] = info.param;
      return c + "_given_" + v;
  }));
  TEST_P(WhenCheckUnnamedUnderlying, should_) {
    auto & [tobeTested, pred] = GetParam();
    ASSERT_TRUE(pred(tobeTested)) << std::string(pred) << " failed";
  }
}
