#include <algorithm>
#include <queue>
#include <vector>
#include <functional>

double findMedianSortedArrays(std::vector<int> &nums1, std::vector<int> &nums2) {
  std::priority_queue<int, std::vector<int>, std::greater<int>> q(nums1.cbegin(), nums1.cend());
  std::for_each(nums2.cbegin(), nums2.cend(), [&](auto e) { q.emplace(e); });

  auto sz = nums1.size() + nums2.size();
  int pos = sz / 2;

  auto item = -1;
  while (pos > 0 && not q.empty()) {
    item = q.top();
    q.pop();
    pos--;
  }

  if (sz % 2 != 0) {
    // there's odd number of elements
    // get next one and return result;
    return q.top();
  } else {
    // there's even number of elements.
    // get next one and return average
    return (q.top() + item) / 2.0;
  }

  return 0;
}
