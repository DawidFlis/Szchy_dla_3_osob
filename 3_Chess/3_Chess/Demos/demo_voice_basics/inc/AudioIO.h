#pragma once

#include "PhotonVoice-cpp/inc/VoiceAudio.h"
#include "PhotonVoice-cpp/inc/Logger.h"
#include <PhotonVoice-cpp/inc/AudioOutDelayControl.h>

using namespace ExitGames::Voice;

void initAudoIO(void);
void finitAudoIO(void);
ExitGames::Voice::IAudioPusher<float>* audioIOCreateMic(void);
ExitGames::Voice::IAudioOut<float>* audioIOCreateOut(const ExitGames::Voice::Logger& logger, AudioOutDelayConfig delayConfig, const ExitGames::Common::JString& logPrefix, bool debugInfo);