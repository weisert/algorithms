// Author: Igor Vayzert <igor.weisert@gmail.com>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "strings/kmp.h"

using testing::ElementsAre;

namespace strings{

TEST(KMP, ComputePrefixMatcher) {
  std::string pattern("ababaca");
  auto prefix_matcher = KMP::ComputePrefixMatcher(pattern);
  ASSERT_EQ(pattern.size(), prefix_matcher.size());
  ASSERT_THAT(prefix_matcher, ElementsAre(-1, -1, 0, 1, 2, -1, 0));

  pattern = "aba";
  prefix_matcher = KMP::ComputePrefixMatcher(pattern);
  ASSERT_EQ(pattern.size(), prefix_matcher.size());
  ASSERT_THAT(prefix_matcher, ElementsAre(-1, -1, 0));
}

TEST(KMP, Matcher) {
  std::string pattern("aba");
  auto prefix_matcher = KMP::ComputePrefixMatcher(pattern);
  KMP kmp(pattern, prefix_matcher);
  ASSERT_EQ(0, kmp.matches());
  kmp.Add('a');
  ASSERT_EQ(0, kmp.matches());
  kmp.Add('b');
  ASSERT_EQ(0, kmp.matches());
  kmp.Add('a');
  ASSERT_EQ(1, kmp.matches());
  kmp.Add('b');
  ASSERT_EQ(1, kmp.matches());
  kmp.Add('a');
  ASSERT_EQ(2, kmp.matches());
  kmp.Add('a');
  ASSERT_EQ(2, kmp.matches());
  kmp.Add('a');
  ASSERT_EQ(2, kmp.matches());
  kmp.Add('b');
  ASSERT_EQ(2, kmp.matches());
  kmp.Add('a');
  ASSERT_EQ(3, kmp.matches());
}

}  // namespace strings
