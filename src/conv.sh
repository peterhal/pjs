#! /bin/bash

find . -name '*.hh' | xargs hhvm convertProgram.php
