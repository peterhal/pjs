#! /bin/bash
NODEJS={NODEJS:-nodejs}
cat files.list | xargs $NODEJS convertMain.js > tt.js
