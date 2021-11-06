#!/bin/bash
# Rename symbols ending with _start and _end in $1 to $3_start, $3_end in $2

start="$(nm -a $1 | awk '/_start$/ {print $3}')"
end="$(nm -a $1 | awk '/_end$/ {print $3}')"

objcopy --redefine-sym "$start"="${3}_start" --redefine-sym "$end"="${3}_end" "$1" "$2"
