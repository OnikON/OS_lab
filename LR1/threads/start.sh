#!/bin/bash

strace -o strace_main.txt ./Q1
strace -f -o strace.txt ./Q1