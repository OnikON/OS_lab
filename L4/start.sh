#!/bin/bash
rm end.txt
touch end.txt
cat text.txt | xargs ./main > end.txt