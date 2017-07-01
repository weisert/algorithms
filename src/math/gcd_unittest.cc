// Author: Igor Vayzert <igor.weisert@gmail.com>

#include "math/gcd.h"

#include <cassert>
#include <cstdint>
#include <vector>

#include "gtest/gtest.h"

namespace math {
namespace {

template<typename E>
std::vector<E> Select(const std::vector<E>& from,
                      const std::vector<bool>& flags) {
  assert(from.size() == flags.size());
  std::vector<E> result;
  auto it = flags.begin();
  for (const E& element : from) {
    if (*it)
      result.push_back(element);
    ++it;
  }
  return result;
}

}  // namespace

TEST(Gcd, Gcd) {
  std::vector<int64_t> first_prime_set{2,5,11,17,23};
  std::vector<int64_t> second_prime_set{3,7,13,19,31};

  for (size_t i = 1; i <= first_prime_set.size(); ++i) {
    std::vector<bool> flags_a(first_prime_set.size(), false);
    std::fill(flags_a.begin(), flags_a.begin() + i, true);
    do {
      int64_t a = 1;
      for (auto aa : Select(first_prime_set, flags_a))
        a *= aa;
      for (size_t j = 1; j <= second_prime_set.size(); ++j) {
        std::vector<bool> flags_b(second_prime_set.size(), false);
        std::fill(flags_b.begin(), flags_b.begin() + i, true);
        do {
          int64_t b = 1;
          for (auto bb : Select(second_prime_set, flags_b))
            b *= bb;
          ASSERT_EQ(1ll, Gcd(a, b));
          ASSERT_EQ(1ll, Gcd(-a, b));
          ASSERT_EQ(1ll, Gcd(a, -b));
          ASSERT_EQ(1ll, Gcd(-a, -b));
          for (int64_t gcd = 2; gcd < 1000; gcd += 19) {
            ASSERT_EQ(gcd, Gcd(a * gcd, b * gcd));
            ASSERT_EQ(gcd, Gcd(-a * gcd, b * gcd));
            ASSERT_EQ(gcd, Gcd(a * gcd, -b * gcd));
            ASSERT_EQ(gcd, Gcd(-a * gcd, -b * gcd));
          }
        } while(std::prev_permutation(flags_b.begin(), flags_b.end()));
      }
    } while(std::prev_permutation(flags_a.begin(), flags_a.end()));
  }
}

TEST(Gcd, ZeroDivision) {
  EXPECT_DEBUG_DEATH(Gcd(0, 2), "a != 0 && b != 0");
  EXPECT_DEBUG_DEATH(Gcd(2, 0), "a != 0 && b != 0");
  EXPECT_DEBUG_DEATH(Gcd(0, 0), "a != 0 && b != 0");
}

}  // namespace math

