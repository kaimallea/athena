/**
 * vim: set ts=4 sw=4 tw=99 noet:
g*/

#include "event_handlers.h"

int EventHandler::handler(IGameEvent *pEntity) 
{
	// List of events used: https://wiki.alliedmods.net/Counter-Strike:_Global_Offensive_Events
	const char *name = pEvent->GetName();
	switch(name) {
		case "player_death":
		case "player_hurt":
		case "item_purchase":
		case "bomb_beginplant":
		case "bomb_abortplant":
		case "bomb_planted":
		case "bomb_defused":
		case "bomb_exploded":
		case "bomb_dropped":
		case "bomb_pickup":
		case "defuser_dropped":
		case "defuser_pickup":
		case "announce_phase_end":
		case "cs_intermission":
		case "bomb_begindefuse":
		case "bomb_abortdefuse":
		case "vip_escaped":
		case "vip_killed":
		case "player_radio":
		case "bomb_beep":
		case "weapon_fire":
		case "weapon_fire_on_empty":
		case "weapon_outofammo":
		case "weapon_reload":
		case "weapon_zoom":
		case "silencer_detach":
		case "inspect_weapon":
		case "weapon_zoom_rifle":
		case "player_spawned":
		case "item_pickup":
		case "ammo_pickup":
		case "item_equip":
		case "enter_buyzone":
		case "exit_buyzone":
		case "buytime_ended":
		case "enter_bombzone":
		case "exit_bombzone":
		case "silencer_off":
		case "silencer_on":
		case "buymenu_open":
		case "buymenu_close":
		case "round_prestart":
		case "round_poststart":
		case "round_start":
		case "round_end":
		case "grenade_bounce":
		case "hegrenade_detonate":
		case "flashbang_detonate":
		case "smokegrenade_detonate":
		case "smokegrenade_expired":
		case "molotov_detonate":
		case "decoy_detonate":
		case "decoy_started":
		case "inferno_startburn":
		case "inferno_expire":
		case "inferno_extinguish":
		case "decoy_firing":
		case "bullet_impact":
		case "player_footstep":
		case "player_jump":
		case "player_blind":
		case "player_falldamage":
		case "door_moving":
		case "round_freeze_end":
		case "mb_input_lock_success":
		case "mb_input_lock_cancel":
		case "spec_target_updated":
		case "spec_mode_updated":
		case "hltv_changed_mode":
		case "cs_game_disconnected":
		case "cs_win_panel_round":
		case "cs_win_panel_match":
		case "cs_match_end_restart":
		case "cs_pre_restart":
		case "show_freezepanel":
		case "hide_freezepanel":
		case "freezecam_started":
		case "player_avenged_teammate":
		case "achievement_earned":
		case "achievement_earned_local":
		case "item_found":
		case "item_gifted":
		case "match_end_conditions":
		case "round_mvp":
		case "player_decal":
		case "teamplay_round_start":
		case "client_disconnect":
		case "write_profile_data":
		case "trial_time_expired":
		case "player_reset_vote":
		case "enable_restart_voting":
		case "sfuievent":
		case "start_vote":
		case "player_given_c4":
		case "gg_reset_round_start_sounds":
		case "tr_player_flashbanged":
		case "tr_highlight_ammo":
		case "tr_mark_complete":
		case "tr_mark_best_time":
		case "tr_exit_hint_trigger":
		case "bot_takeover":
		case "tr_show_finish_msgbox":
		case "tr_show_exit_msgbox":
		case "reset_player_controls":
		case "jointeam_failed":
		case "teamchange_pending":
		case "material_default_complete":
		case "cs_prev_next_spectator":
		case "cs_handle_ime_event":
		case "nextlevel_changed":
		case "tournament_reward":
		default:
			return 1;
	}
	return 0;
}

void EventHandler:player_death(IGameEvent *pEntity)
{
	int userid   = pEvent->GetInt("userid");
	int attacker = pEvent->GetInt("attacker");
	int assister = pEvent->GetInt("assister");
	const char *weapon   = pEvent->GetString("weapon");
	const char *weaponid = pEvent->GetString("weaponid");
	const char *weapon_fauxitemid = pEvent->GetString("weapon_fauxitemid");
	const char *weapon_originalowner_xuid = pEvent->GetString("weapon_originalowner_xuid");
	bool headshot = pEvent->GetBool("headshot");
	const char *dominated = pEvent->GetString("dominated");


}
