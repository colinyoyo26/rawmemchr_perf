#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RUNS 100
#define BUFFER_SIZE 1000000
#define TARGET 'A'
#define CPU_FREQ "lscpu | grep 'CPU max MHz' | awk '{print $4}'"

extern void *rawmemchr(const void *src_void, int c);
extern void *detectchar(const void *src_void, int c);

static inline double gettime() {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ts.tv_sec + ts.tv_nsec / 1e9;
}

long cpu_freq() {
  FILE *cpu = popen(CPU_FREQ, "r");
  if (!cpu)
    return -1;

  char buf[50] = {0};
  if (!fgets(buf, 20, cpu)) {
    fclose(cpu);
    return -1;
  }
  fclose(cpu);
  return atol(buf) * 1e6;
}

int main(int argc, char **argv) {
  size_t dist = atoi(argv[1]);
  char buf[BUFFER_SIZE] = {0};
  buf[dist] = TARGET;

  double slow_start = gettime();
  for (size_t i = 0; i < RUNS; i++) {
    detectchar(buf, TARGET);
  }
  double slow_stop = gettime();
  assert(*(char *)detectchar(buf, TARGET) == TARGET);

  double fast_start = gettime();
  for (size_t i = 0; i < RUNS; i++) {
    rawmemchr(buf, TARGET);
  }
  double fast_stop = gettime();
  assert(*(char *)rawmemchr(buf, TARGET) == TARGET);

  long freq = cpu_freq();
  long slow_cycles = (slow_stop - slow_start) * freq;
  long fast_cycles = (fast_stop - fast_start) * freq;
  long slow_CPE = slow_cycles / dist;
  long fast_CPE = fast_cycles / dist;
  printf("%ld %ld %ld %ld\n", slow_cycles, fast_cycles, slow_CPE, fast_CPE);

  return 0;
}
