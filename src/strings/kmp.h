// Author: Igor Vayzert <igor.weisert@gmail.com>

#ifndef STRINGS_KMP_H_
#define STRINGS_KMP_H_

#include <string>
#include <vector>

namespace strings{

class KMP {
 public:
  static std::vector<int> ComputePrefixMatcher(const std::string& pattern);

  KMP() = delete;
  KMP(const std::string& pattern, std::vector<int>& prefix_function);
  KMP(const KMP&) = delete;
  const KMP& operator=(const KMP&) = delete;
  KMP(const KMP&&) = delete;
  const KMP& operator=(const KMP&&) = delete;
  
  void Add(char c);
  int matches() const { return matches_; }

 private:
  const std::string& pattern_;
  const std::vector<int>& pf_;
  int matches_ = 0;
  int q_ = -1;
};

}  // namespace strings

#endif  // STRINGS_KMP_H_
