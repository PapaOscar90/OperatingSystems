#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50
#define MAX_REFERENCES 100

/// Should output the number of page faults to stdout.

int main() {
  size_t number_of_page_faults = 0;

  size_t amount_of_physical_frames;
  scanf("%lu", &amount_of_physical_frames);

  size_t page_reference;
  while (scanf("%lu", &page_reference) == 1) {
    printf("%lu\n", page_reference);
  }

  printf("Number of page faults: &lu\n", number_of_page_faults);
  return EXIT_SUCCESS;
}
