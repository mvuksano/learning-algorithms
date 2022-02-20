#include <cstdint>
#include <iostream>

void fn(int repeat, int size) {
    uint32_t start_time_hi = 0, 
             start_time_lo = 0, 
             end_time_hi = 0,
             end_time_lo = 0;
    uint64_t start = 0,
             end = 0;
    
    int *a = (int *) malloc(size * sizeof(int));
    for(int i = 0; i < 10; i++) {       
        for(int j = 0; j < size; j++) {
            a[j] = a[j] + 2;
        }
    }
    asm ("mfence\n\t"
         "rdtsc\n\t": "=d"(start_time_hi), "=a"(start_time_lo));
    
    
    for(int i = 0; i < repeat; i++) {       
        for(int j = 0; j < size; j++) {
            a[j] = a[j] + 1;
        }
    }
    asm ("mfence\n\t"
         "rdtsc\n\t": "=d"(end_time_hi), "=a"(end_time_lo));
    start = ((uint64_t) start_time_hi << 32) | start_time_lo;
    end = ((uint64_t) end_time_hi << 32) | end_time_lo;

    printf("H: %g\n", ((end-start) / ((double) repeat * size)));
}

int main() {
    int increments[] = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048,
	                4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288 /* 512kB */, 1048576, 2097152,
			4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824};
    uint64_t run_number_of_times = 1000000000;
    for (int i = 0; i < 29; i++) {
        printf("Measuring %d\n", increments[i]);
        int repeat = run_number_of_times / increments[i];
	if (repeat == 0) {
		repeat = 1;
	}
	printf("Will repeat measurement %d times.\n", repeat);
        fn(repeat, increments[i]);
    }
    
    return 0;
}
