/**
 * vim: set ts=4 sw=4 tw=99 noet:
g*/

#ifndef ATHENA_EVENT_HANDLER
#define ATHENA_EVENT_HANDLER

class EventHandler
{
	int handler(IGameEvent *pEvent);
	void player_death(IGameEvent *pEvent);
};

#endif //ATHENA_EVENT_HANDLER
