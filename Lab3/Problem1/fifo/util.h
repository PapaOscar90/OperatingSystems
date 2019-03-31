#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))

// SafeRealloc
void *checkedRealloc(void *old, size_t size)
{
  void *p = realloc(old, size);
  if (p == NULL)
  {
    fprintf(stderr, "Realloc failed, out of memory?");
    exit(EXIT_FAILURE);
  }
  return p;
}

// SafeMalloc
void *safeMalloc(size_t size)
{
  void *p = malloc(size);
  if (p == NULL)
  {
    fprintf(stderr, "Malloc failed, out of memory?");
    exit(EXIT_FAILURE);
  }
  return p;
}

// The Vec is a container for data
typedef struct Vec
{
  int *data;
  size_t len;
  size_t capacity;
} Vec;

// Creates a new vec, and creates an initial size
Vec newVec()
{
  Vec vec;
  vec.capacity = 50;
  vec.data = safeMalloc(vec.capacity * sizeof(int));
  vec.len = 0;

  return vec;
}

// Will double the size of the array if needed
void resizeVec(Vec *vec)
{
  vec->capacity *= 2;
  vec->data = checkedRealloc(vec->data, vec->capacity * sizeof(int));
}

// Add an int to the end of vec
void push(Vec *vec, int e)
{
  if (vec->len == vec->capacity)
  {
    resizeVec(vec);
  }

  vec->data[vec->len] = e;
  vec->len++;
}

int peek(Vec *vec)
{
  return vec->data[0];
}

// Pop off the first item, and shift everything forward
int pop(Vec *vec)
{
  int val = vec->data[0];

  // Move things forward in the queue
  for (size_t i = 0; i < vec->len - 1; ++i)
  {
    vec->data[i] = vec->data[i + 1];
  }

  vec->len--;
  return val;
}

// Free the vecs data
void freeVec(Vec *vec)
{
  free(vec->data);
}

// Print a vec
void printVec(Vec *vec)
{
  printf("[ ");
  for (size_t i = 0; i < vec->len; ++i)
  {
    printf("%d ", vec->data[i]);
  }
  printf("]\n");
}

#endif // UTIL_H