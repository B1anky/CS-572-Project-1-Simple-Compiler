#include "lexer.h"

#include <assert.h>
#include <ctype.h>

#include <iostream>

const Token Lexer::NextToken() {

    //eat linear whitespace
    while (index_ < size_ && buffer_[index_] != '\n' && isspace(buffer_[index_])) {
        ++index_;
    }

    if (index_ >= size_) {
        return Token(TokenKind::eof, "");
    }

    char ch = buffer_[index_];
    if (isdigit(ch)) {
        return IntegerToken();
    }else if (ch == '\n') {
        ++index_;
        return Token(TokenKind::nl, "\n");
    }else if (ch == '+') {
        ++index_;
        return Token(TokenKind::add, "+");
    }else if (ch == '-') {
        ++index_;
        return Token(TokenKind::sub, "-");
    }else if(ch == '('){
        ++index_;
        return Token(TokenKind::leftP, "(");
    }else if(ch == ')'){
        ++index_;
        return Token(TokenKind::rightP, ")");
  }
  std::string lexeme = buffer_.substr(index_, 1);
  ++index_;
  return Token(TokenKind::error, lexeme);
}

const Token Lexer::IntegerToken() {
    assert(isdigit(buffer_[index_]));

    size_t start = index_;
    while (index_ < size_ && isdigit(buffer_[index_])){
        ++index_;
    }
    std::string lexeme = buffer_.substr(start, index_ - start);
    return Token(TokenKind::integer, lexeme, std::stoi(lexeme));
}

#ifdef TEST_LEXER

#include <fstream>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

static void
tokenize_istream(std::istream &in, std::ostream &out)
{
  std::string line;
  while (std::getline(in, line)) {
    line += "\n";
    Lexer lexer(line);
    while (true) {
      Token token(lexer.NextToken());
      out << token << std::endl;
      if (token.kind == TokenKind::eof) break;
    }
    line.clear();
  }
}

int
main(int argc, const char *argv[])
{
  if (argc == 1) {
    std::cerr << "usage: " << argv[0] << " FILE...\n"
	      << "specify FILE as - for stdin\n";
    exit(1);
  }
  
  for (int i = 1; i < argc; ++i) {
    bool is_stdin = (std::string(argv[i]) == "-");
    if (is_stdin)
      tokenize_istream(std::cin, std::cout);
    else {
      std::ifstream in(argv[i], std::ios::in);
      if (in.fail()) {
	std::cerr << "cannot read " << argv[i]
		  << ": " << strerror(errno) << std::endl;
	exit(1);
      }
      tokenize_istream(in, std::cout);
      in.close();
    }
  }    
  return 0;
}

#endif 