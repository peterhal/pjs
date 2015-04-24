var fs = require('fs');

// TODO: modules.
eval(fs.readFileSync('./stdlib.js').toString());
eval(fs.readFileSync('./t.js').toString());

convertMain(argv);

