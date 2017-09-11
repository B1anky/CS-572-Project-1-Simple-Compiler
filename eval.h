#ifndef EVAL_H_
#define EVAL_H_

#include "lexer.h"
#include <deque>

enum class EvalStatus {
    ok, empty, error, eof
};

struct EvalResult {
    const EvalStatus status;
    const int result;
};

class Eval {
    public:
        Eval(Lexer& lexer) :
        lexer_(lexer), token_(lexer.NextToken()) {}
        const EvalResult NextEvalResult();
    private:
        Lexer &lexer_;
        Token token_;       //lookahead token
};

#endif //ifndef EVAL_H_