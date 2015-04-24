#! /bin/bash

cat files.list | xargs nodejs convertMain.js > tt.js
