#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50
#define MAX_REFERENCES 100

/// Returns -1 if not in queue, else it returns the index.
/// Worst case: not in the queue so O(n).
int queue_find(size_t *q, size_t size, size_t n) {
  for (size_t i = 0; i < size; ++i)
    if (q[i] == n)
      return i;

  return -1;
}

/// Removes from the front of the queue (the back of the array by shifting
/// everything) and inserts at the back of the queue.
void fifo_insert(size_t *q, size_t size, size_t n) {
  // 0 used as a sentinel value.
  assert(n != 0);
  if (q[size - 1] != 0)
    fprintf(stderr, "\t\tDropping page %lu\n", q[size - 1]);
  for (int i = size - 1; i > 0; i--) {
    q[i] = q[i - 1];
  }
  fprintf(stderr, "\t\tInserting page %lu\n", n);
  q[0] = n;
}

size_t page_loop(size_t amount_of_physical_frames) {
  size_t *frames = calloc(amount_of_physical_frames, sizeof(*frames));
  size_t number_of_page_faults = 0;

  // Read the page references in a loop until you cannot.
  size_t page_reference;
  while (scanf("%lu", &page_reference) == 1) {
    fprintf(stderr, "[Accessing page %lu]\n", page_reference);

    // If the page is not in the queue.
    if (queue_find(frames, amount_of_physical_frames, page_reference) == -1) {
      fprintf(stderr, "\t[Page fault]\n");
      // We have a page fault.
      number_of_page_faults++;
      // Insert the page into the queue dropping the oldest page.
      fifo_insert(frames, amount_of_physical_frames, page_reference);
    }
    // Else there was a hit and the reference is fine.
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
