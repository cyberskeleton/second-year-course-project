#ifndef C_SET_INTERVAL_H
#define C_SET_INTERVAL_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "interval.h"

static const int SET_ARRAY_INITIAL_SIZE = 8;
static const bool DEBUG = false;

struct SetInterval {
    struct Interval** in_array;
    int counter;
};

struct SetInterval* init();

void free_set(struct SetInterval* set);

void add(struct SetInterval* set, enum Interval_type left, enum Interval_type right, double a, double b);

void delete_by_index(struct SetInterval* set, int index);

int find_index_by_value(struct SetInterval* set, struct Interval* i);

struct Interval* get_by_index(struct SetInterval* set, int index);

int compare_start(const void* a_str, const void* b_str);

int compare_end(const void* a_ptr, const void* b_ptr);

int compare_start_to_end(const void* a_ptr, const void* b_ptr);

bool is_joint(struct Interval* first_str, struct Interval* next_str);

void sort_intervals(struct SetInterval* set);

struct Interval* get_intersection(struct SetInterval* set);

struct SetInterval* get_union(struct SetInterval* set);

struct SetInterval* get_difference(struct Interval* interval_A, struct Interval *interval_B);

struct Interval* get_copy(struct Interval* interval);

bool equals(struct Interval* given, struct Interval* that);

void print_interval(struct Interval* i);

void print_set(struct SetInterval* set);

#ifdef __cplusplus
}
#endif

#endif