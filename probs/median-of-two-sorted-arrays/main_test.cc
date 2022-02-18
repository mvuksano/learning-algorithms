#include <vector>
#include <gtest/gtest.h>


double findMedianSortedArrays(std::vector<int> &nums1, std::vector<int> &nums2);
TEST(FindMedianSortedArrays, Example1) {
  std::vector a{1,2};
  std::vector b{3,4};
  EXPECT_EQ(2.5, findMedianSortedArrays(a, b));
}

TEST(FindMedianSortedArrays, Example2) {
  std::vector<int> a;
  std::vector b{3,4};
  EXPECT_EQ(3.5, findMedianSortedArrays(a, b));
}
