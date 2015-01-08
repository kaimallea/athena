#include "MatchState.hpp"

void MatchState::Enter(Match* m)
{
}

void MatchState::HandleEvent(Match* m, IGameEvent* e)
{
}

void MatchState::Exit(Match* m)
{
}

void MatchState::ChangeState(Match* m, MatchState* s)
{
    m->ChangeState(s);
}
