#include "PhotonVoice-cpp/inc/AudioUtil.h"
#include "PhotonVoice-cpp/inc/AudioOutDelayControl.h"
#include "PhotonVoice-cpp/inc/VoiceAudio.h"
#include "PhotonVoice-cpp/inc/Buffer.h"
#include "../portaudio/include/portaudio.h"

#define SAMPLE_RATE (static_cast<int64>(44100)) // to avoid int overflow
#define FRAMES_PER_BUFFER (128)
#define INPUT_CHANNELS 1
#define OUTPUT_CHANNELS 1

using namespace ExitGames::Common;
using namespace ExitGames::Common::Helpers;
using namespace ExitGames::Common::MemoryManagement;
using namespace ExitGames::Voice;

template<typename T>
static void* memAdd(void* dst, const void* src, size_t cnt);

class IAudioIOProcessor : virtual public ToString
{
public:
	// returns true if output buffer has been written
	virtual bool audioProcessing(const void* input, void* output, unsigned long sampleCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData, bool outInited) = 0;
	virtual ~IAudioIOProcessor(void) {}
};

static Lockable<JVector<IAudioIOProcessor*> > gProcessors;

static void addProcessor(IAudioIOProcessor* p);
static void removeProcessor(IAudioIOProcessor* p);

// frame (frame sample rate, frame channels) -> push() -> ring in (out sampling rate, out channels) -> audioProcessing() -> ring out (out sampling rate, out channels)
class AudioOut : public AudioOutDelayControl<float>, public IAudioIOProcessor
{
public:
	using ToString::toString;

	AudioOut(const ExitGames::Voice::Logger& logger, AudioOutDelayConfig delayConfig, const ExitGames::Common::JString& logPrefix, bool debugInfo);
	virtual ~AudioOut(void);

	virtual int64 outGetPos() const;
	virtual void outCreate(int frequency, int channels, int bufferSamples);
	virtual void outStart();
	virtual void outWrite(Buffer<float> data, int offsetSamples);

	virtual bool audioProcessing(const void* pInput, void* pOutput, unsigned long outSamples, const PaStreamCallbackTimeInfo* pTimeInfo, PaStreamCallbackFlags statusFlags, void* pUserData, bool outInited);
	virtual JString& toString(JString& retStr, bool withTypes=false) const;
private:
	size_t mReadPosOutSamples;     // absolute read position in samples set by reader consuming audio stream with SAMPLE_RATE and OUTPUT_CHANNELS; outGetPos() converts it to the position in the stream with 'frequency' and 'channels' specified in outCtreate() call
	Buffer<float> mRingBuf;
	int mRingBufSamples;
	mutable std::mutex mLock;

	bool mStarted;
	bool mPlaying;
};

class MicPusher : public IAudioPusher<float>, public IAudioIOProcessor
{
public:
	using ToString::toString;

	MicPusher(void);
	virtual ~MicPusher(void);

	// Inherited via IAudioPusher
	virtual void setCallback(void* opaque, void(*pCallback)(void*, const Buffer<float>&));

	virtual int getSamplingRate(void) const;
	virtual int getChannels(void) const;
	virtual JString getError(void) const;

	// Inherited via IAudioIOProcessor
	virtual bool audioProcessing(const void* pInput, void* pOutput, unsigned long frameCount, const PaStreamCallbackTimeInfo* pTimeInfo, PaStreamCallbackFlags statusFlags, void* pUserData, bool outInited);
	virtual JString& toString(JString& retStr, bool withTypes=false) const;
private:
	void(*mpCallback)(void*, const Buffer<float>&);
	void* mpOpaque;
};

// Audio input/output.
static int audioProcessing(const void* pInput, void* pOutput, unsigned long outSamples, const PaStreamCallbackTimeInfo* pTimeInfo, PaStreamCallbackFlags statusFlags, void* pUserData);
static void finitPortaudio(PaError reasonErr);

static PaStream* gStream = NULL;

// simple mixer: sums samples of all inputs
template<typename T>
static void* memAdd(void* dst, const void* src, size_t cnt)
{
	T* dstT = static_cast<T*>(dst);
	const T* srcT = static_cast<const T*>(src);
	cnt /= sizeof(T);
	while(cnt--)
		*dstT++ += *srcT++;
	return dst;
}

static void addProcessor(IAudioIOProcessor* p)
{
	std::lock_guard<std::mutex> lock(gProcessors);
	gProcessors.addElement(p);
}

static void removeProcessor(IAudioIOProcessor* p)
{
	std::lock_guard<std::mutex> lock(gProcessors);
	while(gProcessors.removeElement(p));
}

AudioOut::AudioOut(const ExitGames::Voice::Logger& logger, AudioOutDelayConfig delayConfig, const ExitGames::Common::JString& logPrefix, bool debugInfo)
	: AudioOutDelayControl<float>(logger, delayConfig, logPrefix, debugInfo)
	, mReadPosOutSamples(0)
	, mRingBufSamples(0)
	, mStarted(false)
	, mPlaying(false)
{
	addProcessor(this);
}

AudioOut::~AudioOut(void)
{
	removeProcessor(this);
}

void AudioOut::outCreate(int frequency, int channels, int bufferSamples)
{
	mRingBufSamples = bufferSamples * (int64)SAMPLE_RATE / mFrequency;
}

void AudioOut::outStart()
{
	mRingBuf = Buffer<float>(OUTPUT_CHANNELS * mRingBufSamples);
	mStarted = true;
	mReadPosOutSamples = 0;
	mPlaying = true;
}

int64 AudioOut::outGetPos(void) const
{
	std::lock_guard<std::mutex> lock(mLock);
	return mStarted ? mReadPosOutSamples * mFrequency / SAMPLE_RATE : 0;
}

// writes to the ring buffer which sampling rate (master output SAMPLE_RATE) may differ from the 'data' buffer sampling rate (mFrequency): resample data by SAMPLE_RATE/mFrequency rartio.
void AudioOut::outWrite(Buffer<float> data, int offsetSamples)
{
	std::lock_guard<std::mutex> lock(mLock);

	// in samples
	int dstOffset = offsetSamples * SAMPLE_RATE / mFrequency;
	int dstLen = data.getSize() / mChannels * SAMPLE_RATE / mFrequency;
	int dstPart1 = mRingBufSamples - dstOffset;
	int dstPart2 = dstLen - dstPart1;
	if(dstPart2 > 0) {
		// in samples
		int srcPart1 = mRingBufSamples * (int64)mFrequency / SAMPLE_RATE - offsetSamples;
		int srcPart2 = data.getSize() / mChannels - srcPart1;
		AudioUtil::resample(data, 0, srcPart1, mChannels, mRingBuf, dstOffset, dstPart1, OUTPUT_CHANNELS);
		AudioUtil::resample(data, srcPart1, srcPart2, mChannels, mRingBuf, 0, dstPart2, OUTPUT_CHANNELS);
	} else {
		AudioUtil::resample(data, 0, data.getSize() / mChannels, mChannels, mRingBuf, dstOffset, dstLen, OUTPUT_CHANNELS);
	}
}

bool AudioOut::audioProcessing(const void* pInput, void* pOutput, unsigned long outSamples, const PaStreamCallbackTimeInfo* pTimeInfo, PaStreamCallbackFlags statusFlags, void* pUserData, bool outInited)
{
	std::lock_guard<std::mutex> lock(mLock);
	if(!mPlaying)
		return false;

	size_t outRem = mRingBufSamples - mReadPosOutSamples;
	if(outSamples <= outRem)
	{
		(outInited ? memAdd<float> : memcpy)(static_cast<float*>(pOutput), mRingBuf.getArray() + mReadPosOutSamples * OUTPUT_CHANNELS, outSamples * OUTPUT_CHANNELS * sizeof(float));
		mReadPosOutSamples += outSamples;
	}
	else
	{
		(outInited ? memAdd<float> : memcpy)(static_cast<float*>(pOutput), mRingBuf.getArray() + mReadPosOutSamples * OUTPUT_CHANNELS, outRem * OUTPUT_CHANNELS * sizeof(float));
		(outInited ? memAdd<float> : memcpy)(static_cast<float*>(pOutput) + outRem * OUTPUT_CHANNELS, mRingBuf.getArray(), (outSamples - outRem) * OUTPUT_CHANNELS * sizeof(float));
		mReadPosOutSamples = outSamples - outRem;
	}
	return true;
}

JString& AudioOut::toString(JString& retStr, bool withTypes) const
{
	return retStr
		+= JString(L"{")
		+ L" ringPosOut: " + mReadPosOutSamples
		+ L" frameSampleRate: " + mFrequency
		+ L" frameChannels: " + mChannels
		+ L" ringBuf: " + mRingBuf.toString(withTypes)
		+ L" ringBufSamples: " + mRingBufSamples
		+ L" started: " + (mStarted?L"true":L"false")
		+ L" playing: " + (mPlaying?L"true":L"false")
		+ L"}";
}

MicPusher::MicPusher(void)
	: mpCallback(NULL)
	, mpOpaque(NULL)
{
}

MicPusher::~MicPusher(void)
{
	removeProcessor(this);
}

// Inherited via IAudioPusher
void MicPusher::setCallback(void* opaque, void(*pCallback)(void*, const Buffer<float>&))
{
	mpOpaque = opaque;
	mpCallback = pCallback;
	addProcessor(this);
}

int MicPusher::getSamplingRate(void) const
{
	return SAMPLE_RATE;
}

int MicPusher::getChannels(void) const
{
	return INPUT_CHANNELS;
}

JString MicPusher::getError(void) const
{
	return JString();
}

// Inherited via IAudioIOProcessor
bool MicPusher::audioProcessing(const void* pInput, void* pOutput, unsigned long frameCount, const PaStreamCallbackTimeInfo* pTimeInfo, PaStreamCallbackFlags statusFlags, void* pUserData, bool outInited)
{
	if(mpCallback)
	{
		Buffer<float> f(frameCount * getChannels());
		memcpy(f.getArray(), pInput, frameCount * getChannels() * sizeof(float));
		mpCallback(mpOpaque, f);
	}
	return false;
}

JString& MicPusher::toString(JString& retStr, bool withTypes) const
{
	return retStr += L"{" + Helpers::TypeName::get(this) + L"}";
}

static int audioProcessing(const void* pInput, void* pOutput, unsigned long outSamples, const PaStreamCallbackTimeInfo* pTimeInfo, PaStreamCallbackFlags statusFlags, void* pUserData)
{
	std::lock_guard<std::mutex> lock(gProcessors);
	bool outInited = false;
	for(unsigned int i=0; i<gProcessors.getSize(); ++i)
		if(gProcessors[i]->audioProcessing(pInput, pOutput, outSamples, pTimeInfo, statusFlags, pUserData, outInited))
			outInited = true;
	if(!outInited)
		memset(pOutput, 0, outSamples * OUTPUT_CHANNELS * sizeof(float));
	return paContinue;
}

void initAudoIO(void)
{
	PaStreamParameters  inputParameters;
	PaStreamParameters  outputParameters;
	PaError             err = paNoError;

	printf("patest_record.c\n");
	fflush(stdout);

	err = Pa_Initialize();
	if(err != paNoError)
	{
		finitPortaudio(err);
		return;
	}

	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	if(inputParameters.device == paNoDevice)
	{
		fprintf(stderr, "Error: No default input device.\n");
		finitPortaudio(paNoError);
		return;
	}
	inputParameters.channelCount = INPUT_CHANNELS; /* stereo input */
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if(outputParameters.device == paNoDevice)
	{
		fprintf(stderr, "Error: No default output device.\n");
		finitPortaudio(paNoError);
		return;
	}
	outputParameters.channelCount = OUTPUT_CHANNELS; /* stereo output */
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(&gStream, &inputParameters, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, /* we won't output out of range samples so don't bother clipping them */ audioProcessing, NULL);
	if(err != paNoError)
	{
		gStream = NULL;
		finitPortaudio(err);
		return;
	}

	err = Pa_StartStream(gStream);
	if(err != paNoError)
	{
		finitPortaudio(err);
		return;
	}
	printf("\n=== Now recording!! Please speak into the microphone. ===\n");
	fflush(stdout);
}

void finitAudoIO(void)
{
	finitPortaudio(paNoError);
}

static void finitPortaudio(PaError reasonErr)
{
	PaError err = paNoError;
	if(gStream)
	{
		err = Pa_CloseStream(gStream);
		if(err != paNoError)
		{
			fprintf(stderr, "Error %d while closing device.\n", err);
			fflush(stdout);
		}
	}

	Pa_Terminate();
	if(reasonErr != paNoError)
	{
		fprintf(stderr, "An error occured while using the portaudio stream\n");
		fprintf(stderr, "Error number: %d\n", err);
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
		err = 1; /* Always return 0 or 1, but no other return codes. */
	}
}

IAudioPusher<float>* audioIOCreateMic(void)
{
	return new MicPusher();
}

IAudioOut<float>* audioIOCreateOut(const ExitGames::Voice::Logger& logger, AudioOutDelayConfig delayConfig, const ExitGames::Common::JString& logPrefix, bool debugInfo)
{
	return new AudioOut(logger, delayConfig, logPrefix, debugInfo);
}