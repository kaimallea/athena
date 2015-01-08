#include "Match.hpp"
#include "ST_MapVote.hpp"

Match::Match()
{
    _state = ST_MapVote::Instance();
}

void Match::Enter()
{
    _state->Enter(this);
}

void Match::Exit()
{
    _state->Exit(this);
}

void Match::ChangeState(MatchState* s)
{
    Exit();
    _state = s;
    Enter();
}

void Match::HandleEvent(IGameEvent *e)
{
    _state->HandleEvent(this, e);
}
