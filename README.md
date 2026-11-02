# Inlupp 2: Webstore backend

# Build and test

make test        build and run test files using CUnit
make memtest     run the tests and check for memory leaks using Valgrind
make coverage    run the tests and check coverage using gcov
make clean       clear the files created when using make

Requires gcc, make, CUnit and Valgrind.

# Programs

    make run

Runs the menu, allowing use of all implemented functions.

# Design decisions

# Test coverage

File 'db.c'
Lines executed:88.81% of 268
Branches executed:97.78% of 90
Taken at least once:74.44% of 90
Calls executed:92.74% of 124
Creating 'db.c.gcov'

Lines executed:88.81% of 268