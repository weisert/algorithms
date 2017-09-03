// Author: Igor Vayzert <igor.weisert@gmail.com>

#include "strings/aho_corasick.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace strings {
namespace {
struct MockCallback {
  MOCK_METHOD2(Call, void(size_t, size_t));
};
}  // namespace

using ::testing::WhenSorted;
using ::testing::ElementsAre;

class AhoCorasickPatternMatcherTest : public ::testing::Test {
 public:
  using Matcher = AhoCorasickPatternMatcher<char, 'a', 'z'>;
  using Node = Matcher::Node;
  const Node* GetRoot(const Matcher& matcher) {
    return matcher.root_.get();
  }

  static bool HasPath(const Node* root, const std::string& path) {
    for (const auto c : path) {
      if (root->child_nodes_[c - Matcher::kMinChar] == nullptr)
        return false;
      root = root->child_nodes_[c - Matcher::kMinChar];
    }
    return true;
  }
};

TEST_F(AhoCorasickPatternMatcherTest, TreeWalk) {
  std::vector<std::string> patterns{"abc", "abd", "zzu"};
  Matcher matcher(std::move(patterns));
  const auto* root = GetRoot(matcher);
  ASSERT_TRUE(HasPath(root, "abc"));
  ASSERT_TRUE(HasPath(root, "abd"));
  ASSERT_TRUE(HasPath(root, "zzu"));
  ASSERT_FALSE(HasPath(root, "aba"));
  ASSERT_FALSE(HasPath(root, "abb"));
  ASSERT_FALSE(HasPath(root, "zza"));
}

TEST_F(AhoCorasickPatternMatcherTest, MatchIndexBasic) {
  std::vector<std::string> patterns{"a", "ab", "aac"};
  Matcher matcher(std::move(patterns));
  auto result1 = matcher.Match("abcdef");
  ASSERT_THAT(result1, WhenSorted(ElementsAre(0, 1)));
  auto result2 = matcher.Match("aac");
  ASSERT_THAT(result2, WhenSorted(ElementsAre(0, 0, 2)));
}

TEST_F(AhoCorasickPatternMatcherTest, MatchIndex) {
  std::vector<std::string> patterns1{"a", "ab", "bc", "aab", "aac", "bd"};
  Matcher matcher(std::move(patterns1));
  auto result = matcher.Match("bcaab");
  ASSERT_THAT(result, WhenSorted(ElementsAre(0, 0, 1, 2, 3)));
}

TEST_F(AhoCorasickPatternMatcherTest, MatchWithPredicateBasic) {
  std::vector<std::string> patterns{"a", "ab", "aac"};
  Matcher matcher(std::move(patterns));
  MockCallback callback;
  auto callback_proxy = [&callback](size_t index, size_t position) {
    callback.Call(index, position);
  };
  EXPECT_CALL(callback, Call(0u, 0u)).Times(1);
  EXPECT_CALL(callback, Call(1u, 0u)).Times(1);
  matcher.Match("abcdef", callback_proxy);
  EXPECT_TRUE(::testing::Mock::VerifyAndClearExpectations(&callback));
  EXPECT_CALL(callback, Call(0u, 0u)).Times(1);
  EXPECT_CALL(callback, Call(0u, 1u)).Times(1);
  EXPECT_CALL(callback, Call(2u, 0u)).Times(1);
  matcher.Match("aac", callback_proxy);
  EXPECT_TRUE(::testing::Mock::VerifyAndClearExpectations(&callback));
}

TEST_F(AhoCorasickPatternMatcherTest, MatchWithPredicate) {
  std::vector<std::string> patterns1{"a", "ab", "bc", "aab", "aac", "bd"};
  Matcher matcher(std::move(patterns1));
  MockCallback callback;
  auto callback_proxy = [&callback](size_t index, size_t position) {
    callback.Call(index, position);
  };
  EXPECT_CALL(callback, Call(0u, 2u)).Times(1);
  EXPECT_CALL(callback, Call(0u, 3u)).Times(1);
  EXPECT_CALL(callback, Call(1u, 3u)).Times(1);
  EXPECT_CALL(callback, Call(2u, 0u)).Times(1);
  EXPECT_CALL(callback, Call(3u, 2u)).Times(1);
  matcher.Match("bcaab", callback_proxy);
  EXPECT_TRUE(::testing::Mock::VerifyAndClearExpectations(&callback));
}

TEST_F(AhoCorasickPatternMatcherTest, MatchPositionsBasic) {
  std::vector<std::string> patterns{"a", "ab", "aac"};
  Matcher matcher(std::move(patterns));
  auto result1 = matcher.MatchPositions("abcdef");
  auto comparator = [](const std::pair<size_t, size_t> &lhs,
                       const std::pair<size_t, size_t> &rhs) -> bool {
    if (lhs.first == rhs.first)
      return lhs.second < rhs.second;
    return lhs.first < rhs.first;
  };
  std::sort(result1.begin(), result1.end(), comparator);
  ASSERT_THAT(result1, ElementsAre(std::make_pair(0u, 0u),
                                   std::make_pair(1u, 0u)));
  auto result2 = matcher.MatchPositions("aac");
  std::sort(result2.begin(), result2.end(), comparator);
  ASSERT_THAT(result2, ElementsAre(std::make_pair(0u, 0u),
                                   std::make_pair(0u, 1u),
                                   std::make_pair(2u, 0u)));
}

TEST_F(AhoCorasickPatternMatcherTest, MatchPositions) {
  std::vector<std::string> patterns1{"a", "ab", "bc", "aab", "aac", "bd"};
  Matcher matcher(std::move(patterns1));
  auto result = matcher.MatchPositions("bcaab");
  auto comparator = [](const std::pair<size_t, size_t> &lhs,
                       const std::pair<size_t, size_t> &rhs) -> bool {
    if (lhs.first == rhs.first)
      return lhs.second < rhs.second;
    return lhs.first < rhs.first;
  };
  std::sort(result.begin(), result.end(), comparator);
  ASSERT_THAT(result, ElementsAre(std::make_pair(0u, 2u),
                                  std::make_pair(0u, 3u),
                                  std::make_pair(1u, 3u),
                                  std::make_pair(2u, 0u),
                                  std::make_pair(3u, 2u)));
}

}  // namespace strings
