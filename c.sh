#!/bin/bash

g++ -std=c++20 -Wall -Wextra main.cpp -o smart_pointers_test
#./smart_pointers_test

valgrind --leak-check=full ./smart_pointers_test
