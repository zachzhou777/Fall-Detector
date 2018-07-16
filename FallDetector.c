/*
 * Threshold-based fall detection algorithm.
 *
 * Author: Zach Zhou
 */

#include "FallDetector.h"
#include <math.h>

typedef enum {
    IDLE,
    IMPACT,
    ACTIVITY
} state_t;

/*
 * Algorithm is essentially a state machine. Each time this function is called, 
 * a state transition occurs. The expectation is that this function is called 
 * at a constant frequency of 100 Hz.
 *
 * Params:
 *   x, y, z: the x-, y-, and z-components of acceleration in m/s^2
 * Returns:
 *   1 if a fall is detected, 0 otherwise
 */
int run_fall_detector(double x, double y, double z) {
    static state_t state                       = IDLE;
    static int     cycles_above_threshold      = 0;
    
    // Maintain a circular buffer that indicates whether a given cycle is 
    // considered "active"
    static char    cycle_buffer[BUFFER_SIZE];
    static int     cycle_buffer_idx            = 0;
    
    // Compute magnitude of acceleration
    double accel_magnitude = sqrt(x*x + y*y + z*z);
    
    switch (state) {
        case IDLE:
            // IDLE to IMPACT transition
            if (accel_magnitude > IDLE_TO_IMPACT_ACCEL_THRESHOLD) {
                state = IMPACT;
            }
            break;
            
        case IMPACT:
            // In this state, 'cycles_buffer_idx' serves the dual purpose of 
            // counting the number of cycles spent in the state
            
            // IMPACT to IMPACT transitions
            if (cycle_buffer_idx < BUFFER_SIZE) {
                if (accel_magnitude > ACTIVITY_ACCEL_THRESHOLD) {
                    cycles_above_threshold++;
                    cycle_buffer[cycle_buffer_idx] = 1;
                }
                else {
                    cycle_buffer[cycle_buffer_idx] = 0;
                }
                cycle_buffer_idx++;
            }
            
            // IMPACT to ACTIVITY transition
            else if (cycles_above_threshold > ACTIVITY_THRESHOLD_CYCLES) {
                state = ACTIVITY;
                cycle_buffer_idx = 0;
            }
            
            // IMPACT to IDLE transition
            else {
                // This transition indicates that a fall has been detected
                
                // Reset state variables
                state = IDLE;
                cycles_above_threshold = 0;
                for (int i = 0; i < BUFFER_SIZE; i++) {
                    cycle_buffer[i] = 0;
                }
                cycle_buffer_idx = 0;
                
                return 1;
            }
            break;
            
        default /* ACTIVITY state */:
            // ACTIVITY to ACTIVITY transition
            if (cycles_above_threshold > ACTIVITY_THRESHOLD_CYCLES) {
                cycles_above_threshold -= cycle_buffer[cycle_buffer_idx];
                if (accel_magnitude > ACTIVITY_ACCEL_THRESHOLD) {
                    cycles_above_threshold++;
                    cycle_buffer[cycle_buffer_idx] = 1;
                }
                else {
                    cycle_buffer[cycle_buffer_idx] = 0;
                }
                cycle_buffer_idx = (cycle_buffer_idx + 1) % BUFFER_SIZE;
            }
            
            // ACTIVITY to IDLE transition
            else {
                // Reset state variables
                state = IDLE;
                cycles_above_threshold = 0;
                for (int i = 0; i < BUFFER_SIZE; i++) {
                    cycle_buffer[i] = 0;
                }
                cycle_buffer_idx = 0;
            }
    }
    
    return 0;
}
