#include "UserMessageHelper.h"

extern IVEngineServer *engine;

void UserMessageHelper::PrintToChatAll(const char *format, ...)
{
    char message[MAX_MESSAGE_LENGTH];
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    CCSUsrMsg_SayText *pMsg = (CCSUsrMsg_SayText *)g_Cstrike15UsermessageHelpers.GetPrototype(CS_UM_SayText)->New();
    MRecipientFilter f;
    f.AddAllPlayers();
    pMsg->set_text(message);
    pMsg->set_chat(true);
    engine->SendUserMessage(f, CS_UM_SayText, *pMsg);
    delete pMsg;
}


void UserMessageHelper::PrintToChat(int userid, const char *format, ...)
{
    char message[MAX_MESSAGE_LENGTH];

    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    CCSUsrMsg_SayText *pMsg = (CCSUsrMsg_SayText *)g_Cstrike15UsermessageHelpers.GetPrototype(CS_UM_SayText)->New();
    MRecipientFilter f;
    f.AddRecipient(userid);
    pMsg->set_text(message);
    pMsg->set_chat(true);
    engine->SendUserMessage(f, CS_UM_SayText, *pMsg);
    delete pMsg;
}


void UserMessageHelper::PrintToChat(edict_t *pPlayer, const char *format, ...)
{
    if (!pPlayer || pPlayer->IsFree())
        return;

    int userid = engine->GetPlayerUserId(pPlayer);
    if (userid == -1)
        return;

    char message[MAX_MESSAGE_LENGTH];

    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    CCSUsrMsg_SayText *pMsg = (CCSUsrMsg_SayText *)g_Cstrike15UsermessageHelpers.GetPrototype(CS_UM_SayText)->New();
    MRecipientFilter f;
    f.AddRecipient(userid);
    pMsg->set_text(message);
    pMsg->set_chat(true);
    engine->SendUserMessage(f, CS_UM_SayText, *pMsg);
    delete pMsg;
}
