#include "LoadBalancing-cpp/inc/Client.h"
#include "PhotonVoice-cpp/inc/RemoteVoiceOptions.h"
#include "PhotonVoice-cpp/inc/Transport/LoadBalancingListener.h"
#include "PhotonVoice-cpp/inc/VoiceAudio.h"
#include "PhotonVoice-cpp/inc/RemoteVoiceOptions.h"
#include "PhotonVoice-cpp/inc/Transport/LoadBalancingListener.h"
#include "PhotonVoice-cpp/inc/VoiceAudio.h"
#include "State.h"
#include "UIListener.h"



// A sample of audio source which does not need to implement IAudioPusher. It calls push() iself instead.
class Echo
{
public:
	Echo(const ExitGames::Voice::Logger& logger, ExitGames::Voice::LocalVoiceAudio<float>* localVoice, const ExitGames::Voice::VoiceInfo& voiceInfo);
	~Echo(void);
	void frameDataCallback(const ExitGames::Voice::Buffer<float>& frame);
private:
	ExitGames::Voice::LocalVoiceAudio<float>* mpLocalVoice;
	const ExitGames::Voice::Logger& mLogger;
	ExitGames::Voice::VoiceInfo mVoiceInfo;
};



struct RemoteVoiceData : public ExitGames::Common::ToStringDefaultImplementation
{
public:
	RemoteVoiceData(ExitGames::Voice::IAudioOut<float>* player, ExitGames::Common::JVector<RemoteVoiceData*>* container);
	~RemoteVoiceData(void);
	ExitGames::Voice::IAudioOut<float>* player;
	ExitGames::Common::JVector<RemoteVoiceData*>* container;
};



class VoiceListener : public ExitGames::Voice::LoadBalancingListener
{
public:
	VoiceListener(UIListener* pOutputListener, const ExitGames::Common::JString& playerName);
	~VoiceListener(void);

	void service(void);
	void connect(void);
	void disconnect(void);
	void toggleEchoMode(void);
	void setInterestGroup(nByte);
	void setTargetPlayer(int);

	const State::States& getState(void);
private:
	static void remoteVoiceRemoveCallback(void* opaque);
	static void remoteVoiceInfoCallback(void* opaque, int channelId, int playerId, nByte voiceId, const ExitGames::Voice::VoiceInfo& voiceInfo, ExitGames::Voice::RemoteVoiceOptions& options);
	void remoteVoiceInfoCallback(int channelId, int playerId, nByte voiceId, const ExitGames::Voice::VoiceInfo& voiceInfo, ExitGames::Voice::RemoteVoiceOptions& options);
	void createLocalVoices();

	// receive and print out debug out here
	virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string);

	// implement your error-handling here
	virtual void connectionErrorReturn(int errorCode);
	virtual void clientErrorReturn(int errorCode);
	virtual void warningReturn(int warningCode);
	virtual void serverErrorReturn(int errorCode);

	// events, triggered by certain operations of all players in the same room
	virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player);
	virtual void leaveRoomEventAction(int playerNr, bool isInactive);
	virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent);

	// callbacks for operations on PhotonLoadBalancing server
	virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster);
	virtual void disconnectReturn(void);
	virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRandomOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinLobbyReturn(void);
	virtual void leaveLobbyReturn(void);
	virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers);

	UIListener* mpOutputListener;
	ExitGames::Voice::Logger mLogger;
	ExitGames::LoadBalancing::Client mLoadBalancingClient;
	State::States mState;
	const ExitGames::Common::JString& mPlayerName;

	bool mVoicesCreated;
	ExitGames::Common::JVector<ExitGames::Voice::LocalVoice*> mLocalVoices;
	ExitGames::Common::JVector<ExitGames::Voice::IAudioPusher<float>*> mAudioSources;
	ExitGames::Common::JVector<RemoteVoiceData*> mAudioPlayers;
};