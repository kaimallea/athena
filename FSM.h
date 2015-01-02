#ifndef ATHENA_FSM_H_
#define ATHENA_FSM_H_

typedef enum {
    ST_READY_UP,
    ST_KNIFE_ROUND,
    ST_DECIDE_SIDE,
    ST_FIRST_HALF_READY_UP,
    ST_FIRST_HALF,
    ST_HALFTIME,
    ST_SECOND_HALF_READY_UP,
    ST_SECOND_HALF,
    ST_FINAL
} STATE;

typedef enum {
    EVT_PLAYERS_READY,
    EVT_ROUND_END,
    EVT_DECISION_MADE,
    EVT_HALF_END,
    EVT_HALFTIME_END
} EVENT;


class FSM
{
    STATE state;
public:
    FSM() : state(ST_READY_UP) {}
    void Transition(EVENT e);
    STATE GetCurrentState(void);
};


#endif // ATHENA_FSM_H_
