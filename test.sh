#!/bin/bash

# Iterating through all tests with .in extension
for input_file in tests/*.in; do

    output_file="${input_file%.in}.out"

    ./petlik < "$input_file" > temporary_output.txt

    # Comparing our answer with test
    if diff -q temporary_output.txt "$output_file" > /dev/null ; then
        echo "Test $input_file correct"
    else
        echo "Test $input_file incorrect"
        echo "Expected: $(cat $output_file)"
        echo "Received: $(cat temporary_output.txt)"
    fi

    # Erasing temporary file
    rm temporary_output.txt
done
