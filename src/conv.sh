#! /bin/bash

cat files.list | xargs hhvm convertProgram.php
