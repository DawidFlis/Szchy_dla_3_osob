#include "RealtimeListener.h"

#if !defined JUST_VOICE_NO_REALTIME

using namespace ExitGames::Common;
using namespace ExitGames::Common::Helpers;
using namespace ExitGames::Common::MemoryManagement;
using namespace ExitGames::LoadBalancing;
using namespace ExitGames::Photon;

static const JString appIDRealtime = L"<no-app-id>"; // set your Realtime app id here
static const JString appVersionRealtime = L"1.0";
static JString gameNameRealtime = L"VoiceBasicsRoom";
static const int SENT_INTERVAL = 1000;

RealtimeListener::RealtimeListener(UIListener* pOutputListener, const JString& playerName)
	: mpOutputListener(pOutputListener)
	, mLoadBalancingClient(*this, appIDRealtime, appVersionRealtime, ClientConstructOptions().setAutoLobbyStats(true).setRegionSelectionMode(RegionSelectionMode::SELECT))
	, mState(State::INITIALIZED)
	, mPlayerName(playerName)
	, mSendCount(0)
	, mReceiveCount(0)
{
	mLoadBalancingClient.setDebugOutputLevel(DEBUG_RELEASE(DebugLevel::INFO, DebugLevel::WARNINGS)); // that instance of LoadBalancingClient and its implementation details
}

void RealtimeListener::service(void)
{
	switch(mState)
	{
	case State::INITIALIZED:
		connect();
		mState = State::CONNECTING;
		break;
	case State::CONNECTED:
		mLoadBalancingClient.opJoinRandomOrCreateRoom(gameNameRealtime);
		mState = State::JOINING;
		break;
	case State::JOINED:
		if(mSendData)
		{
			static JTime timeLastSent = GETTIMEMS();
			if(GETTIMEMS() - timeLastSent > SENT_INTERVAL)
			{
				sendData();
				timeLastSent = GETTIMEMS();
			}
		}
		break;
	case State::LEFT:
		mLoadBalancingClient.disconnect();
		mState = State::DISCONNECTING;
		break;
	case State::DISCONNECTED:
		break;
	default:
		break;
	}
	mLoadBalancingClient.service();
}

void RealtimeListener::connect(void)
{
	ConnectOptions options;
	options.setAuthenticationValues(AuthenticationValues().setUserID(JString() + GETTIMEMS())).setUsername(mPlayerName + GETTIMEMS());
	mLoadBalancingClient.connect(options);
}

void RealtimeListener::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

void RealtimeListener::toggleSendData(void)
{
	mSendData = !mSendData;
}

const State::States& RealtimeListener::getState(void)
{
	return mState;
}

void RealtimeListener::sendData(void)
{
	static int64 count = 0;
	mLoadBalancingClient.opRaiseEvent(false, count, 0);
	mpOutputListener->write(ExitGames::Common::JString(L"S") + ++count + L" ");
}

void RealtimeListener::debugReturn(int /*debugLevel*/, const JString& string)
{
	mpOutputListener->writeLine(string);
}

void RealtimeListener::connectionErrorReturn(int errorCode)
{
	EGLOG(DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(JString(L"received connection error ")+errorCode);
	mState = State::DISCONNECTED;
}

void RealtimeListener::clientErrorReturn(int errorCode)
{
	EGLOG(DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(JString(L"received error ")+errorCode+L" from client");
}

void RealtimeListener::warningReturn(int warningCode)
{
	EGLOG(DebugLevel::WARNINGS, L"code: %d", warningCode);
	mpOutputListener->writeLine(JString(L"received warning ")+warningCode+L" from client");
}

void RealtimeListener::serverErrorReturn(int errorCode)
{
	EGLOG(DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(JString(L"received error ")+errorCode+" from server");
}

void RealtimeListener::joinRoomEventAction(int playerNr, const JVector<int>& playernrs, const Player& player)
{
	EGLOG(DebugLevel::INFO, L"%ls joined the game", player.getName().cstr());
	mpOutputListener->writeLine(L"");
	mpOutputListener->writeLine(JString(L"player ")+playerNr+L" "+player.getName()+L" has joined the game");
	if(playerNr == mLoadBalancingClient.getLocalPlayer().getNumber())
		mpOutputListener->writeLine(L"\n\n\n                         Press 's' to toggle sending of Realtime data\n\n");
}

void RealtimeListener::leaveRoomEventAction(int playerNr, bool isInactive)
{
	EGLOG(DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"");
	mpOutputListener->writeLine(JString(L"player ")+playerNr+L" has left the game");
}

void RealtimeListener::customEventAction(int playerNr, nByte eventCode, const Object& eventContentObj)
{
	// you do not receive your own events, unless you specify yourself as one of the receivers explicitly, so you must start 2 clients, to receive the events, which you have sent, as sendEvent() uses the default receivers of opRaiseEvent() (all players in same room like the sender, except the sender itself)
	EGLOG(ExitGames::Common::DebugLevel::ALL, L"");
	mpOutputListener->write(ExitGames::Common::JString(L"R") + ExitGames::Common::ValueObject<long long>(eventContentObj).getDataCopy() + "<-p" + playerNr + L" ");
}

void RealtimeListener::connectReturn(int errorCode, const JString& errorString, const JString& region, const JString& cluster)
{
	EGLOG(DebugLevel::INFO, L"connected to cluster "+cluster+L" of region "+region);
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mState = State::DISCONNECTING;
		return;
	}
	mpOutputListener->writeLine(L"connected to cluster "+cluster);
	mState = State::CONNECTED;
}

void RealtimeListener::disconnectReturn(void)
{
	EGLOG(DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"disconnected");
	mState = State::DISCONNECTED;
}

void RealtimeListener::createRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opCreateRoom() failed: "+errorString);
		mState = State::CONNECTED;
		return;
	}

	EGLOG(DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room "+mLoadBalancingClient.getCurrentlyJoinedRoom().getName()+" has been created");
	mState = State::JOINED;
}

void RealtimeListener::joinOrCreateRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinOrCreateRoom() failed: "+errorString);
		mState = State::CONNECTED;
		return;
	}

	EGLOG(DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room "+mLoadBalancingClient.getCurrentlyJoinedRoom().getName()+" has been entered");
	mState = State::JOINED;
}

void RealtimeListener::joinRandomOrCreateRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinRandomOrCreateRoom() failed: "+errorString);
		mState = State::CONNECTED;
		return;
	}

	EGLOG(DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room "+mLoadBalancingClient.getCurrentlyJoinedRoom().getName()+" has been entered");
	mState = State::JOINED;
}

void RealtimeListener::joinRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinRoom() failed: "+errorString);
		mState = State::CONNECTED;
		return;
	}
	EGLOG(DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room "+mLoadBalancingClient.getCurrentlyJoinedRoom().getName()+" has been successfully joined");

	mState = State::JOINED;
}

void RealtimeListener::joinRandomRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinRandomRoom() failed: "+errorString);
		mState = State::CONNECTED;
		return;
	}

	EGLOG(DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room "+mLoadBalancingClient.getCurrentlyJoinedRoom().getName()+" has been successfully joined");
	mState = State::JOINED;
}

void RealtimeListener::leaveRoomReturn(int errorCode, const JString& errorString)
{
	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opLeaveRoom() failed: "+errorString);
		mState = State::DISCONNECTING;
		return;
	}
	mState = State::LEFT;
	mpOutputListener->writeLine(L"room has been successfully left");
}

void RealtimeListener::joinLobbyReturn(void)
{
	EGLOG(DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"joined lobby");
}

void RealtimeListener::leaveLobbyReturn(void)
{
	EGLOG(DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"left lobby");
}

void RealtimeListener::onAvailableRegions(const JVector<JString>& /*availableRegions*/, const JVector<JString>& /*availableRegionServers*/)
{
	mLoadBalancingClient.selectRegion(L"eu");
}
#endif