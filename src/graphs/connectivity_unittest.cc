// Author: Igor Vayzert <igor.weisert@gmail.com>

#include "graphs/connectivity.h"

#include "gtest/gtest.h"

namespace graphs {

TEST(Connectivity, InitiallyDisconnected) {
  Connectivity conn(10);
  for (int i = 0; i < conn.Size(); ++i) {
    for (int j = 0; j < conn.Size(); ++j) {
      if (i == j) {
        ASSERT_TRUE(conn.Connected(i, j));
      } else {
        ASSERT_FALSE(conn.Connected(i, j));
      }
    }
  }
}

TEST(Connectivity, Union) {
  Connectivity conn(10);
  ASSERT_FALSE(conn.Connected(2, 9));
  conn.Union(2, 9);
  ASSERT_TRUE(conn.Connected(2, 9));
  ASSERT_FALSE(conn.Connected(9, 0));
  conn.Union(9, 0);
  ASSERT_TRUE(conn.Connected(0, 9));
  ASSERT_TRUE(conn.Connected(9, 0));
  ASSERT_FALSE(conn.Connected(1, 9));
  conn.Union(0, 1);
  ASSERT_TRUE(conn.Connected(0, 1));
  ASSERT_TRUE(conn.Connected(1, 2));
  ASSERT_TRUE(conn.Connected(2, 9));
  ASSERT_TRUE(conn.Connected(0, 9));
  ASSERT_TRUE(conn.Connected(1, 9));
  ASSERT_TRUE(conn.Connected(2, 0));

  Connectivity conn2(100);
  for (int i = 1; i < conn2.Size(); ++i) {
    ASSERT_FALSE(conn2.Connected(i - 1, i));
    conn2.Union(i - 1, i);
    ASSERT_TRUE(conn2.Connected(i - 1, i));
  }
  for (int i = 0; i < conn2.Size() - 1; ++i) {
    ASSERT_TRUE(conn2.Connected(i, conn2.Size() - 1));
  }
}

}  // namespace graphs
