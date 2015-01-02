#include "FSM.h"

STATE FSM::GetCurrentState(void)
{
    return state;
}

/**
 * This FSM is event-based, so transitioning
 * between states requires an event.
 */
void FSM::Transition(EVENT event)
{
    switch(state)
    {
        case ST_READY_UP:
            switch(event)
            {
                case EVT_PLAYERS_READY:
                    state = ST_KNIFE_ROUND;
                    break;
                default:
                    break;
            }
            break;

        case ST_KNIFE_ROUND:
            switch(event)
            {
                case EVT_ROUND_END:
                    state = ST_DECIDE_SIDE;
                    break;
                default:
                    break;
            }
            break;

        case ST_DECIDE_SIDE:
            switch(event)
            {
                case EVT_DECISION_MADE:
                    state = ST_FIRST_HALF_READY_UP;
                    break;
                default:
                    break;
            }
            break;

        case ST_FIRST_HALF_READY_UP:
            switch(event)
            {
                case EVT_PLAYERS_READY:
                    state = ST_FIRST_HALF;
                    break;
                default:
                    break;
            }
            break;

        case ST_FIRST_HALF:
            switch(event)
            {
                case EVT_HALF_END:
                    state = ST_HALFTIME;
                    break;
                default:
                    break;
            }
            break;

        case ST_HALFTIME:
            switch(event)
            {
                case EVT_HALFTIME_END:
                    state = ST_SECOND_HALF_READY_UP;
                    break;
                default:
                    break;
            }
            break;

        case ST_SECOND_HALF_READY_UP:
            switch(event)
            {
                case EVT_PLAYERS_READY:
                    state = ST_SECOND_HALF;
                    break;
                default:
                    break;
            }
            break;

        case ST_SECOND_HALF:
            switch(event)
            {
                case EVT_HALF_END:
                    state = ST_FINAL;
                    break;
                default:
                    break;
            }
            break;

        case ST_FINAL:
            break;

        default:
            break;
    }
}
