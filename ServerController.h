#ifndef ATHENA_SERVERCONTROLLER_H_
#define ATHENA_SERVERCONTROLLER_H_

#define MAX_COMMAND_LENGTH 256

class ServerController
{
public:
    void RestartGame( int delay = 1 );
    void SwapTeams( void );
    void SetTeamName( int team, const char *name );
    void SetTeamFlag( int team, const char *country_code );
    void PauseMatch( void );
    void UnpauseMatch( void );
};

#endif // ATHENA_SERVERCONTROLLER_H_