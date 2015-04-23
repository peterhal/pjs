#! /bin/bash

cat files.list | xargs nodejs stdlib.js > tt.js
