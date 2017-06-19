// Author: Igor Vayzert <igor.weisert@gmail.com>

#include "strings/kmp.h"

namespace strings {

// static
std::vector<int> KMP::ComputePrefixMatcher(const std::string& pattern) {
  std::vector<int> result(pattern.size(), -1);
  for (int i = 1; i < result.size(); i++) {
    int k = result[i - 1];
    while (pattern[k + 1] != pattern[i] && k >= 0) k = result[k];
    if (k == -1 && pattern[0] != pattern[i])
      result[i] = -1;
    else
      result[i] = k + 1;
  }
  return result;
}

KMP::KMP(const std::string& pattern, std::vector<int>& prefix_function)
    : pattern_(pattern), pf_(prefix_function) {
}

void KMP::Add(char c) {
  while (q_ > -1 && pattern_[q_ + 1] != c)
    q_ = pf_[q_];
  if (pattern_[q_ + 1] == c)
    ++q_;
  if (q_ == pattern_.size() - 1) {
    ++matches_;
    q_ = pf_[q_];
  }
}

}  // namespace strings
