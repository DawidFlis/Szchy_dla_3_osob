#pragma once

#include "RealtimeListener.h"
#include "VoiceListener.h"

class UIListener;

class BasicsApplication
{
public:
	//Launch main loop 
	//this function takes ownership of listener
	static void run(UIListener* pListener);
private:
	static ExitGames::Common::JString getStateString(State::States state);
#if defined JUST_VOICE_NO_REALTIME
	static void onKeyPressed(int key, VoiceListener& voiceListener);
#else
	static void onKeyPressed(int key, VoiceListener& voiceListener, RealtimeListener& realtimeListener);
#endif
};