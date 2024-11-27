#include "Common-cpp/inc/Common.h"
#include "BasicsApplication.h"
#include "UIListener.h"

using namespace ExitGames::Common;
using namespace ExitGames::LoadBalancing;

static const JString PLAYER_NAME = L"user";

void BasicsApplication::run(UIListener* pListener)
{
	VoiceListener voiceListener(pListener, PLAYER_NAME);
#if !defined JUST_VOICE_NO_REALTIME
	RealtimeListener realtimeListener(pListener, PLAYER_NAME);
#endif

	int key;
	while((key=pListener->getKeyPressed()) != 27/*ESC*/)
	{
		voiceListener.service();
#if !defined JUST_VOICE_NO_REALTIME
		realtimeListener.service();
		onKeyPressed(key, voiceListener, realtimeListener);
#else
		onKeyPressed(key, voiceListener);
#endif
		static JString lastStateStringVoice;
		if(getStateString(voiceListener.getState()).length() && getStateString(voiceListener.getState()) != lastStateStringVoice)
			pListener->writeLine(L"voiceState: " + (lastStateStringVoice=getStateString(voiceListener.getState())));
#if !defined JUST_VOICE_NO_REALTIME
		static JString lastStateStringRealtime;
		if(getStateString(realtimeListener.getState()).length() && getStateString(realtimeListener.getState()) != lastStateStringRealtime)
			pListener->writeLine(L"realtimeState: " + (lastStateStringRealtime=getStateString(realtimeListener.getState())));
#endif
		SLEEP(20);
	}
	pListener->onLibClosed();
}

JString BasicsApplication::getStateString(State::States state)
{
	switch(state)
	{
	case State::INITIALIZED:
		return L"disconnected";
	case State::CONNECTING:
		return L"connecting";
	case State::CONNECTED:
		return L"connected";
	case State::JOINING:
		return L"joining";
	case State::JOINED:
		return L"";
	case State::LEAVING:
		return L"leaving";
	case State::LEFT:
		return L"left";
	case State::DISCONNECTING:
		return L"disconnecting";
	case State::DISCONNECTED:
		return L"disconnected";
	default:
		return L"unknown state";
	}
}

#if defined JUST_VOICE_NO_REALTIME
void BasicsApplication::onKeyPressed(int key, VoiceListener& voiceListener)
#else
void BasicsApplication::onKeyPressed(int key, VoiceListener& voiceListener, RealtimeListener& realtimeListener)
#endif
{
	switch(key)
	{
	case 'c':
	{
		voiceListener.connect();
#if !defined JUST_VOICE_NO_REALTIME
		realtimeListener.connect();
#endif
	}
	break;
	case 'd':
		voiceListener.disconnect();
#if !defined JUST_VOICE_NO_REALTIME
		realtimeListener.disconnect();
#endif
		break;
	case 'e':
		voiceListener.toggleEchoMode();
		break;
#if !defined JUST_VOICE_NO_REALTIME
	case 's':
		realtimeListener.toggleSendData();
#endif
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
		voiceListener.setTargetPlayer(key - '0');
	break;
	case '6':
		voiceListener.setTargetPlayer(0);
	break;
	case '7':
	case '8':
	case '9':
	case '0':
		voiceListener.setInterestGroup(key - '0');
	break;
	}
}