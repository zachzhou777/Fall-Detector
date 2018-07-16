#ifndef FALLDETECTOR_H
#define FALLDETECTOR_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define IDLE_TO_IMPACT_ACCEL_THRESHOLD 20.0
#define BUFFER_SIZE                    500
#define ACTIVITY_ACCEL_THRESHOLD       11.8
//#define ACTIVITY_THRESHOLD_FRACTION 0.3
//#define ACTIVITY_THRESHOLD_CYCLES
//                       (IMPACT_TO_ACTIVITY_THRESHOLD_FRACTION * IMPACT_CYCLES)
#define ACTIVITY_THRESHOLD_CYCLES      85

bool run_fall_detector(double x, double y, double z);

#endif // FALLDETECTOR_H
