#!/usr/bin/python

import numpy as np
import itertools

PARAMETER_SWEEP_SPECIFIERS_FILENAME = 'parameter_sweep_specifiers.csv'
PARAMETER_SETTINGS_TO_TEST = 'parameter_settings_to_test.csv'
PARAMETER_SWEEP_START_SPECIFIER_INDEX = 1
PARAMETER_SWEEP_STOP_SPECIFIER_INDEX = 2
PARAMETER_SWEEP_STEP_SPECIFIER_INDEX = 3

def main():
    parameter_sweep_intervals = get_intervals_from_file()
    with open(PARAMETER_SETTINGS_TO_TEST, 'w') as file_object:
        write_parameter_settings_to_file(parameter_sweep_intervals, file_object)

def get_intervals_from_file():
    """Gets parameter sweep intervals from the sweep specifiers file.
    
    Retrieves the start, stop, and step values for each parameter listed in the 
    sweep specifiers file. Uses these to create an interval for each parameter.
    
    Returns:
        A list of arrays, where the ith element is the sweep interval of the 
        parameter in the (i+1)th line of the sweep specifiers file.
    """
    all_parameter_intervals = []
    file_contents = get_contents_of_parameter_sweep_specifiers_file()
    file_lines = file_contents.splitlines()
    for file_line in file_lines:
        interval_specifier = get_interval_specifier_from_file_line(file_line)
        parameter_interval = create_interval_from_specifier(interval_specifier)
        all_parameter_intervals.append(parameter_interval)
    return all_parameter_intervals

def get_contents_of_parameter_sweep_specifiers_file():
    with open(PARAMETER_SWEEP_SPECIFIERS_FILENAME, 'r') as file_object:
        file_contents = file_object.read()
        return file_contents

def get_interval_specifier_from_file_line(file_line):
    """Gets an interval specifier from a line from the sweep specifier file.
    
    Gets an interval specifier (start, stop, step) from a line of the sweep 
    specifier file.
    
    Args:
        file_line: A line from the sweep specifier file.
    
    Returns:
        A tuple of the form (<start>, <stop>, <step>).
    """
    parameter_specifiers = file_line.split(',')
    start = float(parameter_specifiers[PARAMETER_SWEEP_START_SPECIFIER_INDEX])
    stop = float(parameter_specifiers[PARAMETER_SWEEP_STOP_SPECIFIER_INDEX])
    step = float(parameter_specifiers[PARAMETER_SWEEP_STEP_SPECIFIER_INDEX])
    interval_specifier = start, stop, step
    return interval_specifier

def create_interval_from_specifier((start, stop, step)):
    parameter_interval = np.arange(start, stop, step)
    return parameter_interval

def write_parameter_settings_to_file(parameter_sweep_intervals, file_object):
    for parameter_setting in itertools.product(*parameter_sweep_intervals):
        parameter_setting_string = tuple_to_string_of_comma_separated_values(
            parameter_setting)
        file_object.write(parameter_setting_string + '\n')

def tuple_to_string_of_comma_separated_values(t):
    tuple_string = str(t)
    tuple_string_without_parentheses = tuple_string[1:-1]
    return tuple_string_without_parentheses

if __name__ == '__main__':
    main()
