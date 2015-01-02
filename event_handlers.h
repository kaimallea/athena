/**
 * vim: set ts=4 sw=4 tw=99 noet:
g*/

#include<map>
#include"igameevent.h"

#ifndef ATHENA_EVENT_HANDLER
#define ATHENA_EVENT_HANDLER

class EventHandler
{
	private:
		typedef void (*Callback)();
		std::map <int, Callback> event_map;

	int handler(IGameEvent *pEvent);
	void player_death(IGameEvent *pEvent);
};

#endif //ATHENA_EVENT_HANDLER
