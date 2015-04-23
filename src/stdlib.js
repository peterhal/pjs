
var fs = require('fs');

var count = function(values) {
  return values.length;
};

var STDOUT = 1;
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

var str_repeat = function(value, count) {
  var result = '';
  while (count > 0) {
    result += value;
    count --;
  }
  return result;
};

var ord = function(value) {
  return value.charCodeAt(0);
};

var chr = function(value) {
  return String.fromCharCode(value);
};

var PHP_INT_MAX = 1 << 32;

Array.prototype.count = function() {
  return this.length;
};

Array.prototype.add = function(value) {
  this.push(value);
};

Array.prototype.linearSearch = function (value) {
  for (var index = 0; index < this.length; index++) {
    if (this[index] == value) return true;
  }
  return false;
};

// Map
Object.prototype.containsKey = function(key) {
  return this.hasOwnProperty(key);
};

Object.prototype.get = function(key) {
  return this[key];
};

var invariant = function(condition, message) {
  if (!condition) {
    throw new Error(message);
  }
};

global.Exception = Error;


eval(fs.readFileSync('./t.js').toString());

try {
  tokenDumpMain(['t.js', 'Utils/IndentedWriter.hh']);
} catch (e) {
  fwrite(STDERR, e.message + "\n");
  fwrite(STDERR, e.stack);
  fwrite(STDERR, "\n");
}

