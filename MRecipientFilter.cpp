#include "MRecipientFilter.h"
#include <interface.h>
#include <filesystem.h>
#include <iserverplugin.h>
#include <iplayerinfo.h>
#include <eiface.h>
#include <igameevents.h>
#include <convar.h>
#include <Color.h>

#include <shake.h>
#include <IEffects.h>
#include <IEngineSound.h>

extern IVEngineServer       *engine;
extern IPlayerInfoManager   *playerinfomanager;
extern IServerPluginHelpers *helpers;
extern CGlobalVars      *gpGlobals;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


int MRecipientFilter::GetRecipientCount() const
{
	return m_Recipients.Count();
}


int MRecipientFilter::GetRecipientIndex(int slot) const
{
	if (slot < 0 || slot >= GetRecipientCount())
		return -1;

	return m_Recipients[slot];
}


void MRecipientFilter::AddAllPlayers()
{
	m_Recipients.RemoveAll();

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		edict_t *pPlayer = (edict_t *)(gpGlobals->pEdicts + i);

		if (!pPlayer || pPlayer->IsFree())
			continue;

		// Check if this is a real person, or a bunch of message warnings
		// will pollute the console
		IPlayerInfo *playerinfo = playerinfomanager->GetPlayerInfo(pPlayer);
		if (!playerinfo || playerinfo->IsFakeClient())
			continue;

		m_Recipients.AddToTail(i);
	}
}


void MRecipientFilter::AddRecipient(int iPlayer)
{
	// Return if the recipient is already in the vector
	if (m_Recipients.Find(iPlayer) != m_Recipients.InvalidIndex())
		return;

	// Make sure the player is valid
	edict_t* pPlayer = (edict_t *)(gpGlobals->pEdicts + iPlayer);
	if (!pPlayer || pPlayer->IsFree())
		return;

	m_Recipients.AddToTail(iPlayer);
}
