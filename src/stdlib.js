
var fs = require('fs');
eval(fs.readFileSync('./t.js').toString());

var count = function(values) {
  return values.length;
};

var STDIO = 1;
var STDERR = 2;

var fwrite = function (file, value) {
  fs.writeSync(file, value);
};

var file_get_contents = function (filename) {
  return fs.readFileSync(filename).toString();
};

var strlen = function(value) {
  return value.length;
};

var ord = function(value) {
  return value.charCodeAt(0);
};

var PHP_INT_MAX = 1 << 32;

Array.prototype.count = function() {
  return this.length;
};

Array.prototype.add = function(value) {
  this.push(value);
};

convertMain(['t.js', 'Utils/IndentedWriter.hh']);

