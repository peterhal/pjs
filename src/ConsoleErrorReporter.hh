<?hh //strict

require_once 'ErrorReporter.hh';

namespace Utils {

class ConsoleErrorReporter implements ErrorReporter
{
  public function error(string $message): void
  {
    fwrite(STDERR, $message . "\n");
  }
}

}
