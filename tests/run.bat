@echo off
setlocal enabledelayedexpansion

:: Bat version is simpler, it does not check exit codes.
:: Also newlines are disregarded when comparing results.

set TERLANG=..\build\Debug\ter.exe
if not exist "%TERLANG%" (
    echo Error: terlang interpreter not found at %TERLANG%
    echo Please build the project first
    exit /b 1
)

set total_tests=0
set passed_tests=0
set failed_tests=0

echo Running tests...
echo --------------------------------

for %%f in (*.ter) do (
    set /a total_tests+=1

    set ter_file=%%f
    set out_file=%%f.result
    set arg_file=%%f.args
    set err_file=%%f.error

    :: Run the test and capture output
    set actual_output=
    if not exist "!arg_file!" (
        for /f "delims=" %%o in ('%TERLANG% "!ter_file!" 2^>^&1') do set actual_output=!actual_output!%%o
    ) else (
        :: set /p args=< arg_file
        for /f "delims=" %%a in (!arg_file!) do set args=!args! %%a
        for /f "delims=" %%o in ('%TERLANG% "!ter_file!" !args! 2^>^&1') do set actual_output=!actual_output!%%o
    )
    set actual_output_escaped=!actual_output!

    :: Prepare expected output
    set expected_output=
    if exist "!out_file!" (
        for /f "delims=" %%e in (!out_file!) do set expected_output=!expected_output!%%e
    )
    if exist "!err_file!" (
        for /f "delims=" %%e in (!err_file!) do set expected_output=!expected_output!%%e
    )

    :: Compare actual and expected output
    if "!actual_output_escaped!" equ "!expected_output!" (
        set /a passed_tests+=1
    ) else (
        echo Error at test !ter_file!
        echo Expected: '!expected_output!'
        echo Found:    '!actual_output!'
        set /a failed_tests+=1
    )
)

echo --------------------------------
echo Test summary:
echo Total tests: %total_tests%
echo Passed:      %passed_tests%
echo Failed:      %failed_tests%

if %failed_tests% gtr 0 (
    exit /b 1
)
exit /b 0
