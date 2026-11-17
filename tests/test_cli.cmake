function(expect_rejection)
    execute_process(COMMAND "${APP}" ${ARGN}
                    RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(NOT "${status}" STREQUAL "1" OR NOT output STREQUAL "" OR NOT error MATCHES "Error:")
        message(FATAL_ERROR "Invalid arguments did not fail cleanly: ${ARGN}")
    endif()
endfunction()

expect_rejection()
expect_rejection(unknown 4 3 42)
expect_rejection(dfs 0 3 42)
expect_rejection(dfs -1 3 42)
expect_rejection(dfs 4 0 42)
expect_rejection(dfs 4 3 -1)
expect_rejection(dfs 4 3 4294967296)
expect_rejection(dfs 2147483648 3 42)
expect_rejection(dfs 4x 3 42)
expect_rejection(dfs 4 3 42 unused.txt extra)
expect_rejection(--analyze)
expect_rejection(--analyze dfs 4 3 -1)
expect_rejection(--analyze unknown 4 3 42)
expect_rejection(--compare)
expect_rejection(--compare 4 3 42 0)
expect_rejection(--compare 4 3 42 -1)
expect_rejection(--compare 4 3 4294967296)

file(MAKE_DIRECTORY "${TEST_DIR}")
foreach(pair IN ITEMS "A,dfs" "B,kruskal" "C,sidewinder")
    string(REPLACE "," ";" names "${pair}")
    list(GET names 0 short_name)
    list(GET names 1 full_name)
    execute_process(COMMAND "${APP}" "${short_name}" 4 3 42
                    RESULT_VARIABLE status OUTPUT_VARIABLE expected ERROR_VARIABLE error)
    if(NOT "${status}" STREQUAL "0" OR expected STREQUAL "" OR NOT error STREQUAL "")
        message(FATAL_ERROR "Generation failed: ${short_name}")
    endif()
    set(output_path "${TEST_DIR}/maze ${full_name}.txt")
    execute_process(COMMAND "${APP}" "${full_name}" 4 3 42 "${output_path}"
                    RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(NOT "${status}" STREQUAL "0" OR NOT output STREQUAL "" OR NOT error STREQUAL "")
        message(FATAL_ERROR "Saving failed: ${full_name}")
    endif()
    file(READ "${output_path}" saved)
    if(NOT saved STREQUAL expected)
        message(FATAL_ERROR "Saved file differs from stdout: ${full_name}")
    endif()
    string(REGEX MATCHALL "[^\n]+" lines "${saved}")
    list(LENGTH lines row_count)
    if(NOT row_count EQUAL 7)
        message(FATAL_ERROR "Wrong rendered height")
    endif()
    foreach(line IN LISTS lines)
        string(LENGTH "${line}" columns)
        if(NOT columns EQUAL 9)
            message(FATAL_ERROR "Wrong rendered width")
        endif()
    endforeach()
    execute_process(COMMAND "${APP}" "${full_name}" 1 1 4294967295
                    RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(NOT "${status}" STREQUAL "0" OR NOT output STREQUAL "###\n#S#\n###\n")
        message(FATAL_ERROR "1x1 generation failed: ${full_name}")
    endif()
    execute_process(COMMAND "${APP}" "${full_name}" 4 3 42 "${TEST_DIR}/missing/output.txt"
                    RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(NOT "${status}" STREQUAL "1" OR NOT error MATCHES "Cannot open output file")
        message(FATAL_ERROR "Output error was not reported: ${full_name}")
    endif()
endforeach()

execute_process(COMMAND "${APP}" --analyze dfs 1 5 42 "${TEST_DIR}/analyzed.txt"
                RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(NOT "${status}" STREQUAL "0" OR NOT error STREQUAL "" OR
   NOT output MATCHES "Dead ends: 2" OR NOT output MATCHES "Longest path \\(moves\\): 4" OR
   NOT output MATCHES "Average straight corridor \\(moves\\): 4.000000" OR
   NOT output MATCHES "Generation time \\(ms\\): [0-9]+\\.[0-9]+" OR
   NOT output MATCHES "Perfect maze: yes")
    message(FATAL_ERROR "Analyzer output incorrect: ${output} ${error}")
endif()
file(READ "${TEST_DIR}/analyzed.txt" saved)
if(NOT saved MATCHES "#S#" OR NOT saved MATCHES "#E#")
    message(FATAL_ERROR "Analyze command did not save maze")
endif()
# Removed workflows must not remain exposed by the executable.
expect_rejection(--compare 4 3 42)
execute_process(COMMAND "${APP}" --help
                RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(NOT "${status}" STREQUAL "0" OR output MATCHES "--compare|output.csv|repeats")
    message(FATAL_ERROR "Help exposes workflows outside the C++ role")
endif()
foreach(name IN ITEMS dfs kruskal sidewinder)
    execute_process(COMMAND "${APP}" --analyze "${name}" 1 1 42
                    RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(NOT "${status}" STREQUAL "0" OR NOT error STREQUAL "" OR
       NOT output MATCHES "Dead ends: 0" OR
       NOT output MATCHES "Longest path \\(moves\\): 0" OR
       NOT output MATCHES "Average straight corridor \\(moves\\): 0.000000" OR
       output MATCHES "S-to-E|Straight corridor count")
        message(FATAL_ERROR "1x1 analyzer metrics incorrect: ${name} ${output} ${error}")
    endif()
endforeach()
message(STATUS "CLI input, aliases, maze output and four analyzer measurements passed")
