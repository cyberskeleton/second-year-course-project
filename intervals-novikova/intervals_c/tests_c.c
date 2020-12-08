#include "tests_c.h"

void populate(struct SetInterval* set, int lower_limit, int upper_limit, int elements) {
    srand(time(NULL));
    if(DEBUG_TEST) printf("Populating %d elements\n", elements);
    for (int i = 0; i < elements; i++) {
        double a = (double)(rand() % (upper_limit + 1 - lower_limit) + lower_limit) / 10.0;
        double b = (double)(rand() % (upper_limit + 1 - lower_limit) + lower_limit) / 10.0;
        enum Interval_type left = rand() % 2;
        enum Interval_type right = rand() % 2;
        add(set, left, right, MIN(a, b), MAX(a, b));
        if(DEBUG_TEST) printf("left: %s, right: %s\n", left == open ?"open":"closed", right == open ?"open":"closed");
    }
}

void test_find_index_by_value(struct SetInterval* set) {
    int expected = 1;
    struct Interval* interval = get_by_index(set, expected);
    struct Interval* invalid = (struct Interval*) malloc(sizeof(struct Interval));

    int actual = find_index_by_value(set, interval);
    int unexpected = find_index_by_value(set, invalid);

    assert(unexpected == -1);
    assert(actual == expected);
    printf("passed: %s\n", __func__);
    free(invalid);
}

void test_delete_by_index(struct SetInterval* set) {
    int index_to_remove = 1;
    struct Interval* to_be_removed = get_by_index(set, index_to_remove);
    struct Interval* to_be_kept = get_by_index(set, 2);

    delete_by_index(set, index_to_remove);
    struct Interval* actual = get_by_index(set, index_to_remove);
    int unexpected = find_index_by_value(set, to_be_removed);

    assert(actual == to_be_kept);
    assert(unexpected == -1);
    if(DEBUG_TEST) print_interval(to_be_kept);
    if(DEBUG_TEST) printf(" equals ");
    if(DEBUG_TEST) print_interval(actual);
    if(DEBUG_TEST) printf(": %s\n", to_be_kept == actual ? "true" : "false");
    printf("passed: %s\n", __func__);
}

void test_indexes() {
    struct SetInterval* set = init();
    int lower_limit = -100;
    int upper_limit = 100;
    int elements = 3;
    populate(set, lower_limit, upper_limit, elements);
    if(DEBUG_TEST) printf("---\n");
    if(DEBUG_TEST) print_set(set);
    test_find_index_by_value(set);
    test_delete_by_index(set);
    if(DEBUG_TEST) print_set(set);
    if(DEBUG_TEST) printf("---\n");
    free_set(set);
}

void test_equals_true() {
    struct Interval* first = (struct Interval*)malloc(sizeof(struct Interval));
    first->left = closed;
    first->a = -100;
    first->b = 100;
    first->right = open;
    struct Interval* last = (struct Interval*)malloc(sizeof(struct Interval));
    last->left = closed;
    last->a = -100;
    last->b = 100;
    last->right = open;

    assert(equals(first, last) == true);

    free(first);
    free(last);
    printf("passed: %s\n", __func__);
}

void test_equals_when_first_null() {
    struct Interval* first = (struct Interval*)malloc(sizeof(struct Interval));
    first->left = closed;
    first->a = -100;
    first->b = 100;
    first->right = open;
    struct Interval* last = NULL;

    assert(equals(first, last) == false);

    free(first);
    printf("passed: %s\n", __func__);
}

void test_equals_when_first_empty() {
    struct Interval* first = (struct Interval*)malloc(sizeof(struct Interval));
    first->left = closed;
    first->a = -100;
    first->b = 100;
    first->right = closed;
    struct Interval* last = (struct Interval*)malloc(sizeof(struct Interval));

    assert(equals(first, last) == false);

    free(first);
    free(last);
    printf("passed: %s\n", __func__);
}

void test_equals_false_when_a() {
    struct Interval* first = (struct Interval*)malloc(sizeof(struct Interval));
    first->left = closed;
    first->a = 100;
    first->b = 100;
    first->right = closed;
    struct Interval* last = (struct Interval*)malloc(sizeof(struct Interval));
    last->left = closed;
    last->a = -100;
    last->b = 100;
    last->right = closed;

    assert(equals(first, last) == false);

    free(first);
    free(last);
    printf("passed: %s\n", __func__);
}

void test_equals_false_when_b() {
    struct Interval* first = (struct Interval*)malloc(sizeof(struct Interval));
    first->left = closed;
    first->a = -100;
    first->b = 100;
    first->right = closed;
    struct Interval* last = (struct Interval*)malloc(sizeof(struct Interval));
    last->left = closed;
    last->a = -100;
    last->b = -100;
    last->right = closed;

    assert(equals(first, last) == false);

    free(first);
    free(last);
    printf("passed: %s\n", __func__);
}

void test_equals_false_when_left() {
    struct Interval* first = (struct Interval*)malloc(sizeof(struct Interval));
    first->left = closed;
    first->a = -100;
    first->b = 100;
    first->right = closed;
    struct Interval* last = (struct Interval*)malloc(sizeof(struct Interval));
    last->left = open;
    last->a = -100;
    last->b = 100;
    last->right = closed;

    assert(equals(first, last) == false);

    free(first);
    free(last);
    printf("passed: %s\n", __func__);
}

void test_equals_false_when_right() {
    struct Interval* first = (struct Interval*)malloc(sizeof(struct Interval));
    first->left = closed;
    first->a = -100;
    first->b = 100;
    first->right = closed;
    struct Interval* last = (struct Interval*)malloc(sizeof(struct Interval));
    last->left = closed;
    last->a = -100;
    last->b = 100;
    last->right = open;

    assert(equals(first, last) == false);

    free(first);
    free(last);
    printf("passed: %s\n", __func__);
}

void test_get_intersection_when_empty_set() {
    struct SetInterval* set = init();

    assert(get_intersection(set) == NULL);

    free_set(set);
    printf("passed: %s\n", __func__);
}

void test_get_intersection_when_single_entry_set() {
    struct SetInterval* set = init();
    add(set, open, closed, -1, 1);

    assert(equals(get_intersection(set), set->in_array[0]) == true);

    free_set(set);
    printf("passed: %s\n", __func__);
}

void test_get_intersection_when_first() {
    struct SetInterval* set = init();
    add(set, closed, open, -2, 2);
    add(set, open, closed, -5, 5);
    struct Interval* expected = set->in_array[0];

    assert(equals(get_intersection(set), expected) == true);

    free_set(set);
    printf("passed: %s\n", __func__);
}

void test_get_intersection_when_last() {
    struct SetInterval* set = init();
    add(set, open, open, -2, 2);
    add(set, closed, closed, -1, 1);
    struct Interval* expected = set->in_array[1];

    assert(equals(get_intersection(set), expected) == true);

    free_set(set);
    printf("passed: %s\n", __func__);
}

void test_get_intersection_when_between() {
    struct SetInterval* set = init();
    add(set, closed, open, -5, 3);
    add(set, open, open, -2, 2);
    add(set, closed, closed, 1, 3);
    add(set, open, open, 1, 5);
    struct Interval* expected = (struct Interval*)malloc(sizeof(struct Interval));
    expected->left = open;
    expected->a = 1;
    expected->b = 2;
    expected->right = open;
    struct Interval* result = get_intersection(set);

    assert(equals(result, expected) == true);

    free_set(set);
    printf("passed: %s\n", __func__);
}

void test_compare_lesser_when_by_value() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = -2.0;
    a->b = 0.0;
    a->right = closed;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = 0.0;
    b->b = 2.0;
    b->left = closed;

    assert(compare_start(a, b) == -1);
    assert(compare_end(a, b) == -1);
    assert(compare_start_to_end(a, b) == -1);
    assert(compare_start_to_end(b, a) == 0);

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_compare_greater_when_by_value() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 1.0;
    a->b = 2.0;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = -1.0;
    b->b = 0.0;

    assert(compare_start(a, b) == 1);
    assert(compare_end(a, b) == 1);
    assert(compare_start_to_end(a, b) == 1);
    assert(compare_start_to_end(b, a) == -1);

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_compare_lesser_when_by_type() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 0.0;
    a->b = 2.0;
    a->left = open;
    a->right = closed;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = 0.0;
    b->b = 2.0;
    b->left = closed;
    b->right = open;

    assert(compare_start(a, b) == 1);
    assert(compare_end(a, b) == 1);
    assert(compare_start_to_end(a, b) == -1);
    assert(compare_start_to_end(b, a) == -1);

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_compare_greater_when_by_type() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 0.0;
    a->b = 0.0;
    a->left = closed;
    a->right = open;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = 0.0;
    b->b = 0.0;
    b->left = open;
    b->right = closed;

    assert(compare_start(a, b) == -1);
    assert(compare_end(a, b) == -1);
    assert(compare_start_to_end(a, b) == 0);
    assert(compare_start_to_end(b, a) == 0);

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_compare_equals_when_closed() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 0.0;
    a->b = 2.0;
    a->left = closed;
    a->right = open;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = 0.0;
    b->b = 2.0;
    b->left = closed;
    b->right = open;

    assert(compare_start(a, b) == 0);
    assert(compare_end(a, b) == 0);
    assert(compare_start_to_end(a, b) == -1);
    assert(compare_start_to_end(b, a) == -1);

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_compare_equals_when_open() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 0.0;
    a->b = 2.0;
    a->left = open;
    a->right = closed;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = 0.0;
    b->b = 2.0;
    b->left = open;
    b->right = closed;

    assert(compare_start(a, b) == 0);
    assert(compare_end(a, b) == 0);
    assert(compare_start_to_end(a, b) == -1);
    assert(compare_start_to_end(b, a) == -1);

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_is_joint_true_when_infinity() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 0.0;
    a->b = INFINITY;
    struct Interval* b = calloc(1, sizeof(struct Interval));

    assert(is_joint(a, b));

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_is_joint_true_when_closed_both() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 0.0;
    a->b = 2.0;
    a->right = closed;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = 2.0;
    b->b = 3.0;
    b->left = closed;

    assert(is_joint(a, b));

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_is_joint_false_when_closed_left_open_right() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 0.0;
    a->b = 2.0;
    a->right = closed;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = 2.0;
    b->b = 3.0;
    b->left = open;

    assert(is_joint(a, b) == false);

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_is_joint_false_when_open_left_closed_right() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 0.0;
    a->b = 2.0;
    a->right = open;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = 2.0;
    b->b = 3.0;
    b->left = closed;

    assert(is_joint(a, b) == false);

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_is_joint_true_when_greater_value_left_end() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 0.0;
    a->b = 3.0;
    a->right = closed;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = 1.0;
    b->b = 3.0;
    b->left = closed;

    assert(is_joint(a, b));

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_is_joint_false_when_less_value_left_end() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 0.0;
    a->b = 1.0;
    a->right = closed;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = 2.0;
    b->b = 3.0;
    b->left = closed;

    assert(is_joint(a, b) == false);

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_is_joint_false_when_open_both() {
    struct Interval* a = calloc(1, sizeof(struct Interval));
    a->a = 0.0;
    a->b = 2.0;
    a->right = open;
    struct Interval* b = calloc(1, sizeof(struct Interval));
    b->a = 2.0;
    b->b = 3.0;
    b->left = open;

    assert(is_joint(a, b) == false);

    free(a);
    free(b);
    printf("passed: %s\n", __func__);
}

void test_sort_intervals() {
    struct SetInterval* set = init();
    if (DEBUG_TEST) printf("Before:\n");
    // third after sort
    add(set, open, closed, 15.0, 20.0);
    // second after sort
    add(set, open, closed, 10.0, 20.0);
    // first after sort
    add(set, closed, open, 10.0, 20.0);

    sort_intervals(set);

    if (DEBUG_TEST) printf("After:\n");
    struct Interval* i_0 = get_by_index(set, 0);
    struct Interval* i_1 = get_by_index(set, 1);
    struct Interval* i_2 = get_by_index(set, 2);

    assert(i_0->a == 10.0 && i_0->left == closed);
    assert(i_1->a == 10.0 && i_1->left == open);
    assert(i_2->a == 15.0);
    free_set(set);
    printf("passed: %s\n", __func__);
}

void test_get_union_empty_set() {
    struct SetInterval* set = init();

    struct SetInterval* result = get_union(set);

    assert(result->counter == 0);
    free_set(set);
    free_set(result);
    printf("passed: %s\n", __func__);
}

void test_get_union_one_when_given_one() {
    struct SetInterval* set = init();
    add(set, open, closed, 15.0, 20.0);

    struct SetInterval* result = get_union(set);
    struct Interval* interval = result->in_array[0];

    assert(result->counter == 1);
    assert(interval->left == set->in_array[0]->left);
    assert(interval->right == set->in_array[0]->right);
    assert(interval->a == set->in_array[0]->a);
    assert(interval->b == set->in_array[0]->b);
    free_set(set);
    free_set(result);
    printf("passed: %s\n", __func__);

}

void test_get_union_one_when_intersection() {
    struct SetInterval* set = init();
    add(set, closed, open, 20.0, 30.0);
    add(set, open, closed, 15.0, 20.0);

    struct SetInterval* result = get_union(set);
    struct Interval* interval = result->in_array[0];

    assert(result->counter == 1);
    assert(interval->left == set->in_array[0]->left);
    assert(interval->right == set->in_array[1]->right);
    assert(interval->a == set->in_array[0]->a);
    assert(interval->b == set->in_array[1]->b);
    free_set(set);
    free_set(result);
    printf("passed: %s\n", __func__);
}

void test_get_union_two_when_no_intersection() {
    struct SetInterval* set = init();
    add(set, open, closed, 15.0, 20.0);
    add(set, closed, open, 25.0, 30.0);

    struct SetInterval* result = get_union(set);
    struct Interval* first = result->in_array[0];
    struct Interval* second = result->in_array[1];

    assert(result->counter == 2);
    assert(first->left == set->in_array[0]->left);
    assert(first->right == set->in_array[0]->right);
    assert(first->a == set->in_array[0]->a);
    assert(first->b == set->in_array[0]->b);
    assert(second->left == set->in_array[1]->left);
    assert(second->right == set->in_array[1]->right);
    assert(second->a == set->in_array[1]->a);
    assert(second->b == set->in_array[1]->b);
    free_set(set);
    free_set(result);
    printf("passed: %s\n", __func__);
}

void test_get_difference_start_A_greater_end_B() {
    struct Interval* interval_A = malloc(sizeof(struct Interval));
    interval_A->a = 10.0;
    interval_A->b = 12.0;
    interval_A->left = open;
    interval_A->right = closed;
    struct Interval* interval_B = malloc(sizeof(struct Interval));
    interval_B->a = 0.0;
    interval_B->b = 2.0;
    interval_B->left = open;
    interval_B->right = closed;

    struct SetInterval* result = get_difference(interval_A, interval_B);

    assert(equals(result->in_array[0], interval_A));
    assert(result->counter == 1);
    free(interval_A);
    free(interval_B);
    free_set(result);
    printf("passed: %s\n", __func__);
}

void test_get_difference_start_B_greater_end_A() {
    struct Interval* interval_A = malloc(sizeof(struct Interval));
    interval_A->a = 0.0;
    interval_A->b = 2.0;
    interval_A->left = open;
    interval_A->right = closed;
    struct Interval* interval_B = malloc(sizeof(struct Interval));
    interval_B->a = 10.0;
    interval_B->b = 12.0;
    interval_B->left = open;
    interval_B->right = closed;

    struct SetInterval* result = get_difference(interval_A, interval_B);

    assert(equals(result->in_array[0], interval_A));
    assert(result->counter == 1);
    free(interval_A);
    free(interval_B);
    free_set(result);
    printf("passed: %s\n", __func__);
}

void test_get_difference_starts_with_A_ends_with_A() {
    struct Interval* interval_A = malloc(sizeof(struct Interval));
    interval_A->a = 0.0;
    interval_A->b = 4.0;
    interval_A->left = open;
    interval_A->right = closed;
    struct Interval* interval_B = malloc(sizeof(struct Interval));
    interval_B->a = 1.0;
    interval_B->b = 2.0;
    interval_B->left = open;
    interval_B->right = closed;

    struct SetInterval* result = get_difference(interval_A, interval_B);

    assert(result->in_array[0]->a == interval_A->a);
    assert(result->in_array[0]->b == interval_B->a);
    assert(result->in_array[0]->left == interval_A->left);
    assert(result->in_array[0]->right == interval_B->left == closed ? open : closed);
    assert(result->in_array[1]->a == interval_B->b);
    assert(result->in_array[1]->b == interval_A->b);
    assert(result->in_array[1]->left == interval_B->right == closed ? open : closed);
    assert(result->in_array[1]->right == interval_A->right);
    assert(result->counter == 2);
    free(interval_A);
    free(interval_B);
    free_set(result);
    printf("passed: %s\n", __func__);
}

void test_get_difference_starts_with_A_ends_with_B() {
    struct Interval* interval_A = malloc(sizeof(struct Interval));
    interval_A->a = 0.0;
    interval_A->b = 3.0;
    interval_A->left = open;
    interval_A->right = closed;
    struct Interval* interval_B = malloc(sizeof(struct Interval));
    interval_B->a = 2.0;
    interval_B->b = 4.0;
    interval_B->left = open;
    interval_B->right = closed;

    struct SetInterval* result = get_difference(interval_A, interval_B);

    assert(result->in_array[0]->a == interval_A->a);
    assert(result->in_array[0]->b == interval_B->a);
    assert(result->in_array[0]->left == interval_A->left);
    assert(result->in_array[0]->right == interval_B->left == closed ? open : closed);
    assert(result->counter == 1);
    free(interval_A);
    free(interval_B);
    free_set(result);
    printf("passed: %s\n", __func__);
}

void test_get_difference_starts_with_B_ends_with_A() {
    struct Interval* interval_A = malloc(sizeof(struct Interval));
    interval_A->a = 2.0;
    interval_A->b = 4.0;
    interval_A->left = open;
    interval_A->right = closed;
    struct Interval* interval_B = malloc(sizeof(struct Interval));
    interval_B->a = 1.0;
    interval_B->b = 3.0;
    interval_B->left = open;
    interval_B->right = closed;

    struct SetInterval* result = get_difference(interval_A, interval_B);

    assert(result->in_array[0]->a == interval_B->b);
    assert(result->in_array[0]->b == interval_A->b);
    assert(result->in_array[0]->left == interval_A->left);
    assert(result->in_array[0]->right == interval_B->right);
    assert(result->counter == 1);
    free(interval_A);
    free(interval_B);
    free_set(result);
    printf("passed: %s\n", __func__);
}

void test_get_difference_starts_with_B_ends_with_B() {
    struct Interval* interval_A = malloc(sizeof(struct Interval));
    interval_A->a = 2.0;
    interval_A->b = 3.0;
    interval_A->left = open;
    interval_A->right = closed;
    struct Interval* interval_B = malloc(sizeof(struct Interval));
    interval_B->a = 1.0;
    interval_B->b = 4.0;
    interval_B->left = open;
    interval_B->right = closed;

    struct SetInterval* result = get_difference(interval_A, interval_B);

    assert(result->in_array[0] == NULL);
    assert(result->counter == 0);
    free(interval_A);
    free(interval_B);
    free_set(result);
    printf("passed: %s\n", __func__);
}

void test() {

    // TODO implement
    printf("not implemented: %s\n", __func__);
}

/**
 * tests runner
 *
 * @return
 */
int run_tests() {
    test_indexes();

    test_equals_true();
    test_equals_when_first_null();
    test_equals_when_first_empty();
    test_equals_false_when_a();
    test_equals_false_when_b();
    test_equals_false_when_left();
    test_equals_false_when_right();

    test_get_intersection_when_empty_set();
    test_get_intersection_when_single_entry_set();
    test_get_intersection_when_first();
    test_get_intersection_when_last();
    test_get_intersection_when_between();

    test_compare_lesser_when_by_value();
    test_compare_greater_when_by_value();
    test_compare_lesser_when_by_type();
    test_compare_greater_when_by_type();
    test_compare_equals_when_closed();
    test_compare_equals_when_open();

    test_is_joint_true_when_infinity();
    test_is_joint_true_when_closed_both();
    test_is_joint_false_when_closed_left_open_right();
    test_is_joint_false_when_open_left_closed_right();
    test_is_joint_true_when_greater_value_left_end();
    test_is_joint_false_when_less_value_left_end();
    test_is_joint_false_when_open_both();

    test_sort_intervals();

    test_get_union_empty_set();
    test_get_union_one_when_given_one();
    test_get_union_one_when_intersection();
    test_get_union_two_when_no_intersection();

    test_get_difference_start_A_greater_end_B();
    test_get_difference_start_B_greater_end_A();
    test_get_difference_starts_with_A_ends_with_A();
    test_get_difference_starts_with_A_ends_with_B();
    test_get_difference_starts_with_B_ends_with_A();
    test_get_difference_starts_with_B_ends_with_B();

    return 0;
}
