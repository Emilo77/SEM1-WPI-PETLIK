cmake_minimum_required(VERSION 3.22)
project(petlik C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-std=c17 -pedantic -Wall -Wextra -Wformat-security -Wduplicated-cond -Wfloat-equal -Wshadow -Wconversion -Wjump-misses-init -Wlogical-not-parentheses -Wnull-dereference -Wvla -Werror -fstack-protector-strong -fsanitize=undefined -fno-sanitize-recover -g -fno-omit-frame-pointer -O1")

add_executable(petlik petlik.c)
#add_executable(old_petlik old_petlik.c)
