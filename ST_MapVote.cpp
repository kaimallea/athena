#include "ST_MapVote.hpp"
#include "ServerController.h"
#include "UserMessageHelper.h"
#include <iplayerinfo.h>
#include "athena_mm.h"

extern AthenaPlugin g_AthenaPlugin;
extern UserMessageHelper *usermessagehelper;

ST_MapVote* ST_MapVote::_instance = 0;

ST_MapVote* ST_MapVote::Instance()
{
    if (_instance == 0)
        _instance = new ST_MapVote;

    return _instance;
}

void ST_MapVote::Enter(Match* m)
{
    ServerController s;
    s.RestartGame();
}

void ST_MapVote::Exit(Match* m)
{
}

void ST_MapVote::HandleEvent(Match* m, IGameEvent *e)
{
    const char *eventName = e->GetName();
    if (strcmp(eventName, "player_say") == 0)
    {
        int userid = e->GetInt("userid");
        const char *text = e->GetString("text");
        IPlayerInfo *playerinfo = g_AthenaPlugin.GetPlayerInfoByUserId(userid);

        if ( playerinfo->IsObserver() || playerinfo->IsFakeClient() )
            return;

        if (strcmp(text, ".vote") == 0)
        {
            usermessagehelper->PrintToChatAll(
                "[%s%s%s]: %s tried to vote.",
                PURPLE,
                g_AthenaPlugin.GetLogTag(),
                WHITE,
                playerinfo->GetName()
            );
        }
    }
}
