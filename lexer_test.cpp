#include "lexer.h"
#include "eval.h"

#include <string>

#include <gtest/gtest.h>

TEST(LexerTest, Eof) {
  Token eof(TokenKind::eof, "");
  EXPECT_EQ(eof, Lexer("").NextToken());
  EXPECT_EQ(eof, Lexer(" \t ").NextToken());
}

TEST(LexerTest, Newline) {
  Token nl(TokenKind::nl, "\n");
  EXPECT_EQ(nl, Lexer("\n").NextToken());
  EXPECT_EQ(nl, Lexer(" \t \n").NextToken());
}
  
TEST(LexerTest, Operators) {
  Token add(TokenKind::add, "+");
  EXPECT_EQ(add, Lexer("+").NextToken());
  EXPECT_EQ(add, Lexer(" \t +").NextToken());
  Token sub(TokenKind::sub, "-");
  EXPECT_EQ(sub, Lexer("-").NextToken());
  EXPECT_EQ(sub, Lexer(" \t -").NextToken());
}

TEST(LexerTest, Integers) {
  for (auto i : {0, 1, 12, 1<<16,}) {
    std::string s = std::to_string(i);
    Token expected(TokenKind::integer, s, i);
    EXPECT_EQ(expected, Lexer(s).NextToken());
    EXPECT_EQ(expected,
	      Lexer(" \t " + s).NextToken());
  }
}

TEST(LexerTest, Sequence1) {
  int i1 = 63;
  int i2 = 20;
  int i3 = 11;
  auto tokens = {
    Token(TokenKind::integer, std::to_string(i1), i1),
    Token(TokenKind::add, "+"),
    Token(TokenKind::integer, std::to_string(i2), i2),
    Token(TokenKind::sub, "-"),
    Token(TokenKind::integer, std::to_string(i3), i3),
    Token(TokenKind::nl, "\n"),
    Token(TokenKind::eof, ""),
  };
  std::string s;
  for (auto token : tokens) {
    s += " \t"; s += token.lexeme;
  }
  Lexer lexer = Lexer(s);
  int i = 0;
  for (auto token : tokens) {
    EXPECT_EQ(token, lexer.NextToken()) <<
      "for index " << i;
    ++i;
  }
}

//My tests
TEST(LexerTest, Errors) {
  int i1 = 1;
  std::vector<Token> tokens = {
    Token(TokenKind::leftP, "("),
    Token(TokenKind::integer, std::to_string(i1), i1),
    Token(TokenKind::rightP, ")"),
  };
  std::string s1;
  std::string s2;
  std::string s3;
  std::string s4;
  std::string s5;
  std::string s6;

  std::vector<std::string> strs = {
    s1 += tokens[0].lexeme, //"("
    s2 += tokens[2].lexeme, //")"
    s3 += s1 + s2, //"()"
    s4 += tokens[0].lexeme + tokens[1].lexeme, //"(1"
    s5 += tokens[1].lexeme + tokens[2].lexeme, //"1)"
    s6 += s4 + s2 + s2,
  };

  for(auto str: strs){
    Lexer lexer = Lexer(str);
    Eval evl(lexer);
    const EvalStatus err(EvalStatus::error);
    EXPECT_EQ(err, evl.NextEvalResult().status);
  }
}
