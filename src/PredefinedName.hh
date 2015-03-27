<?hh // strict

abstract class PredefinedName
{
  const string hh = 'hh';

  const string pn_await = 'await'; // TODO: SPEC: is await a keyword?
  const string this = 'this';
  const string bool = 'bool';
  const string int = 'int';
  const string float = 'float';
  const string mixed = 'mixed';
  const string pn_resource = 'resource';
  const string pn_array = 'array';
  const string string = 'string';
  const string void = 'void';
  const string __construct = '__construct';
  const string __destruct = '__destruct';
}

