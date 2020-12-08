#ifndef C_INTERVAL_H
#define C_INTERVAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Interval_type {
    open,
    closed
} Interval_type;


typedef struct Interval {
    enum Interval_type left;
    enum Interval_type right;
    double a;
    double b;
} Interval;

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#ifdef __cplusplus
}
#endif

#endif //C_INTERVAL_H