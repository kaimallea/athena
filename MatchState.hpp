#ifndef MATCH_STATE_H_
#define MATCH_STATE_H_

#include "Match.hpp"

class MatchState
{
public:
    virtual void Enter(Match* m);
    virtual void HandleEvent(Match* m, IGameEvent* e);
    virtual void Exit(Match* m);
protected:
    void ChangeState(Match* m, MatchState* s);
};

#endif // MATCH_STATE_H_
