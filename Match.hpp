#ifndef MATCH_H_
#define MATCH_H_

#include <igameevents.h>

class MatchState;

class Match
{
public:
    Match();

    void Enter();
    void HandleEvent(IGameEvent*);
    void Exit();
private:
    friend class MatchState;
    void ChangeState(MatchState*);
private:
    MatchState* _state;
};

#endif // MATCH_H_
