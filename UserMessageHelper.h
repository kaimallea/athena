#ifndef _USER_MESSAGE_HELPER_H
#define _USER_MESSAGE_HELPER_H

#include <eiface.h>
#include <cstrike15_usermessage_helpers.h>
#include "MRecipientFilter.h"

#define MAX_MESSAGE_LENGTH 256

#define WHITE       "\x01"
#define RED         "\x02"
#define PURPLE      "\x03"
#define GREEN       "\x04"
#define LIGHT_GREEN "\x05"
#define LIME_GREEN  "\x06"
#define LIGHT_RED   "\x07"
#define OFF_WHITE   "\x08"
#define YELLOW      "\x09"
#define ORANGE      "\x10"

class UserMessageHelper
{
public:
    UserMessageHelper(void) {};
    ~UserMessageHelper(void) {};

    void PrintToChatAll(const char *format, ...);
    void PrintToChat(int userid, const char *format, ...);
    void PrintToChat(edict_t *pPlayer, const char *format, ...);
};

#endif