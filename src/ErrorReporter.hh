<?hh //strict

interface ErrorReporter
{
  public function error(string $message): void;
}
