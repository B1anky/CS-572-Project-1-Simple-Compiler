#include <iostream>
#include <string>

#include <assert.h>

#include "eval.h"
#include "lexer.h"


int
main(int argc, const char *argv[])
{
  const std::string prompt(">> ");
  std::string line;
  while ((std::cout << prompt) &&
	 std::getline(std::cin, line)) {
    line += "\n";
    Lexer lexer(line);
    EvalResult eval(Eval(lexer).NextEvalResult());
    switch (eval.status) {
    case EvalStatus::ok:
      std::cout << eval.result << std::endl;
      break;
    case EvalStatus::empty:
      continue;
    case EvalStatus::error:
      std::cout << "syntax error\n";
      std::cout << line <<
	std::string(lexer.offset() - 1, ' ') << "^\n";
      break;
    case EvalStatus::eof:
      //should not occur since nl-terminated
      assert(0);
      break;
    }
    line.clear();
  }
  return 0;
}
