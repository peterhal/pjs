
var fs = require('fs');

// file handles
var STDOUT = 1;
var STDERR = 2;

// file IO
var fwrite = function (file, value) {
  fs.writeSync(file, value);
};

var file_get_contents = function (filename) {
  return fs.readFileSync(filename).toString();
};

// string
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

var substr = function(value, start, length_opt)
{
  if (start < 0) {
    start = value.length + start;
  }
  if (value.length <= start) {
    return false;
  }
  if (start < 0) {
    throw new Error('TODO: substr error.');
  }
  if (arguments.length < 3) {
    length = value.length - start;
  } else if (!length_opt) {
    return '';
  } else if (length_opt < 0) {
    length = value.length - start + length_opt;
    if (length <= 0) {
      return false;
    }
  } else {
    length = Math.min(length_opt, value.length - start);
  }

  return value.substr(start, length);
}

// char
var ord = function(value) {
  return value.charCodeAt(0);
};

var chr = function(value) {
  return String.fromCharCode(value);
};

// int
var PHP_INT_MAX = 1 << 32;

// array
var count = function(values) {
  return values.length;
};

Array.prototype.count = function() {
  return this.length;
};

Array.prototype.add = function(value) {
  this.push(value);
};

Array.prototype.linearSearch = function (value) {
  for (var index = 0; index < this.length; index++) {
    if (this[index] == value) return index;
  }
  return -1;
};

Array.prototype.isEmpty = function() {
  return this.length === 0;
};

// Map
Object.prototype.containsKey = function(key) {
  return this.hasOwnProperty(key);
};

Object.prototype.get = function(key) {
  return this[key];
};

// builtins
var invariant = function(condition, message) {
  if (!condition) {
    throw new Error(message);
  }
};

global.Exception = Error;


eval(fs.readFileSync('./t.js').toString());

// Remove the leading 'nodejs'
// THe First element will be the .js file.
var argv = process.argv.slice(1);

  // tokenDumpMain(['', 'Utils/IndentedWriter.hh']);
  // parseMain(['', 'main.hh']);
  convertMain(argv);

