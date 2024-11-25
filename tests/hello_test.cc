#include <gtest/gtest.h>
// #include "naminghelper.h"
#include "testinghelper.h"
#include "enums.h"

using namespace testing;
using namespace testing::naming;

class ContainsScenario : public TestWithParam<std::tuple<pred::vec::pred_t<int>>> {};
INSTANTIATE_TEST_SUITE_P(Vec, ContainsScenario, Values(
  ContainsScenario::ParamType{pred::vec::contains(1)},
  ContainsScenario::ParamType{pred::vec::not_contains(2)}), 
  [](const auto& info){ auto & [pred] = info.param;
    return pred + "_when_check";
});
TEST_P(ContainsScenario, should_) {
  auto & [pred] = GetParam();
  auto tobeTested = std::vector<int> {0,1,3};
  ASSERT_TRUE(pred(tobeTested)) << std::string(pred) << " failed";
}

class ContainsEnumScenario : public TestWithParam<std::tuple<pred::vec::pred_t<Color>>> {};
INSTANTIATE_TEST_SUITE_P(Vec, ContainsEnumScenario, Values(
  ContainsEnumScenario::ParamType{pred::vec::contains(Color::Red)},
  ContainsEnumScenario::ParamType{pred::vec::not_contains(Color::Yellow)}), 
  [](const auto& info){ auto & [c] = info.param;
    return c + "_when_check";
});
TEST_P(ContainsEnumScenario, should_) {
  auto & [pred] = GetParam();
  auto tobeTested = std::vector<Color> {Color::Blue,Color::Red};
  ASSERT_TRUE(pred(tobeTested)) << std::string(pred) << " failed";
}

class ContainsNamedEnumScenario : public TestWithParam<std::tuple<pred::vec::pred_t<ColorN>>> {};
INSTANTIATE_TEST_SUITE_P(Vec, ContainsNamedEnumScenario, Values(
  ContainsNamedEnumScenario::ParamType{pred::vec::contains<ColorN>(Color::Red)},
  ContainsNamedEnumScenario::ParamType{pred::vec::not_contains<ColorN>(Color::Yellow)}), 
  [](const auto& info){ auto & [c] = info.param;
    return c + "_when_check";
});
TEST_P(ContainsNamedEnumScenario, should_) {
  auto & [pred] = GetParam();
  auto tobeTested = std::vector<Color> {Color::Blue,Color::Red};
  ASSERT_TRUE(pred(tobeTested)) << std::string(pred) << " failed";
}

class ContainsUnderlyingScenario : public TestWithParam<std::tuple<pred::vec::pred_u_t<Color>>> {};
INSTANTIATE_TEST_SUITE_P(Vec, ContainsUnderlyingScenario, Values(
  ContainsUnderlyingScenario::ParamType{pred::vec::contains_u(Color::Red)},
  ContainsUnderlyingScenario::ParamType{pred::vec::not_contains_u(Color::Yellow)}), 
  [](const auto& info){ auto & [c] = info.param;
    return c + "_when_check";
});
TEST_P(ContainsUnderlyingScenario, should_) {
  auto & [pred] = GetParam();
  auto tobeTested = std::vector<int> {0, 1};
  ASSERT_TRUE(pred(tobeTested)) << std::string(pred) << " failed";
}

class ContainsUnderlyingNamedScenario : public TestWithParam<std::tuple<pred::vec::pred_u_t<ColorN>>> {};
INSTANTIATE_TEST_SUITE_P(Vec, ContainsUnderlyingNamedScenario, Values(
  ContainsUnderlyingNamedScenario::ParamType{pred::vec::contains_u(_n(Color::Red))},
  ContainsUnderlyingNamedScenario::ParamType{pred::vec::not_contains_u(_n(Color::Yellow))}), 
  [](const auto& info){ auto & [c] = info.param;
    return c + "_when_check";
});
TEST_P(ContainsUnderlyingNamedScenario, should_) {
  auto & [pred] = GetParam();
  auto tobeTested = std::vector<int> {0, 1};
  ASSERT_TRUE(pred(tobeTested)) << std::string(pred) << " failed";
}

class ContainsUnderlyingUnamedScenario : public TestWithParam<std::tuple<pred::vec::pred_u_t<Direction>>> {};
INSTANTIATE_TEST_SUITE_P(Vec, ContainsUnderlyingUnamedScenario, Values(
  ContainsUnderlyingUnamedScenario::ParamType{pred::vec::contains_u(Direction::Up)},
  ContainsUnderlyingUnamedScenario::ParamType{pred::vec::not_contains_u(Direction::Right)}), 
  [](const auto& info){ auto & [c] = info.param;
    return c + "_when_check";
});
TEST_P(ContainsUnderlyingUnamedScenario, should_) {
  auto & [pred] = GetParam();
  auto tobeTested = std::vector<int> {0, 2, 3};
  ASSERT_TRUE(pred(tobeTested)) << std::string(pred) << " failed";
}


class IncludesUnderlyingNamedScenario : public TestWithParam<std::tuple<pred::vec::pred_u_t<ColorN>>> {};
INSTANTIATE_TEST_SUITE_P(Vec, IncludesUnderlyingNamedScenario, Values(
  IncludesUnderlyingNamedScenario::ParamType{pred::vec::includes_all_u<ColorN>({Color::Red})},
  IncludesUnderlyingNamedScenario::ParamType{pred::vec::excludes_all_u<ColorN>({Color::Yellow})}), 
  [](const auto& info){ auto & [c] = info.param;
    return c + "_when_check";
});
TEST_P(IncludesUnderlyingNamedScenario, should_) {
  auto & [pred] = GetParam();
  auto tobeTested = std::vector<int> {0, 1};
  ASSERT_TRUE(pred(tobeTested)) << std::string(pred) << " failed";
}
