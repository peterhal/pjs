<?hh // strict

namespace Utils {

use \Exception;

function nullthrows<T>(?T $value, ?string $message = null): T {
  if ($value === null) {
    throw new Exception ($message !== null ? $message : 'Unexpected null');
  }

  return $value;
}

function contains<T>(Vector<T> $values, T $value): bool
{
  return -1 !== $values->linearSearch($value);
}

}
