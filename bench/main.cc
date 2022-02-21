#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

#include <glog/logging.h>

#define TIME_IT(hi, lo)                                                        \
  asm("mfence\n\t"                                                             \
      "rdtsc\n\t"                                                              \
      : "=d"(hi), "=a"(lo))

void __attribute__((optimize("O0"))) fn(uint32_t repeat, uint64_t size) {
  uint32_t start_time_hi = 0, start_time_lo = 0, end_time_hi = 0,
           end_time_lo = 0;
  uint64_t start = 0, end = 0;

  int *a = (int *)malloc(size * sizeof(int));
  TIME_IT(start_time_hi, start_time_lo);
  for (uint32_t i = 0; i < repeat; i++) {
    for (uint64_t j = 0; j < size; j++) {
      a[j] = a[j] + 1;
    }
  }
  TIME_IT(end_time_hi, end_time_lo);
  start = ((uint64_t)start_time_hi << 32) | start_time_lo;
  end = ((uint64_t)end_time_hi << 32) | end_time_lo;

  LOG(INFO) << "Cycles / op = " << ((end - start) / ((double)repeat * size));
}

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);

  std::vector<std::pair<uint, uint64_t>> buckets;
  int upper_bound = 30;
  for (int i = 2; i < upper_bound; i++) {
    buckets.push_back({i, std::pow(2, i)});
  }

  uint64_t run_number_of_times = ((uint64_t)buckets.back().second) * 3;
  LOG(INFO) << "Will run loop " << run_number_of_times << " times.";
  std::for_each(buckets.cbegin(), buckets.cend(), [&](auto el) {
    LOG(INFO) << "Measuring bucket 2^" << el.first << "(" << el.second
              << " bytes)";
    uint32_t repeat = run_number_of_times / el.second;
    LOG(INFO) << "Repeating measurement " << repeat << " times.";
    fn(repeat, el.second);
  });

  return 0;
}
