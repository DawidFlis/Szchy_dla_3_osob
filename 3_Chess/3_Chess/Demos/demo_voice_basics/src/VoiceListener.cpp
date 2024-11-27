#include "PhotonVoice-cpp/inc/AudioGen.h"
#include "PhotonVoice-cpp/inc/OpusCodec.h"
#include "PhotonVoice-cpp/inc/Transport/LoadBalancingTransport.h"
#include "AudioIO.h"
#include "DelayProcessor.h"
#include "VoiceListener.h"

using namespace ExitGames::Common;
using namespace ExitGames::Common::Helpers;
using namespace ExitGames::Common::MemoryManagement;
using namespace ExitGames::LoadBalancing;
using namespace ExitGames::Photon;
using namespace ExitGames::Voice;
using namespace ExitGames::Voice::AudioUtil;


static const bool DIRECT = false; // send audio peer to peer
static const bool CLIENT_ECHO = false; // for each remote voice, create local voice and broadcast incoming stream through it, can be useful for local testing when sending audio peer to peer; you need to run at least two clients on the same machine, of which this option needs to be turned on for exactly one client and turned off for all other clients in the same room, for this mode to work - turning this mode on on more than one of the clients that are inside the same room is not supported and leads to undefined behavior!
static const bool DEBUG_ECHO_MODE_INIT = false; // initial state of local voice debug echo mode (streaming back to client via server)
static const JString appIDVoice = L"<no-app-id>"; // set your Voice app id here
static const JString appVersionVoice = L"1.0";
static const JString gameNameVoice = L"VoiceBasicsRoom_voice_";



Echo::Echo(const ExitGames::Voice::Logger& logger, LocalVoiceAudio<float>* localVoice, const VoiceInfo& voiceInfo)
	: mpLocalVoice(localVoice)
	, mLogger(logger)
	, mVoiceInfo(voiceInfo)
{
	EGLOG(DebugLevel::INFO, L"Creating Client Echo for " + mVoiceInfo.toString());
}

Echo::~Echo(void)
{
	EGLOG(DebugLevel::INFO, L"Destroying Client Echo for " + mVoiceInfo.toString());
	mpLocalVoice->removeSelf();
}

void Echo::frameDataCallback(const Buffer<float>& frame)
{
	mpLocalVoice->pushDataAsync(frame);
}

static void echoFrameDataCallback(void* opaque, const Buffer<float>& frame)
{
	static_cast<Echo*>(opaque)->frameDataCallback(frame);
}

static void echoRemoteVoiceRemoveCallback(void* opaque)
{
	delete(static_cast<Echo*>(opaque));
}

static void frameDataCallback(void* opaque, const Buffer<float>& frame)
{
	IAudioOut<float>* audioOut = static_cast<IAudioOut<float>*>(opaque);
	audioOut->push(frame);
}



RemoteVoiceData::RemoteVoiceData(IAudioOut<float>* player, JVector<RemoteVoiceData*>* container)
	: player(player)
	, container(container)
{
}

RemoteVoiceData::~RemoteVoiceData(void)
{
	delete player;
}



template <typename T>
void del(const T* e)
{
	delete e;
}




void VoiceListener::remoteVoiceRemoveCallback(void* opaque)
{
	RemoteVoiceData* d = static_cast<RemoteVoiceData*>(opaque);
	while(d->container->removeElement(d));
	delete d;
}

void VoiceListener::remoteVoiceInfoCallback(void* opaque, int channelId, int playerId, nByte voiceId, const VoiceInfo& voiceInfo, RemoteVoiceOptions& options)
{
	static_cast<VoiceListener*>(opaque)->remoteVoiceInfoCallback(channelId, playerId, voiceId, voiceInfo, options);
}

void VoiceListener::remoteVoiceInfoCallback(int channelId, int playerId, nByte voiceId, const VoiceInfo& voiceInfo, RemoteVoiceOptions& options)
{
	if (voiceInfo.getCodec() == Codec::AUDIO_OPUS)
	{
		if (CLIENT_ECHO)
		{
			LocalVoiceAudio<float>* pV = mpVoiceClient->createLocalVoiceAudio<float>(voiceInfo, AudioDesc(voiceInfo.getSamplingRate(), voiceInfo.getChannels()));
			Echo* pE = new Echo(mLogger, pV, voiceInfo);
			options.setOutput(pE, echoFrameDataCallback);
			options.setRemoteVoiceRemoveAction(pE, echoRemoteVoiceRemoveCallback);
			pV->getVoiceDetector()->setOn(false);
		}
		else
		{
			AudioOutDelayConfig delayConfig(200, 500, 1000, 5);
			IAudioOut<float>* pPlayer = audioIOCreateOut(mLogger, delayConfig, JString(L"Audio Player p#") + playerId + L", v#" + voiceId, true);

			pPlayer->start(voiceInfo.getSamplingRate(), voiceInfo.getChannels(), voiceInfo.getFrameDurationSamples());
			RemoteVoiceData* pRVD = new RemoteVoiceData(pPlayer, &mAudioPlayers);
			mAudioPlayers.addElement(pRVD);
			options.setRemoteVoiceRemoveAction(pRVD, remoteVoiceRemoveCallback);

			// either setOutput or setDecoder
			options.setOutput(pPlayer, frameDataCallback);
			//options.setDecoder(SharedPointer<IDecoder>(new Opus::Decoder<float>(pPlayer, frameDataCallback, *mpTransport), del<IDecoder>));
		}
	}
}



VoiceListener::VoiceListener(UIListener* pOutputListener, const JString& playerName)
	: mpOutputListener(pOutputListener)
	, mLoadBalancingClient(*this, appIDVoice, appVersionVoice, ClientConstructOptions().setAutoLobbyStats(true).setRegionSelectionMode(RegionSelectionMode::SELECT))
	, mState(State::INITIALIZED)
	, mPlayerName(playerName)
	, mVoicesCreated(false)
{
	mLogger.setListener(*this);
	//mLogger.setDebugOutputLevel(DebugLevel::ALL);
	mLoadBalancingClient.setDebugOutputLevel(DEBUG_RELEASE(DebugLevel::INFO, DebugLevel::WARNINGS)); // that instance of LoadBalancingClient and its implementation details

	mpTransport = new LoadBalancingTransport(mLogger, mLoadBalancingClient,  DIRECT);
	mpVoiceClient = new VoiceClient(mLogger, mpTransport);
	mpVoiceClient->setOnRemoteVoiceInfoAction(this, VoiceListener::remoteVoiceInfoCallback);

	Base::setListener(this);
	Base::setDebugOutputLevel(DEBUG_RELEASE(DebugLevel::INFO, DebugLevel::WARNINGS)); // all classes that inherit from Base

	initAudoIO();

	createLocalVoices();
}

VoiceListener::~VoiceListener(void)
{
	for(unsigned int i=0; i<mAudioSources.getSize(); ++i)
		delete mAudioSources[i];
	mAudioSources.removeAllElements();
	for(unsigned int i=0; i<mLocalVoices.getSize(); ++i)
		mLocalVoices[i]->removeSelf();
	finitAudoIO();

	delete mpVoiceClient;
	delete mpTransport;
}

void VoiceListener::service(void)
{
	switch(mState)
	{
	case State::INITIALIZED:
		connect();
		mState = State::CONNECTING;
		break;
	case State::CONNECTED:
		{
			RoomOptions opt;
			if(DIRECT)
				opt.setDirectMode(DirectMode::ALL_TO_ALL);
			mLoadBalancingClient.opJoinRandomOrCreateRoom(gameNameVoice, opt);
			mState = State::JOINING;
		}
		break;
	case State::JOINED:
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

	mpTransport->service();
	mpVoiceClient->service();
	for(unsigned int i=0; i<mAudioPlayers.getSize(); ++i)
		mAudioPlayers[i]->player->service();
}

void VoiceListener::connect(void)
{
	ConnectOptions options;
	options.setAuthenticationValues(AuthenticationValues().setUserID(JString() + GETTIMEMS())).setUsername(mPlayerName + GETTIMEMS());
	mLoadBalancingClient.connect(options);
}

void VoiceListener::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

void VoiceListener::toggleEchoMode(void)
{
	for(unsigned int i=0; i<mLocalVoices.getSize(); ++i)
	{
		mLocalVoices[i]->setDebugEchoMode(!mLocalVoices[i]->getDebugEchoMode());
		mpOutputListener->writeLine(mLocalVoices[i]->getName() + L" echo: " + (mLocalVoices[i]->getDebugEchoMode()?L"ON":L"OFF"));
	}
}

void VoiceListener::setInterestGroup(nByte v)
{
	for (unsigned int i = 0; i < mLocalVoices.getSize(); ++i)
	{
		mLocalVoices[i]->setInterestGroup(v);
		mpOutputListener->writeLine(mLocalVoices[i]->getName() + L" interest group: " + (mLocalVoices[i]->getInterestGroup()));
	}
	JVector<nByte> empty;
	JVector<nByte> groups(&v, 1);
	mLoadBalancingClient.opChangeGroups(&empty, &groups);
}

void VoiceListener::setTargetPlayer(int v)
{
	for (unsigned int i = 0; i < mLocalVoices.getSize(); ++i)
	{
		if (v == 0)
			mLocalVoices[i]->setTargetPlayersDisabled();
		else
			mLocalVoices[i]->setTargetPlayers(&v, 1);
	}
}

const State::States& VoiceListener::getState(void)
{
	return mState;
}

void VoiceListener::debugReturn(int /*debugLevel*/, const JString& string)
{
	mpOutputListener->writeLine(string);
}

void VoiceListener::connectionErrorReturn(int errorCode)
{
	EGLOG(DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(JString(L"received connection error ") + errorCode);
	mState = State::DISCONNECTED;
}

void VoiceListener::clientErrorReturn(int errorCode)
{
	EGLOG(DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(JString(L"received error ") + errorCode + L" from client");
}

void VoiceListener::warningReturn(int warningCode)
{
	EGLOG(DebugLevel::WARNINGS, L"code: %d", warningCode);
	mpOutputListener->writeLine(JString(L"received warning ") + warningCode + L" from client");
}

void VoiceListener::serverErrorReturn(int errorCode)
{
	EGLOG(DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(JString(L"received error ") + errorCode + " from server");
}

void VoiceListener::joinRoomEventAction(int playerNr, const JVector<int>& playernrs, const Player& player)
{
	LoadBalancingListener::joinRoomEventAction(playerNr, playernrs, player);

	EGLOG(DebugLevel::INFO, L"%ls joined the game", player.getName().cstr());
	mpOutputListener->writeLine(L"\n" + JString(L"player ") + playerNr + L" " + player.getName() + L" has joined the game");

//	createLocalVoices();

	if (playerNr == mLoadBalancingClient.getLocalPlayer().getNumber())
		mpOutputListener->writeLine(L"\n\n\n                         Press 'e' to toggle debug echo mode\n\n");
}

void VoiceListener::createLocalVoices()
{
	if(!mVoicesCreated)
	{
		/*
		ToneAudioPusher<float>* pAudioSource0 = new ToneAudioPusher<float>(220, 22, 10000, 1);
		VoiceInfo i0=VoiceInfo::createAudioOpus(48000, pAudioSource0->getChannels(), 10000, 30000);
		LocalVoice* v0 = mpVoiceClient->createLocalVoiceAudioFromSource(i0, pAudioSource0);
		v0->setDebugEchoMode(DEBUG_ECHO_MODE_INIT);
		mAudioSources.addElement(pAudioSource0);
		mLocalVoices.addElement(v0);
		*/
		/*
		ToneAudioPusher<float>* pAudioSource1 = new ToneAudioPusher<float>(220 * 3 /2, 22, 10000, 2);
		VoiceInfo i1=VoiceInfo::createAudioOpus(24000, pAudioSource1->getChannels(), 60000, 30000);
		VoiceCreateOptions opt;
		opt.setFEC(3);
		opt.setFragment(true);
		//mpVoiceClient->setRemoteVoiceDelayFrames(Codec::AUDIO_OPUS, 6);
		LocalVoice* v1 = mpVoiceClient->createLocalVoiceAudioFromSource(i1, pAudioSource1, 0, opt );
		v1->setDebugEchoMode(DEBUG_ECHO_MODE_INIT);
		mAudioSources.addElement(pAudioSource1);
		mLocalVoices.addElement(v1);
		*/

		IAudioPusher<float>* pAudioSource2 = audioIOCreateMic();
		VoiceInfo i2 = VoiceInfo::createAudioOpus(48000, pAudioSource2->getChannels(), 20000, 30000);
		// default or user's decoder
		LocalVoiceAudio<float>* v2 = mpVoiceClient->createLocalVoiceAudioFromSource(i2, pAudioSource2, 0);
		//LocalVoiceAudio<float>* v2 = mpVoiceClient->createLocalVoiceAudioFromSource(i2, pAudioSource2, 0, VoiceCreateOptions().setEncoder(SharedPointer<IEncoder>(new Opus::Encoder<float>(i2, *mpTransport), del<IEncoder>)));
		v2->setDebugEchoMode(DEBUG_ECHO_MODE_INIT);
		v2->getVoiceDetector()->setOn(false);
		mLocalVoices.addElement(v2);

		// processor with allocation default or provided by user
		v2->addPostProcessor(SharedPointer<IProcessor<float>>(new DelayProcessor<float>(8000, 0.2f), del<IProcessor<float>>));
		mAudioSources.addElement(pAudioSource2);


		mVoicesCreated = true;
	}
}

void VoiceListener::leaveRoomEventAction(int playerNr, bool isInactive)
{
	LoadBalancingListener::leaveRoomEventAction(playerNr, isInactive);

	EGLOG(DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"");
	mpOutputListener->writeLine(JString(L"player ") + playerNr + L" has left the game");
}

void VoiceListener::customEventAction(int playerNr, nByte eventCode, const Object& eventContentObj)
{
	LoadBalancingListener::customEventAction(playerNr, eventCode, eventContentObj);
}

void VoiceListener::connectReturn(int errorCode, const JString& errorString, const JString& region, const JString& cluster)
{
	EGLOG(DebugLevel::INFO, L"connected to cluster " + cluster + L" of region " + region);
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mState = State::DISCONNECTING;
		return;
	}
	mpOutputListener->writeLine(L"connected to cluster " + cluster);
	mState = State::CONNECTED;
}

void VoiceListener::disconnectReturn(void)
{
	LoadBalancingListener::disconnectReturn();

	EGLOG(DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"disconnected");
	mState = State::DISCONNECTED;
}

void VoiceListener::createRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	LoadBalancingListener::createRoomReturn(localPlayerNr, gameProperties, playerProperties, errorCode, errorString);

	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opCreateRoom() failed: " + errorString);
		mState = State::CONNECTED;
		return;
	}

	EGLOG(DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been created");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mState = State::JOINED;
}

void VoiceListener::joinOrCreateRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	LoadBalancingListener::joinOrCreateRoomReturn(localPlayerNr, gameProperties, playerProperties, errorCode, errorString);

	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinOrCreateRoom() failed: " + errorString);
		mState = State::CONNECTED;
		return;
	}

	EGLOG(DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been entered");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mState = State::JOINED;
}

void VoiceListener::joinRandomOrCreateRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	LoadBalancingListener::joinRandomOrCreateRoomReturn(localPlayerNr, gameProperties, playerProperties, errorCode, errorString);

	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinRandomOrCreateRoom() failed: " + errorString);
		mState = State::CONNECTED;
		return;
	}

	EGLOG(DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been entered");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mState = State::JOINED;
}

void VoiceListener::joinRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	LoadBalancingListener::joinRoomReturn(localPlayerNr, gameProperties, playerProperties, errorCode, errorString);

	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinRoom() failed: " + errorString);
		mState = State::CONNECTED;
		return;
	}
	EGLOG(DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been successfully joined");
	mpOutputListener->writeLine(L"regularly sending dummy events now");

	mState = State::JOINED;
}

void VoiceListener::joinRandomRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	LoadBalancingListener::joinRandomRoomReturn(localPlayerNr, gameProperties, playerProperties, errorCode, errorString);

	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinRandomRoom() failed: " + errorString);
		mState = State::CONNECTED;
		return;
	}

	EGLOG(DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been successfully joined");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mState = State::JOINED;
}

void VoiceListener::leaveRoomReturn(int errorCode, const JString& errorString)
{
	LoadBalancingListener::leaveRoomReturn(errorCode, errorString);

	EGLOG(DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opLeaveRoom() failed: " + errorString);
		mState = State::DISCONNECTING;
		return;
	}
	mState = State::LEFT;
	mpOutputListener->writeLine(L"room has been successfully left");
}

void VoiceListener::joinLobbyReturn(void)
{
	EGLOG(DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"joined lobby");
}

void VoiceListener::leaveLobbyReturn(void)
{
	EGLOG(DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"left lobby");
}

void VoiceListener::onAvailableRegions(const JVector<JString>& /*availableRegions*/, const JVector<JString>& /*availableRegionServers*/)
{
	mLoadBalancingClient.selectRegion(L"eu");
}