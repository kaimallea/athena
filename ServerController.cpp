#include "ServerController.h"
#include <eiface.h>

extern IVEngineServer *engine;

void ServerController::RestartGame( int delay )
{
    char cmd[MAX_COMMAND_LENGTH];

    if (!delay)
        delay = 1;

    snprintf( cmd, sizeof(cmd), "mp_restartgame %d\n", delay );
    engine->ServerCommand( (const char *)cmd );
}


void ServerController::SetTeamName( int team, const char *name )
{
    if (team != 2 && team != 3)
        return;

    // Quirk: commands are mp_teamflag_1 and mp_teamflag_2,
    // for CTs and Ts, respectively. But internally, CTs and Ts
    // are teams 3 and 2, respectively, in the game.
    if (team == 3)
        team = 1;

    if (!strlen( name ))
        return;

    char cmd[MAX_COMMAND_LENGTH];
    snprintf( cmd, sizeof(cmd), "mp_teamname_%d %s\n", team, name );
    engine->ServerCommand( (const char *)cmd );
}


void ServerController::SetTeamFlag( int team, const char *country_code )
{
    if (team != 2 && team != 3)
        return;

    // Quirk: commands are mp_teamflag_1 and mp_teamflag_2,
    // for CTs and Ts, respectively. But internally, CTs and Ts
    // are teams 3 and 2, respectively, in the game.
    if (team == 3)
        team = 1;

    if (!strlen( country_code ))
        return;

    char cmd[MAX_COMMAND_LENGTH];
    snprintf( cmd, sizeof(cmd), "mp_teamflag_%d %s\n", team, country_code );
    engine->ServerCommand( (const char *)cmd );
}


void ServerController::PauseMatch( void )
{
    engine->ServerCommand( "mp_pause_match\n" );
}


void ServerController::UnpauseMatch( void )
{
    engine->ServerCommand( "mp_unpause_match\n" );
}


void ServerController::SwapTeams( void )
{
    engine->ServerCommand( "mp_swapteams\n" );
}
