// Author: Igor Vayzert <igor.weisert@gmail.com>

#include <algorithm>
#include <cassert>

namespace math {

template<typename Integer>
Integer Gcd(Integer a, Integer b) {
  assert (a != 0 && b != 0);
  if (a < 0)
    a = -a;
  if (b < 0)
    b = -b;
  if (a == 1 || b == 1)
    return 1;
  if (a == b)
    return a;
  const auto mm = std::minmax(a, b);
  Integer remainder = mm.second % mm.first;
  if (remainder == Integer(0))
    return mm.first;
  return Gcd(remainder, mm.first);
}

}  // namespace math
