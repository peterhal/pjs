<?hh // strict

function nullthrows<T>(?T $value, ?string $message = null): T {
  if ($value === null) {
    throw new Exception ($message !== null ? $message : 'Unexpected null');
  }

  return $value;
}
