#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <vector>

#include <gflags/gflags.h>
#include <glog/logging.h>

#define TIME_IT(hi, lo)                                                        \
  asm("mfence\n\t"                                                             \
      "rdtsc\n\t"                                                              \
      : "=d"(hi), "=a"(lo))

DEFINE_string(output, "", "Filename where to store output of this run.");
DEFINE_int32(mem_size, 16, "Amount of memory to allocate for benchmarking.");

double __attribute__((optimize("O0")))
fn(int *a, uint64_t repeat, uint64_t size) {
  uint32_t start_time_hi = 0, start_time_lo = 0, end_time_hi = 0,
           end_time_lo = 0;
  uint64_t start = 0, end = 0;

  TIME_IT(start_time_hi, start_time_lo);
  for (uint64_t i = 0; i < repeat; i++) {
    for (uint64_t j = 0; j < size; j++) {
      a[j] = a[j] + 1;
    }
  }
  TIME_IT(end_time_hi, end_time_lo);
  start = ((uint64_t)start_time_hi << 32) | start_time_lo;
  end = ((uint64_t)end_time_hi << 32) | end_time_lo;

  auto cycles_per_op = ((end - start) / ((double)repeat * size));
  LOG(INFO) << "Cycles / op = " << cycles_per_op;
  return cycles_per_op;
}

void store_results_to_file(
    const std::string filename,
    const std::vector<std::pair<uint, double>> &results) noexcept {
  std::ofstream file(filename);
  std::for_each(results.cbegin(), results.cend(),
                [&](auto e) { file << e.first << "\t" << e.second << "\n"; });
}

int main(int argc, char **argv) {
  std::vector<std::pair<uint, uint64_t>> buckets;
  std::vector<std::pair<uint, double>> results;

  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_output != "") {
    LOG(INFO) << "Result of this benchmark will be stored in " << FLAGS_output
              << " file.";
  }

  int upper_bound = FLAGS_mem_size;
  for (int i = 2; i < upper_bound; i++) {
    buckets.push_back({i, std::pow((double)2, i)});
  }

  size_t max_size = buckets.back().second;
  auto array_size = max_size * sizeof(int);
  LOG(INFO) << "Allocating array of size: " << array_size << " bytes.";
  int *a = (int *)malloc(array_size);

  uint64_t run_number_of_times = ((uint64_t)buckets.back().second) * 3;
  LOG(INFO) << "Will run loop " << run_number_of_times << " times.";
  std::for_each(buckets.cbegin(), buckets.cend(), [&](auto el) {
    LOG(INFO) << "Measuring bucket with 2^" << el.first << "(" << el.second
              << ") elements.";
    uint64_t repeat = run_number_of_times / el.second;
    LOG(INFO) << "Repeating measurement " << repeat << " times.";
    results.push_back({el.first, fn(a, repeat, el.second)});
  });

  free(a);

  if (FLAGS_output != "") {
    LOG(INFO) << "Storing results into " << FLAGS_output;
    store_results_to_file(FLAGS_output, results);
  }

  return 0;
}
