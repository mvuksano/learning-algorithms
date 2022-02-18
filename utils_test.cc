#include <gtest/gtest.h>

#include "utils.h"

TEST(ParseListTest, BasicListParse) {
  auto head = parseList("[1, 2, 3]");
  EXPECT_EQ(head->val, 1);
}

TEST(ParseListTest, PrintList) {
  testing::internal::CaptureStdout();
  printList(nullptr);

  auto output = testing::internal::GetCapturedStdout();
  EXPECT_STREQ("[]", output.c_str());
}
