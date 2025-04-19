#!/bin/bash

TERLANG="../build/ter"
if [ ! -f "$TERLANG" ]; then
    echo "Error: terlang interpreter not found at $TERLANG"
    echo "Please build the project first"
    exit 1
fi

total_tests=0
passed_tests=0
failed_tests=0

echo "Running tests..."
echo "----------------"

for ter_file in *.ter; do
    # Only if there is no .ter files
    [ -f "$ter_file" ] || continue

    ((total_tests++))

    out_file="${ter_file}.result"
    arg_file="${ter_file}.args"
    err_file="${ter_file}.error"

    # Run the test and capture output
    if [ ! -f "$arg_file" ]; then
        actual_output=$($TERLANG "$ter_file" 2>&1)
    else
        actual_output=$($TERLANG "$ter_file" $(cat "$arg_file") 2>&1)
    fi
    exit_code=$?
    actual_output_escaped=$(echo -n "$actual_output" | sed ':a;N;$!ba;s/\n/\\n/g')

    # Prepare expected output
    expected_output=""
    if [ -f "$out_file" ]; then
        expected_output+=$(cat "$out_file" | sed ':a;N;$!ba;s/\n/\\n/g')
    fi
    if [ -f "$err_file" ]; then
        expected_output+=$(cat "$err_file" | sed ':a;N;$!ba;s/\n/\\n/g')
    fi
    
    # Compare actual and expected output
    if [ "$exit_code" -ne 0 ] && [ ! -f "$err_file" ] ; then
        echo "Error at test $ter_file (non-zero exit code: $exit_code)"
        echo "Error:    '$actual_output'"
        ((failed_tests++))
    elif [ "$actual_output_escaped" != "$expected_output" ]; then
        echo "Error at test $ter_file"
        echo "Expected: '$(cat "$out_file")'"
        echo "Found:    '$actual_output'"
        ((failed_tests++))
    else
        ((passed_tests++))
    fi
done

echo "----------------"
echo "Test summary:"
echo "Total tests: $total_tests"
echo "Passed:      $passed_tests"
echo "Failed:      $failed_tests"

if [ "$failed_tests" -gt 0 ]; then
    exit 1
fi
exit 0
