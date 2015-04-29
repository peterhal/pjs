#! /bin/sh
HHVM=${HHVM:-hhvm}
hhvm ParseTreeGeneratorProgram.php Syntax/ParseTreeKind.hh Syntax/ParseTree.hh Syntax/ParseTrees.hh

