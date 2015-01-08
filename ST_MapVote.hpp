#ifndef ST_MAP_VOTE_H_
#define ST_MAP_VOTE_H_

#include "MatchState.hpp"

class ST_MapVote : public MatchState
{
public:
    static ST_MapVote* Instance();

    virtual void Enter(Match* m);
    virtual void HandleEvent(Match* m, IGameEvent*);
    virtual void Exit(Match* m);
private:
    static ST_MapVote* _instance;
};

#endif // ST_MAP_VOTE_H_
