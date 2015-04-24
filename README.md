# pjs
Hack PhP to JavaScript transpiler written in Hack.

# Getting Started

Install [hhvm](http://hhvm.com/) and [nodejs](nodejs.org) and ensure that they are both on your path.

# Testing
Currently the only test is selfhosting. To convert the transpiler from hack to js run:

> ./conv.sh

This produces the converted transpiler in t.js. To test the transpiled code, use the transpiled transpiler to transpile the original hack source to javascript:

> ./conv-selfhost.sh

Which produces another copy of the transpiled code in tt.js. Verify that the output of the original transpiler and the transpiled transpiler are equal:

> diff t.js tt.js

Should yield no diffs.

# Overview

- Utils - contains utilities for reading text files, reporting errors, and handling characters.
- Syntax - Hack front end including lexer and parser.
- Convert - Converts parse trees to javascript.
- tokenDumpProgram.php - tokenizes hack source and writes a list of tokens to stdout.
- parseProgram.php - parses hack source, and write parse trees to stdout.
- convertProgram.php - converts a list of hack sources to javascript and writes the result to stdout.
- stdlib.js - Polyfils for Hack standard library in javascript.
- convertMain.js - the javascript version of convertProgram.php. Loads stdlib.js and t.js (the transpiled hack code) and converts a list of hack source to javascript.

# Status

Only those constructs required by the transpiler are complete. The transpiler will (usually) throw when encountering constructs
it can't handle.

Some notable TODOs:

- parent within a class (other than parent::__construct)
- instance property initializers
- foreach with keys and values
- e1 ?: e2
- new static()
- traits
- $ quoted expressions in string literals
- function statics
- yield, async methods
- attributes
- type aliases
- intrinsics - echo, exit, clone, array
- standard library - only those parts of the Hack standard library are

Some notable hacks:

- namespaces - all namespace members are dumped into the global namespace.
- files should be converted to nodejs modules
