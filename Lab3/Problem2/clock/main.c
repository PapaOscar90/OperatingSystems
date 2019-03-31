#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50
#define MAX_REFERENCES 100

typedef struct Page {
  size_t page_number;
  bool used;
} Page;

/// Returns -1 if not in queue, else it returns the index.
/// Worst case: not in the queue so O(n).
int queue_find(Page *q, size_t size, size_t n) {
  for (size_t i = 0; i < size; ++i)
    if (q[i].page_number == n)
      return i;

  return -1;
}

size_t clock_insert(size_t hand, Page *q, size_t size, size_t n) {
  // 0 used as a sentinel value.
  assert(n != 0);

  while (q[hand].used) {
    for (size_t i = 0; i < size; i++) {
      printf("%lu %d\n", q[i].page_number, q[i].used);
    }
    printf("\n");
    q[hand].used = false;
    hand = (hand + 1) % size;
  }
  q[hand].page_number = n;
  q[hand].used = true;
  hand = (hand + 1) % size;

  return hand;
}

size_t page_loop(size_t amount_of_physical_frames) {
  Page *frames = calloc(amount_of_physical_frames, sizeof(*frames));
  size_t number_of_page_faults = 0;
  size_t hand = 0;

  // Read the page references in a loop until you cannot.
  size_t page_reference;
  while (scanf("%lu", &page_reference) == 1) {
    fprintf(stderr, "[Accessing page %lu]\n", page_reference);

    // If the page is not in the queue.
    int pos = queue_find(frames, amount_of_physical_frames, page_reference);
    if (pos == -1) {
      fprintf(stderr, "\t[Page fault]\n");
      // We have a page fault.
      number_of_page_faults++;
      // Insert the page into the queue using clock logic.
      hand =
          clock_insert(hand, frames, amount_of_physical_frames, page_reference);
    } else {
      // Else there was a hit and the reference is fine.
      frames[pos].used = true;
    }
  }

  free(frames);
  return number_of_page_faults;
}

int main() {
  // Read-in the amount of physical frames.
  size_t amount_of_physical_frames;
  scanf("%lu", &amount_of_physical_frames);

  size_t number_of_page_faults = page_loop(amount_of_physical_frames);

  printf("Number of page faults: %lu\n", number_of_page_faults);
  return EXIT_SUCCESS;
}
