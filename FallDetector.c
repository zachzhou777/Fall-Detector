/**
 * Threshold-based fall detection algorithm.
 *
 * Author: Zach Zhou
 */

#include "FallDetector.h"

typedef enum {
    IDLE,
    IMPACT,
    ACTIVITY
} state_t;

static state_t state = IDLE;
static int cycles_above_threshold = 0;

// Maintain a circular buffer that indicates whether a given cycle is "active"
static bool cycle_buffer[BUFFER_SIZE];
static int cycle_buffer_idx = 0;

static double inline magnitude_of_vector(double x, double y, double z) {
    return sqrt(x*x + y*y + z*z);
}

static bool inline impact_detected(double accel_magnitude) {
    return accel_magnitude > IDLE_TO_IMPACT_ACCEL_THRESHOLD;
}

/**
 * TODO: explain myself
 */
static bool inline cycle_buffer_full(void) {
    return !cycle_buffer_idx;
}

static bool inline enough_cycles_in_buffer_were_active(void) {
    return cycles_above_threshold > ACTIVITY_THRESHOLD_CYCLES;
}

static bool inline activity_detected(double accel_magnitude) {
    return accel_magnitude > ACTIVITY_ACCEL_THRESHOLD;
}

static void inline add_record_to_cycle_buffer(bool record) {
    cycle_buffer[cycle_buffer_idx] = record;
    cycle_buffer_idx = (cycle_buffer_idx + 1) % BUFFER_SIZE;
}

static void inline record_activity(void) {
    cycles_above_threshold++;
    add_record_to_cycle_buffer(true);
}

static void inline record_no_activity(void) {
    add_record_to_cycle_buffer(false);
}

static void inline remove_oldest_record_from_cycle_buffer(void) {
    cycles_above_threshold -= cycle_buffer[cycle_buffer_idx];
}

static void inline go_to_state(state_t next_state) {
    state = next_state;
}

static void inline reset_state_machine(void) {
    go_to_state(IDLE);
    cycles_above_threshold = 0;
    cycle_buffer_idx = 0;
    // No need to zero out 'cycle_buffer'
}

/**
 * Algorithm is essentially a state machine. Each time this function is called, 
 * a state transition occurs. The expectation is that this function is called 
 * at a constant frequency of 100 Hz.
 *
 * Params:
 *   x, y, z: the x-, y-, and z-components of acceleration in m/s^2
 * Returns:
 *   1 if a fall is detected, 0 otherwise
 */
bool run_fall_detector(double x, double y, double z) {
    double accel_magnitude = magnitude_of_vector(x, y, z);
    
    switch (state) {
        case IDLE:
            if (impact_detected(accel_magnitude)) {
                record_activity();
                go_to_state(IMPACT);
            }
            break;
            
        case IMPACT:
            if (activity_detected(accel_magnitude)) {
                record_activity();
            }
            else {
                record_no_activity();
            }
            
            if (cycle_buffer_full()) {
                if (enough_cycles_in_buffer_were_active()) {
                    go_to_state(ACTIVITY);
                }
                else {
                    reset_state_machine();
                    return true;
                }
            }
            break;
            
        default /* ACTIVITY */:
            remove_oldest_record_from_cycle_buffer();
            if (activity_detected(accel_magnitude)) {
                record_activity();
            }
            else {
                record_no_activity();
            }
            
            if (!enough_cycles_in_buffer_were_active()) {
                reset_state_machine();
            }
    }
    
    return false;
}
