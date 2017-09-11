#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <string>

enum TokenKind {
  //language tokens
  add,
  integer,
  nl,
  sub,
  leftP,
  rightP,
  nil,

  //technical tokens
  error,
  eof,
};

struct Token {
  TokenKind kind;
  std::string lexeme;
  int value;

  Token(const TokenKind kind, const std::string lexeme,
        const int value=0)
  : kind(kind), lexeme(lexeme), value(value)  {
  }
};

inline bool operator==(const Token& tok1, const Token& tok2) {
  return tok1.kind == tok2.kind && tok1.lexeme == tok2.lexeme &&
         (tok1.kind != TokenKind::integer || tok1.value == tok2.value);
}

inline bool operator!=(const Token &tok1, const Token &tok2) {
  return !(tok1 == tok2);
}

inline std::ostream& operator<<(std::ostream& os, const Token tok) {
  os << "Token(kind=" << tok.kind << ", lexeme=" << tok.lexeme;
  if (tok.kind == TokenKind::integer) {
    os << ", value=" << tok.value;
  }
  os <<  ")";
  return os;
}

class Lexer {
 public:
  Lexer(const std::string& input, unsigned int offset=0)
    : buffer_(input), index_(offset),
      size_(input.size())
  {
  }
  const Token NextToken();
  size_t offset() const { return index_; }

 private:
  const std::string buffer_;
  size_t index_;
  const size_t size_;

  const Token IntegerToken();
};

#endif //#ifndef LEXER_H_