/**
 * vim: set ts=4 sw=4 tw=99 noet :
 */

#include <stdio.h>
#include <stdarg.h>
#include "athena_mm.h"

SH_DECL_HOOK6(IServerGameDLL, LevelInit, SH_NOATTRIB, 0, bool, char const *, char const *, char const *, char const *, bool, bool);
SH_DECL_HOOK3_void(IServerGameDLL, ServerActivate, SH_NOATTRIB, 0, edict_t *, int, int);
SH_DECL_HOOK1_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool);
SH_DECL_HOOK0_void(IServerGameDLL, LevelShutdown, SH_NOATTRIB, 0);
SH_DECL_HOOK2_void(IServerGameClients, ClientActive, SH_NOATTRIB, 0, edict_t *, bool);
SH_DECL_HOOK1_void(IServerGameClients, ClientDisconnect, SH_NOATTRIB, 0, edict_t *);
SH_DECL_HOOK2_void(IServerGameClients, ClientPutInServer, SH_NOATTRIB, 0, edict_t *, char const *);
SH_DECL_HOOK1_void(IServerGameClients, SetCommandClient, SH_NOATTRIB, 0, int);
SH_DECL_HOOK1_void(IServerGameClients, ClientSettingsChanged, SH_NOATTRIB, 0, edict_t *);
SH_DECL_HOOK2_void(IServerGameClients, NetworkIDValidated, SH_NOATTRIB, 0, const char *, const char *);
SH_DECL_HOOK2_void(IServerGameClients, ClientCommand, SH_NOATTRIB, 0, edict_t *, const CCommand &);
SH_DECL_HOOK5(IServerGameClients, ClientConnect, SH_NOATTRIB, 0, bool, edict_t *, const char*, const char *, char *, int);
SH_DECL_HOOK2(IGameEventManager2, FireEvent, SH_NOATTRIB, 0, bool, IGameEvent *, bool);

MatchState g_MatchState = WARMUP;
AthenaPlugin g_AthenaPlugin;
IServerGameDLL *server = NULL;
IServerGameClients *gameclients = NULL;
IVEngineServer *engine = NULL;
IServerPluginHelpers *helpers = NULL;
IGameEventManager2 *gameevents = NULL;
IServerPluginCallbacks *vsp_callbacks = NULL;
IPlayerInfoManager *playerinfomanager = NULL;
ICvar *icvar = NULL;
CGlobalVars *gpGlobals = NULL;

UserMessageHelper *usermessagehelper = NULL;

ConVar sample_cvar("sample_cvar", "42", 0);

/**
 * Something like this is needed to register cvars/CON_COMMANDs.
 */
class BaseAccessor : public IConCommandBaseAccessor
{
public:
	bool RegisterConCommandBase(ConCommandBase *pCommandBase)
	{
		/* Always call META_REGCVAR instead of going through the engine. */
		return META_REGCVAR(pCommandBase);
	}
} s_BaseAccessor;

PLUGIN_EXPOSE(AthenaPlugin, g_AthenaPlugin);
bool AthenaPlugin::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetEngineFactory, gameevents, IGameEventManager2, INTERFACEVERSION_GAMEEVENTSMANAGER2);
	GET_V_IFACE_CURRENT(GetEngineFactory, helpers, IServerPluginHelpers, INTERFACEVERSION_ISERVERPLUGINHELPERS);
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetServerFactory, server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_ANY(GetServerFactory, gameclients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS);
	GET_V_IFACE_ANY(GetServerFactory, playerinfomanager, IPlayerInfoManager, INTERFACEVERSION_PLAYERINFOMANAGER);

	gpGlobals = ismm->GetCGlobals();

	META_LOG(g_PLAPI, "Starting plugin.");

	/* Load the VSP listener.  This is usually needed for IServerPluginHelpers. */
	if ((vsp_callbacks = ismm->GetVSPInfo(NULL)) == NULL)
	{
		ismm->AddListener(this, this);
		ismm->EnableVSPListener();
	}

	usermessagehelper = new UserMessageHelper;

	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelInit, server, this, &AthenaPlugin::Hook_LevelInit, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, server, this, &AthenaPlugin::Hook_ServerActivate, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, GameFrame, server, this, &AthenaPlugin::Hook_GameFrame, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, server, this, &AthenaPlugin::Hook_LevelShutdown, false);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientActive, gameclients, this, &AthenaPlugin::Hook_ClientActive, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, gameclients, this, &AthenaPlugin::Hook_ClientDisconnect, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, gameclients, this, &AthenaPlugin::Hook_ClientPutInServer, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, SetCommandClient, gameclients, this, &AthenaPlugin::Hook_SetCommandClient, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientSettingsChanged, gameclients, this, &AthenaPlugin::Hook_ClientSettingsChanged, false);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientConnect, gameclients, this, &AthenaPlugin::Hook_ClientConnect, false);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientCommand, gameclients, this, &AthenaPlugin::Hook_ClientCommand, false);
	SH_ADD_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &AthenaPlugin::Hook_FireEvent, false);

	ENGINE_CALL(LogPrint)("All hooks started!\n");

	g_pCVar = icvar;
	ConVar_Register(0, &s_BaseAccessor);

	return true;
}

bool AthenaPlugin::Unload(char *error, size_t maxlen)
{
	if (gameevents)
		gameevents->RemoveListener(this);

	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelInit, server, this, &AthenaPlugin::Hook_LevelInit, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, server, this, &AthenaPlugin::Hook_ServerActivate, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, GameFrame, server, this, &AthenaPlugin::Hook_GameFrame, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, server, this, &AthenaPlugin::Hook_LevelShutdown, false);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientActive, gameclients, this, &AthenaPlugin::Hook_ClientActive, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, gameclients, this, &AthenaPlugin::Hook_ClientDisconnect, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, gameclients, this, &AthenaPlugin::Hook_ClientPutInServer, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, SetCommandClient, gameclients, this, &AthenaPlugin::Hook_SetCommandClient, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientSettingsChanged, gameclients, this, &AthenaPlugin::Hook_ClientSettingsChanged, false);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientConnect, gameclients, this, &AthenaPlugin::Hook_ClientConnect, false);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientCommand, gameclients, this, &AthenaPlugin::Hook_ClientCommand, false);
	SH_REMOVE_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &AthenaPlugin::Hook_FireEvent, false);

	if (usermessagehelper)
		delete usermessagehelper;

	return true;
}

void AthenaPlugin::OnVSPListening(IServerPluginCallbacks *iface)
{
	vsp_callbacks = iface;
}

void AthenaPlugin::Hook_ServerActivate(edict_t *pEdictList, int edictCount, int clientMax)
{
	META_LOG(g_PLAPI, "ServerActivate() called: edictCount = %d, clientMax = %d", edictCount, clientMax);
}

void AthenaPlugin::AllPluginsLoaded()
{
	/* This is where we'd do stuff that relies on the mod or other plugins
	 * being initialized (for example, cvars added and events registered).
	 */

	/* Player chat is not emitted by default, so we need to add a listener for it */
	if (gameevents)
		gameevents->AddListener(this, "player_say", true);
}

void AthenaPlugin::Hook_ClientActive(edict_t *pEntity, bool bLoadGame)
{
	META_LOG(g_PLAPI, "Hook_ClientActive(%d, %d)", IndexOfEdict(pEntity), bLoadGame);
}

void AthenaPlugin::Hook_ClientCommand(edict_t *pEntity, const CCommand &args)
{
	if (!pEntity || pEntity->IsFree())
	{
		return;
	}

	const char *cmd = args.Arg(0);
	META_CONPRINTF("%s: %s\n", GetLogTag(), cmd);
}

void AthenaPlugin::Hook_ClientSettingsChanged(edict_t *pEdict)
{
	if (playerinfomanager)
	{
		IPlayerInfo *playerinfo = playerinfomanager->GetPlayerInfo(pEdict);

		const char *name = engine->GetClientConVarValue(IndexOfEdict(pEdict), "name");

		if (playerinfo != NULL
			&& name != NULL
			&& strcmp(engine->GetPlayerNetworkIDString(pEdict), "BOT") != 0
			&& playerinfo->GetName() != NULL
			&& strcmp(name, playerinfo->GetName()) == 0)
		{
			char msg[128];
			MM_Format(msg, sizeof(msg), "Your name changed to \"%s\" (from \"%s\")\n", name, playerinfo->GetName());
			engine->ClientPrintf(pEdict, msg);
		}
	}
}

bool AthenaPlugin::Hook_ClientConnect(edict_t *pEntity,
									const char *pszName,
									const char *pszAddress,
									char *reject,
									int maxrejectlen)
{
	META_LOG(g_PLAPI, "Hook_ClientConnect(%d, \"%s\", \"%s\")", IndexOfEdict(pEntity), pszName, pszAddress);

	return true;
}

void AthenaPlugin::Hook_ClientPutInServer(edict_t *pEntity, char const *playername)
{

}

void AthenaPlugin::Hook_ClientDisconnect(edict_t *pEntity)
{
	META_LOG(g_PLAPI, "Hook_ClientDisconnect(%d)", IndexOfEdict(pEntity));
}

void AthenaPlugin::Hook_GameFrame(bool simulating)
{
	/**
	 * simulating:
	 * ***********
	 * true  | game is ticking
	 * false | game is not ticking
	 */
}

bool AthenaPlugin::Hook_LevelInit(const char *pMapName,
								char const *pMapEntities,
								char const *pOldLevel,
								char const *pLandmarkName,
								bool loadGame,
								bool background)
{
	META_LOG(g_PLAPI, "Hook_LevelInit(%s)", pMapName);

	return true;
}

void AthenaPlugin::Hook_LevelShutdown()
{
	META_LOG(g_PLAPI, "Hook_LevelShutdown()");
}

void AthenaPlugin::Hook_SetCommandClient(int index)
{
	META_LOG(g_PLAPI, "Hook_SetCommandClient(%d)", index);
}

bool AthenaPlugin::Hook_FireEvent(IGameEvent *pEvent, bool bDontBroadcast)
{
	const char *eventName = pEvent->GetName();

	if (strcmp(eventName, "player_say") == 0)
	{
		int userid = pEvent->GetInt("userid");
		const char *text = pEvent->GetString("text");
		IPlayerInfo *playerinfo = GetPlayerInfoByUserId(userid);

		if (playerinfo)
		{
		if (strcmp(text, ".ready") == 0)
		{
			usermessagehelper->PrintToChatAll(
				"[%s%s%s]: %s is now %sready.",
				PURPLE,
				GetLogTag(),
				WHITE,
				playerinfo->GetName(),
				GREEN
			);
		}
		else if (strcmp(text, ".notready") == 0)
		{
			usermessagehelper->PrintToChatAll(
				"[%s%s%s]: %s is %sNOT %sready.",
				PURPLE,
				GetLogTag(),
				WHITE,
				playerinfo->GetName(),
				RED,
				WHITE
			);
		}
		}
	}

	if (strcmp(eventName, "player_death") == 0)
	{
		int userid   = pEvent->GetInt("userid");
		int attacker = pEvent->GetInt("attacker");
		const char *weapon = pEvent->GetString("weapon");
		META_CONPRINTF("%s: Player(%d) was killed by Player(%d) with a %s.\n", GetLogTag(), userid, attacker, weapon);
	}

	RETURN_META_VALUE(MRES_IGNORED, true);
}


IPlayerInfo* AthenaPlugin::GetPlayerInfoByUserId(int userid)
{
	IPlayerInfo *playerinfo = NULL;

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		edict_t *pPlayer = (edict_t *)(gpGlobals->pEdicts + i);

		if (!pPlayer || pPlayer->IsFree())
			continue;

		playerinfo = playerinfomanager->GetPlayerInfo(pPlayer);

		if (!playerinfo)
			continue;

		if (userid == playerinfo->GetUserID())
			return playerinfo;
	}

	return NULL;
}

bool AthenaPlugin::Pause(char *error, size_t maxlen)
{
	return true;
}

bool AthenaPlugin::Unpause(char *error, size_t maxlen)
{
	return true;
}

const char *AthenaPlugin::GetLicense()
{
	return "TBD";
}

const char *AthenaPlugin::GetVersion()
{
	return "0.0.1";
}

const char *AthenaPlugin::GetDate()
{
	return __DATE__;
}

const char *AthenaPlugin::GetLogTag()
{
	return "ATHENA";
}

const char *AthenaPlugin::GetAuthor()
{
	return "Kai";
}

const char *AthenaPlugin::GetDescription()
{
	return "Match Manager";
}

const char *AthenaPlugin::GetName()
{
	return "Athena";
}

const char *AthenaPlugin::GetURL()
{
	return "TBD";
}
