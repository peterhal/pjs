#! /bin/bash
HHVM=${HHVM:-hhvm}
cat files.list | xargs $HHVM convertProgram.php > t.js
