<?hh //strict

require_once 'ErrorReporter.hh';

class ConsoleErrorReporter implements ErrorReporter
{
  public function error(string $message): void
  {
    fwrite(STDERR, $message);
  }
}
