<?hh //strict

namespace Utils {

interface ErrorReporter
{
  public function error(string $message): void;
}

}

