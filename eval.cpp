#include "eval.h"

#include "lexer.h"


enum State { want_integer, want_operator, want_leftP, want_rightP };

const EvalResult Eval::NextEvalResult() {
    std::deque<std::pair<int, TokenKind>> stck;
    int leftPCnt = 0;
    if(token_.kind == TokenKind::nl){
        return EvalResult({EvalStatus::empty, 0});
    }else if (token_.kind == TokenKind::eof) {
        return EvalResult({EvalStatus::eof, 0});
    }

    if(token_.kind == TokenKind::rightP && !leftPCnt){
        return EvalResult({EvalStatus::error, 0});
    }
    auto result = 0;
    auto state1 = State::want_integer;

    auto sign = 1;
    auto is_error = false;  
    int intCnt = 0;
    while (!is_error && token_.kind != TokenKind::nl && token_.kind != TokenKind::eof) {
        if(token_.kind == TokenKind::leftP){
            state1 = State::want_integer;
            leftPCnt++;
            stck.push_back(std::make_pair(0, TokenKind::leftP));
        }else if(token_.kind == TokenKind::rightP){
            state1 = State::want_operator;
            leftPCnt--;
            if(leftPCnt < 0){
                return  EvalResult({EvalStatus::error, 0});
            }
            //pop and add/subtract previous elements up to last left paren, then pop left paren
            auto it = stck.end();
            it--;
            int popCnt = 0;
            while(it->second != TokenKind::leftP){
                it--;
                popCnt++;
            }
            popCnt++;
            auto opType = TokenKind::nil;
            int temp = 0;
            while(it != stck.end()){
                if(it->second == TokenKind::integer && opType == TokenKind::nil){
                    temp += it->first;
                }else if(it->second == TokenKind::integer && opType != TokenKind::nil){
                    if(opType == TokenKind::add){
                        if(it->first < 0){
                            temp -= it->first;
                        }else{
                            temp += it->first;
                        }
                    }else if(opType == TokenKind::sub){
                        if(it->first < 0){
                            temp += abs(it->first);
                        }else{
                            temp -= it->first;
                        }
                    }
                    opType = TokenKind::nil;
                }else if(it->second == TokenKind::add){
                    opType = TokenKind::add;
                }else if(it->second == TokenKind::sub){
                     opType = TokenKind::sub;
                }
                it++;
            }
            for(int i = 0; i < popCnt; i++) stck.pop_back();
            stck.push_back(std::make_pair(temp, TokenKind::integer));
        }else if(state1 == State::want_integer){
            state1 = State::want_operator;
            is_error |= (token_.kind != TokenKind::integer);
            if (!is_error){
                sign = (token_.kind == TokenKind::sub) ? -1 : +1;
                stck.push_back(std::make_pair(sign * token_.value, TokenKind::integer));
                intCnt++;
            }
        }else if(state1 == State::want_operator){
            state1 = State::want_integer;
            is_error |= (token_.kind != TokenKind::add && token_.kind != TokenKind::sub);
            if(!is_error) {
                sign = (token_.kind == TokenKind::sub) ? -1 : +1;
                if(sign > 0){
                    stck.push_back(std::make_pair(0, TokenKind::add));
                }else{
                     stck.push_back(std::make_pair(0, TokenKind::sub));
                }
            }
        } //different if cases, used to be switches
        if (!is_error){
            token_ = lexer_.NextToken();
        }
    } //while 
    is_error |= state1 != State::want_operator;
    is_error |= leftPCnt > 0;
    is_error |= !intCnt;

    if(stck.size() <= 2){
        result = stck.back().first;
    }else if(stck.size() > 2){
        auto opType = TokenKind::nil;
        for(int i = 0; i < stck.size(); i++){
            if(stck[i].second == TokenKind::integer){
                if(opType == TokenKind::nil){
                    result = stck[i].first;
                }else if(opType == TokenKind::add){
                    if(stck[i].first < 0){
                        result -= abs(stck[i].first);
                    }else{
                        result += abs(stck[i].first);
                    }
                }else if(opType == TokenKind::sub){
                    if(stck[i].first < 0){
                        result += abs(stck[i].first);
                    }else{
                        result -= abs(stck[i].first);
                    }
                }
                opType = TokenKind::nil;
            }else if(stck[i].second == TokenKind::add){
                opType = TokenKind::add;
            }else if(stck[i].second == TokenKind::sub){
                opType = TokenKind::sub;
            }
        }
    }

    return is_error ?  EvalResult({EvalStatus::error, 0}) : EvalResult({EvalStatus::ok, result});
}