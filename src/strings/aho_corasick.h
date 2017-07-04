// Author: Igor Vayzert <igor.weisert@gmail.com>

#ifndef STRINGS_AHO_CORASICK_H_
#define STRINGS_AHO_CORASICK_H_

#include <cassert>
#include <iterator>
#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace strings {
// Good videos to explain how it works
// AhoCorasickBuildTree
// https://www.youtube.com/watch?v=ePafMI_rSJg

// AhoCorasickBuildFail
// https://www.youtube.com/watch?v=qPyhPXPl3T4

// AhoCorasickMatching
// https://www.youtube.com/watch?v=IcXimoT_YXA

template<typename Char, Char kMin, Char kMax>
class AhoCorasickPatternMatcher {
 public:
  using CharType = Char;
  const static CharType kMinChar = kMin;
  const static CharType kMaxChar = kMax;

  explicit AhoCorasickPatternMatcher(
      std::vector<std::basic_string<Char>> patterns);
  AhoCorasickPatternMatcher(const AhoCorasickPatternMatcher&) = delete;
  AhoCorasickPatternMatcher& operator=(
      const AhoCorasickPatternMatcher&) = delete;
  AhoCorasickPatternMatcher(AhoCorasickPatternMatcher&&) = default;
  AhoCorasickPatternMatcher& operator=(AhoCorasickPatternMatcher&&) = default;

  const std::vector<std::basic_string<Char>>& patterns() const {
    return patterns_;
  }

  std::vector<size_t> Match(const std::basic_string<Char>&);
  template<typename Function>
  void Match(const std::basic_string<Char>&, Function);
  std::vector<std::pair<size_t, size_t>> MatchPositions(
      const std::basic_string<Char>&);

 private:
  friend class AhoCorasickPatternMatcherTest;

  struct Node {
    static size_t GetIndex(Char c) {
        assert(kMin <= c && c <= kMax);
        return static_cast<size_t>(c) - kMin;
    }

    explicit Node(char c) : fail_node_(nullptr), c_(c) {
        std::fill(std::begin(child_nodes_), std::end(child_nodes_), nullptr);
    }

    // default constructor for root node
    Node() : Node(static_cast<Char>(0)) { fail_node_ = this; }

    ~Node() {
        for (auto* ptr : child_nodes_)
            delete ptr;
    }

    Node(const Node&) = delete;
    const Node& operator=(const Node&) = delete;

    Node(Node&&) = default;
    Node& operator=(Node&&) = default;

    bool HasChild(char c) const { return child_nodes_[GetIndex(c)] != nullptr; }

    Node* fail_node_;
    char c_;
    std::vector<size_t> matched_patterns_;
    Node* child_nodes_[kMax - kMin + 1];
  };

  void BuidTree();
  void BuidFails();

  std::vector<std::basic_string<Char>> patterns_;
  std::unique_ptr<Node> root_;
};

template<typename Char, Char kMin, Char kMax>
AhoCorasickPatternMatcher<Char,kMin, kMax>::AhoCorasickPatternMatcher(
    std::vector<std::basic_string<Char>> patterns)
    : patterns_(std::move(patterns)) {
  root_.reset(new Node());
  BuidTree();
  BuidFails();
}

template<typename Char, Char kMin, Char kMax>
void AhoCorasickPatternMatcher<Char,kMin, kMax>::BuidTree() {
  for (size_t i = 0u; i < patterns_.size(); ++i) {
    const auto& pattern = patterns_[i];
    Node* current = root_.get();
    for (const Char& c : pattern) {
      size_t ch_index = Node::GetIndex(c);
      if (current->child_nodes_[ch_index] == nullptr) {
        current->child_nodes_[ch_index] = new Node(c);
      }
      current = current->child_nodes_[ch_index];
    }
    current->matched_patterns_.push_back(i);
  }
}

template<typename Char, Char kMin, Char kMax>
void AhoCorasickPatternMatcher<Char,kMin, kMax>::BuidFails() {
  std::queue<Node*> queue;
  for (Node* root_child : root_->child_nodes_) {
    if (root_child) {
      root_child->fail_node_ = root_.get();
      queue.push(root_child);
    }
  }
  while (!queue.empty()) {
    Node* n = queue.front();
    queue.pop();
    for (Node* child : n->child_nodes_) {
      if (!child)
        continue;
      Node* fail_node = n->fail_node_;
      while (fail_node != fail_node->fail_node_ &&
             !fail_node->HasChild(child->c_)) {
        fail_node = fail_node->fail_node_;
      }
      if(fail_node->HasChild(child->c_)) {
        Node* fn = fail_node->child_nodes_[Node::GetIndex(child->c_)];
        child->fail_node_ = fn;
      } else {
        assert(fail_node == root_.get());
        child->fail_node_ = fail_node;
      }
      queue.push(child);
    }
  }
}

template<typename Char, Char kMin, Char kMax>
std::vector<size_t> AhoCorasickPatternMatcher<Char,kMin, kMax>::Match(
    const std::basic_string<Char>& input) {
  std::vector<size_t> result;
  Node* current = root_.get();
  for (const auto& c : input) {
    while (current != current->fail_node_ && !current->HasChild(c))
      current = current->fail_node_;
    if (!current->HasChild(c)) {
      assert(current == root_.get());
      continue;
    }
    current = current->child_nodes_[Node::GetIndex(c)];
    Node *temp = current;
    while (temp != root_.get()) {
      std::copy(temp->matched_patterns_.begin(),
                temp->matched_patterns_.end(),
                std::back_inserter(result));
      temp = temp->fail_node_;
    }
  }
  return result;
}

template<typename Char, Char kMin, Char kMax>
template<typename Function>
void AhoCorasickPatternMatcher<Char,kMin, kMax>::Match(
    const std::basic_string<Char>& input, Function func) {
  Node* current = root_.get();
  for (size_t i = 0; i < input.size(); ++i) {
    const auto& c = input[i];
    while (current != current->fail_node_ && !current->HasChild(c))
      current = current->fail_node_;
    if (!current->HasChild(c)) {
      assert(current == root_.get());
      continue;
    }
    current = current->child_nodes_[Node::GetIndex(c)];
    Node *temp = current;
    while (temp != root_.get()) {
      for (const auto &mpi : temp->matched_patterns_)
        func(mpi, i - patterns_[mpi].size() + 1);
      temp = temp->fail_node_;
    }
  }
}

template<typename Char, Char kMin, Char kMax>
std::vector<std::pair<size_t, size_t>>
AhoCorasickPatternMatcher<Char,kMin, kMax>::MatchPositions(
    const std::basic_string<Char>& input) {
  std::vector<std::pair<size_t, size_t>> result;
  Node* current = root_.get();
  for (size_t i = 0; i < input.size(); ++i) {
    const auto& c = input[i];
    while (current != current->fail_node_ && !current->HasChild(c))
      current = current->fail_node_;
    if (!current->HasChild(c)) {
      assert(current == root_.get());
      continue;
    }
    current = current->child_nodes_[Node::GetIndex(c)];
    Node *temp = current;
    while (temp != root_.get()) {
      for (const auto &mpi : temp->matched_patterns_)
        result.emplace_back(mpi, i - patterns_[mpi].size() + 1);
      temp = temp->fail_node_;
    }
  }
  return result;
}

}  // namespace strings

#endif  // STRINGS_AHO_CORASICK_H_
