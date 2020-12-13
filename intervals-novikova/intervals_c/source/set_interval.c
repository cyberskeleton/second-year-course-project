#include "../headers/set_interval.h"

struct SetInterval* init() {
    struct SetInterval* set = malloc(sizeof(struct SetInterval));
    struct Interval** in_array = calloc(SET_ARRAY_INITIAL_SIZE, sizeof(struct Interval*));
    set->in_array = in_array;
    set->counter = 0;
    if (DEBUG) printf("Set array for intervals initialized with size: %d\n", SET_ARRAY_INITIAL_SIZE);

    return set;
}

void free_set(struct SetInterval* set) {
    for(int i = 0; i < set->counter; i++) {
        free(set->in_array[i]);
    }
    free(set->in_array);
    set->counter = 0;
}

void add(struct SetInterval* set, enum Interval_type left, enum Interval_type right, double a, double b) {
    int counter = set->counter;
    if (counter > 0 && counter % SET_ARRAY_INITIAL_SIZE == 0) {
        struct Interval** new_array = realloc(set->in_array, (counter + SET_ARRAY_INITIAL_SIZE) * sizeof(struct Interval*));
        set->in_array = new_array;
        if (DEBUG) printf("Set array for intervals expanded\n");
    }
    set->in_array[counter] = calloc(1, sizeof(struct Interval));
    struct Interval* i = set->in_array[counter];
    i->a = a;
    i->b = b;
    i->left = left;
    i->right = right;
    if (DEBUG) printf("Interval %s%g;%g%s added\n", i->left == open ? "(" : "[", i->a, i->b, i->right == open ? ")" : "]");
    set->counter = ++counter;
}

void delete_by_index(struct SetInterval *set, int index) {
    int counter = set->counter;
    if (counter < 1) return;
    if (DEBUG) printf("Interval %g;%g removed\n", set->in_array[index]->a, set->in_array[index]->b);
    for (int i = index; i < counter - 1; i++) set->in_array[i] = set->in_array[i + 1];
    set->in_array[counter - 1] = NULL;
    if (counter > SET_ARRAY_INITIAL_SIZE && counter % SET_ARRAY_INITIAL_SIZE == 0) {
        struct Interval** new_array = realloc(set->in_array, (counter - SET_ARRAY_INITIAL_SIZE) * sizeof(struct Interval*));
        set->in_array = new_array;
        if (DEBUG) printf("Set array for intervals shrunk\n");
    }
    set->counter = --counter;
}

int find_index_by_value(struct SetInterval* set, struct Interval* i) {
    int limit = set->counter;
    for (int count = 0; count < limit; count++) {
        if (set->in_array[count]->a == i->a && set->in_array[count]->b == i->b) {
            return count;
        }
    }
    return -1;
}

struct Interval* get_by_index(struct SetInterval* set, int index) {
    struct Interval* i = set->in_array[index];
    if (DEBUG) printf("Interval %s%g;%g%s retrieved\n", i->left == open ? "(" : "[", i->a, i->b, i->right == open ? ")" : "]");

    return i;
}

/**
 * Returns intersection of all intervals
 *
 * @param set of intervals
 * @return result of intersection or NULL
 */
struct Interval* get_intersection(struct SetInterval* set) {
    if (set->counter == 0) return NULL;
    struct Interval* intersection = set->in_array[0];
    if (set->counter == 1) return intersection;
    for (int i = 1; i < set-> counter; i++) {
        struct Interval* next = set->in_array[i];
        if (intersection->b < next->a) {
            return NULL;
        }
        if (intersection->a == next->a) {
            intersection->left = intersection->left == open ? intersection->left : next->left;
        } else if (intersection->a < next->a) {
            intersection->a = next->a;
            intersection->left = next->left;
        }
        if (intersection->b == next->b) {
            intersection->right = intersection->right == open ? intersection->right : next->right;
        } else if (intersection->b > next->b) {
            intersection->b = next->b;
            intersection->right = next->right;
        }
    }
    if (intersection->a == intersection->b) {
        if (intersection->left == closed && intersection->right == closed) {
            // result is single point
        } else {
            return NULL;
        }
    }

    return intersection;
}

/**
 * Compares two intervals by start (left margin)
 *
 * @param a_ptr firstCoefficient(left) interval
 * @param b_ptr next(right) interval
 * @return -1 if firstCoefficient is less, 1 when is greater than secondCoefficient, 0 if equals
 */
int compare_start(const void* a_ptr, const void* b_ptr) {
    const struct Interval* a_str = (struct Interval*)a_ptr;
    const struct Interval* b_str = (struct Interval*)b_ptr;
    double a = a_str->a;
    double b = b_str->a;
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    // [1;+INFINITY] < (1;+INFINITY]
    return a_str->left == open && b_str->left == closed ? 1 : a_str->left == closed && b_str->left == open ? -1 : 0;
}

/**
 * Compares two intervals by end (right margin)
 *
 * @param a_ptr firstCoefficient(left) interval
 * @param b_ptr next(right) interval
 * @return -1 if firstCoefficient is less, 1 when greater than secondCoefficient, 0 if equals
 */
int compare_end(const void* a_ptr, const void* b_ptr) {
    struct Interval* a_str = (struct Interval*)a_ptr;
    struct Interval* b_str = (struct Interval*)b_ptr;
    double a = a_str->b;
    double b = b_str->b;
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    // (-INFINITY;1) < (-INFINITY;1]
    return a_str->right == open && b_str->right == closed ? -1 : a_str->right == closed && b_str->right == open ? 1 : 0;
}

/**
 * Compares two intervals start to end (left margin of the firstCoefficient interval to right margin of the secondCoefficient one)
 *
 * @param a_ptr firstCoefficient(left) interval
 * @param b_ptr next(right) interval
 * @return -1 if firstCoefficient is less, 1 when greater than secondCoefficient, 0 if equals
 */
int compare_start_to_end(const void* a_ptr, const void* b_ptr) {
    struct Interval* a_str = (struct Interval*)a_ptr;
    struct Interval* b_str = (struct Interval*)b_ptr;
    double a = a_str->a;
    double b = b_str->b;
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    // A = (1;+INFINITY), B = (-INFINITY;1] => A > B
    return a_str->left == open && b_str->right == closed ? 1 : a_str->left == closed && b_str->right == open ? -1 : 0;
}

/**
 * Checks if two sorted intervals intersect
 *
 * @param a_ptr firstCoefficient(left) interval
 * @param b_ptr next(right) interval
 * @return true if intersect, otherwise false
 */
bool is_joint(struct Interval* first_str, struct Interval* next_str) {
    if (first_str->b == INFINITY) {
        return true;
    }
    int first_end = (int)(first_str->b * 100000); //round double value to comparable int
    int next_start = (int)(next_str->a * 100000);
    if (first_end < next_start) {
        return false;
    }
    if (first_end > next_start) {
        return true;
    }
    return first_str->right == closed && next_str->left == closed ? true : false;
}

/**
 * Sorts intervals in set by start (left margin)
 *
 * @param set of intervals
 */
void sort_intervals(struct SetInterval* set) {
    struct Interval** intervals = set->in_array;
    int size = set->counter;
    struct Interval arr[size];
    for (int i = 0; i < size; i++) {
        arr[i] = *intervals[i];
    }
    qsort(arr, size, sizeof(struct Interval), compare_start);
    for (int i = 0; i < size; ++i) {
        *set->in_array[i] = arr[i];
    }
    if (DEBUG) {printf(__func__ ); for (int i = 0; i < set->counter; ++i) {print_interval(set->in_array[i]);}; printf("\n");}
}

/**
 * Returns union of intervals
 *
 * @param set of intervals to merge
 * @return set of intervals as a result of merge
 */
struct SetInterval* get_union(struct SetInterval* set) {
    if (DEBUG) {printf(__func__ ); for (int i = 0; i < set->counter; ++i) {print_interval(set->in_array[i]);}; printf("\n");}
    struct SetInterval* union_set = init();
    if (set->counter == 0) {
        return union_set;
    }
    sort_intervals(set);
    //assign current result to the firstCoefficient interval
    struct Interval* prototype = set->in_array[0];
    add(union_set, prototype->left, prototype->right, prototype->a, prototype->b);
    if (set->counter == 1) {
        return union_set;
    }
    struct Interval* current = union_set->in_array[union_set->counter - 1];
    for (int i = 1; i < set->counter; i++) {
        struct Interval* next = set->in_array[i];
        bool edges_join = current->b == next->a && !(current->right == open && next->left == open);
        if (!is_joint(current, next) && !edges_join) {
            //when intervals disjoint, add next interval to the result
            add(union_set, next->left, next->right, next->a, next->b);
            current = union_set->in_array[union_set->counter - 1];
        } else if (compare_end(current, next) == -1 || edges_join) {
            //if intervals joint and right endpoint of the next one is greater, merge result and the next interval
            current->b = next->b;
            current->right = next->right;
        }
    }
    if (DEBUG) {printf(__func__ ); for (int i = 0; i < union_set->counter; ++i) {print_interval(union_set->in_array[i]);}; printf("\n");}
    return union_set;
}

/**
 * Computes difference between two intervals: (A)\(B)
 *
 * @param set that contains intervals
 * @return difference between intervals
 */
struct SetInterval* get_difference(struct Interval* interval_A, struct Interval* interval_B) {
    if (DEBUG) {printf(__func__ ); print_interval(interval_A); printf("\\"); print_interval(interval_B); printf("\n");}

    struct SetInterval* result = init();
    if (compare_start_to_end(interval_A, interval_B) > 0 || compare_start_to_end(interval_B, interval_A) > 0) {
        //intervals don't overlap, return A
        add(result, interval_A->left, interval_A->right, interval_A->a, interval_A->b);
        if (DEBUG) {printf(__func__ ); for (int i = 0; i < result->counter; ++i) {print_interval(result->in_array[i]);}; printf("\n");}
        return result;
    }
    bool starts_with_A = compare_start(interval_A, interval_B) < 0;
    bool ends_with_A = compare_end(interval_A, interval_B) > 0;
    if (starts_with_A) {
        if (ends_with_A) {
            add(result, interval_A->left, interval_B->left == closed ? open : closed, interval_A->a, interval_B->a);
            add(result, interval_B->right == closed ? open : closed, interval_A->right, interval_B->b, interval_A->b);
        } else {
            add(result, interval_A->left, interval_B->left == closed ? open : closed, interval_A->a, interval_B->a);
        }
    } else {
        if (ends_with_A) {
            add(result, interval_B->right == closed ? open : closed, interval_A->right, interval_B->b, interval_A->b);
        } else {
            // do nothing
        }
    }
    if (DEBUG) {printf(__func__ ); for (int i = 0; i < result->counter; ++i) {print_interval(result->in_array[i]);}; printf("\n");}
    return result;
}

struct Interval* get_copy(struct Interval* interval) {
    struct Interval* result = calloc(1, sizeof(struct Interval));
    result->a = interval->a;
    result->b = interval->b;
    result->left = interval->left;
    result->right = interval->right;
    return result;
}

bool equals(struct Interval* given, struct Interval* that) {
    if (given == NULL || that == NULL) return false;
    return given->left == that->left && given->a == that->a && given->b == that->b && given->right == that->right;
}

void print_interval(struct Interval* i) {
    printf("%s%g;%g%s", i->left == open ? "(" : "[", i->a, i->b, i->right == open ? ")" : "]");
}

void print_set(struct SetInterval* set) {
    printf("Intervals in set: %d\n", set->counter);
    for (int i = 0; i < set->counter; i++) {
        print_interval(get_by_index(set, i));
        printf("\n");
    }
}
