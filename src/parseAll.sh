#! /bin/bash

find . -name '*.hh' | xargs hhvm parseProgram.php

