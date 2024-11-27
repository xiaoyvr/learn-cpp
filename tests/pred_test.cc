#include <gtest/gtest.h>
#include "enums.h"
#include "testinghelper.h"

using namespace testing;
using namespace testing::naming;
using namespace pred::vec;

namespace TestVecBasic {
  class WhenCheckVecOperations : public TestWithParam<std::tuple<std::vector<int>, pred_t<int>, pred_t<int>>> {};
  using Val = WhenCheckVecOperations::ParamType;
  INSTANTIATE_TEST_SUITE_P(Vec, WhenCheckVecOperations, Values(
    Val{{0, 1, 3}, contains(1), contains(2)},
    Val{{0, 1, 3}, !contains(2), !contains(1)},
    Val{{0, 1, 3}, includes_all<int>({1, 3}), includes_all<int>({1, 4})},
    Val{{0, 1, 3}, !includes_all<int>({1, 2}), !includes_all<int>({1, 3})},
    Val{{0, 1, 3}, excludes_all<int>({2,4,6}), excludes_all<int>({1, 4, 6})},
    Val{{0, 1, 3}, !excludes_all<int>({3,4}), !excludes_all<int>({2, 4, 6})}),
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
  class WhenCheckEnumContains : public TestWithParam<std::tuple<std::vector<Color>, pred_t<Color>, pred_t<Color>>> {};
  using Val = WhenCheckEnumContains::ParamType;
  INSTANTIATE_TEST_SUITE_P(Vec, WhenCheckEnumContains, Values(
    Val{{Color::Blue,Color::Red}, contains(Color::Red), contains(Color::Yellow)},
    Val{{Color::Blue,Color::Red}, !contains(Color::Yellow), !contains(Color::Red)}), 
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
  class WhenCheckUnderlyingContains : public TestWithParam<std::tuple<std::vector<int>, pred_u_t<Color>, pred_u_t<Color>>> {};
  using Val = WhenCheckUnderlyingContains::ParamType;
  INSTANTIATE_TEST_SUITE_P(Vec, WhenCheckUnderlyingContains, Values(
    Val{{0, 1}, contains_u(Color::Red), contains_u(Color::Yellow)},
    Val{{0, 1}, !contains_u(Color::Yellow), !contains_u(Color::Red)}), 
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
  class WhenCheckUnnamed : public TestWithParam<std::tuple<std::vector<Direction>, pred_t<Direction>>> {};
  using Val = WhenCheckUnnamed::ParamType;
  INSTANTIATE_TEST_SUITE_P(Vec, WhenCheckUnnamed, Values(
    Val{{Direction::Up, Direction::Bottom, Direction::Left}, contains(Direction::Up)},
    Val{{Direction::Up, Direction::Bottom, Direction::Left}, !contains(Direction::Right)}), 
    ([](const auto& info){ auto & [v, c] = info.param;
      return c + "_given_" + v;
  }));
  TEST_P(WhenCheckUnnamed, should_) {
    auto & [v, pred] = GetParam();
    ASSERT_TRUE(pred(v)) << std::string(pred) << " failed";
  }

  class WhenCheckUnnamedUnderlying : public TestWithParam<std::tuple<std::vector<int>, pred_u_t<Direction>>> {};
  using ValU = WhenCheckUnnamedUnderlying::ParamType;
  INSTANTIATE_TEST_SUITE_P(Vec, WhenCheckUnnamedUnderlying, Values(
    ValU{{0, 2, 3}, contains_u(Direction::Up)},
    ValU{{0, 2, 3}, !contains_u(Direction::Right)}), 
    ([](const auto& info){ auto & [v, c] = info.param;
      return c + "_given_" + v;
  }));
  TEST_P(WhenCheckUnnamedUnderlying, should_) {
    auto & [tobeTested, pred] = GetParam();
    ASSERT_TRUE(pred(tobeTested)) << std::string(pred) << " failed";
  }
}
