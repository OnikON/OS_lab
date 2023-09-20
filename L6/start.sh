#!/bin/bash
rm end.txt
touch end.txt
cat test.txt | xargs ./main > end.txt