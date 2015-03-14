<?hh

$arg1 = $argv[1];

$file_contents = file_get_contents($arg1);
var_dump($file_contents);

