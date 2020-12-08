#ifndef C_TESTS_H
#define C_TESTS_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include "interval.h"
#include "set_interval.h"

static const bool DEBUG_TEST = false;

int run_tests();

#ifdef __cplusplus
}
#endif

#endif