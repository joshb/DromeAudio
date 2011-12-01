/*
 * Copyright (C) 2010 Josh A. Beam
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <DromeAudio/Exception.h>
#include <DromeAudio/Endian.h>
#include <DromeAudio/AudioContext.h>
#include <DromeAudio/AudioDriverOSX.h>
#include <CoreServices/CoreServices.h>

namespace DromeAudio
{

static void *
osxRunLoop(void *arg)
{
	AudioDriverOSX *driver = (AudioDriverOSX *)arg;
	driver->runLoop();
	return NULL;
}

static OSStatus
osxRenderCallback(void *inRefCon,
                  AudioUnitRenderActionFlags *ioActionFlags,
                  const AudioTimeStamp *inTimeStamp,
                  UInt32 inBusNumber,
                  UInt32 inNumberFrames,
                  AudioBufferList *ioData)
{
	AudioDriverOSX *driver = (AudioDriverOSX *)inRefCon;
	driver->renderCallback(ioActionFlags, inTimeStamp, inBusNumber, inNumberFrames, ioData);

	return noErr;
}

/*
 * AudioDriverOSX class
 */
AudioDriverOSX::AudioDriverOSX()
{
	m_sampleRate = 44100.0;

	m_data[0] = NULL;
	m_data[1] = NULL;
	m_sampleIndex = 0;
	m_numSamples = 0;

	// create ComponentDescription
	ComponentDescription desc;
	desc.componentType = kAudioUnitType_Output;
	desc.componentSubType = kAudioUnitSubType_DefaultOutput;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;

	// get Component
	Component component = FindNextComponent(NULL, &desc);
	if(!component)
		throw Exception("AudioDriverOSX::AudioDriverOSX(): FindNextComponent failed");

	// open AudioUnit
	if(OpenAComponent(component, &m_outputUnit) != noErr)
		throw Exception("AudioDriverOSX::AudioDriverOSX(): OpenAComponent failed");

	// configure callback
	AURenderCallbackStruct callback;
	callback.inputProc = osxRenderCallback;
	callback.inputProcRefCon = this;
	OSStatus result = AudioUnitSetProperty(m_outputUnit,
	                                       kAudioUnitProperty_SetRenderCallback,
	                                       kAudioUnitScope_Global,
	                                       0,
	                                       &callback,
	                                       sizeof(callback));
	if(result != noErr)
		throw Exception("AudioDriverOSX::AudioDriverOSX(): AudioUnitSetProperty (for SetRenderCallback) failed");

	// configure audio stream
	AudioStreamBasicDescription streamDesc;
	streamDesc.mSampleRate = m_sampleRate;
	streamDesc.mFormatID = kAudioFormatLinearPCM;
	streamDesc.mFormatFlags = kAudioFormatFlagsNativeFloatPacked;
	streamDesc.mBitsPerChannel = 32;
	streamDesc.mChannelsPerFrame = 2;
	streamDesc.mBytesPerFrame = streamDesc.mChannelsPerFrame * (streamDesc.mBitsPerChannel / 8);
	streamDesc.mFramesPerPacket = 1;
	streamDesc.mBytesPerPacket = streamDesc.mBytesPerFrame * streamDesc.mFramesPerPacket;
	result = AudioUnitSetProperty(m_outputUnit,
	                              kAudioUnitProperty_StreamFormat,
	                              kAudioUnitScope_Global,
	                              0,
	                              &streamDesc,
	                              sizeof(streamDesc));
	if(result != noErr)
		throw Exception("AudioDriverOSX::AudioDriverOSX(): AudioUnitSetProperty (for StreamFormat) failed");

	// initialize AudioUnit
	if(AudioUnitInitialize(m_outputUnit) != noErr)
		throw Exception("AudioDriverOSX::AudioDriverOSX(): AudioUnitInitialize failed");

	// start output
	if(AudioOutputUnitStart(m_outputUnit) != noErr)
		throw Exception("AudioDriverOSX::AudioDriverOSX(): AudioOutputUnitStart failed");

	// create thread to call runLoop
	m_running = true;
	if(pthread_create(&m_thread, NULL, osxRunLoop, this) != 0)
		throw Exception("AudioDriverOSX::AudioDriverOSX(): pthread_create failed");
}

AudioDriverOSX::~AudioDriverOSX()
{
	// stop the runLoop thread
	m_running = false;
	if(pthread_join(m_thread, NULL) != 0)
		throw Exception("AudioDriverOSX::~AudioDriverOSX(): pthread_join failed");

	// stop output
	if(AudioOutputUnitStop(m_outputUnit) != noErr)
		throw Exception("AudioDriverOSX::~AudioDriverOSX(): AudioOutputUnitStop failed");

	// uninitialize AudioUnit
	if(AudioUnitUninitialize(m_outputUnit) != noErr)
		throw Exception("AudioDriverOSX::~AudioDriverOSX(): AudioUnitUninitialize failed");
}

const char *
AudioDriverOSX::getDriverName() const
{
	return "AudioDriverOSX";
}

void
AudioDriverOSX::writeSample(const Sample &sample)
{
	if(m_sampleIndex >= m_numSamples)
		return;

	m_data[0][m_sampleIndex] = sample[0];
	m_data[1][m_sampleIndex] = sample[1];

	++m_sampleIndex;
}

void
AudioDriverOSX::runLoop()
{
	while(m_running)
		CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
}

void
AudioDriverOSX::renderCallback(AudioUnitRenderActionFlags * /*ioActionFlags*/,
                               const AudioTimeStamp * /*inTimeStamp*/,
                               UInt32 /*inBusNumber*/,
                               UInt32 inNumberFrames,
                               AudioBufferList *ioData)
{
	m_data[0] = (float *)ioData->mBuffers[0].mData;
	m_data[1] = (float *)ioData->mBuffers[1].mData;
	m_sampleIndex = 0;
	m_numSamples = inNumberFrames;

	if(m_audioContext) {
		m_audioContext->writeSamples(this, m_numSamples);
	} else {
		for(unsigned int i = 0; i < m_numSamples; i++) {
			m_data[0][i] = 0.0f;
			m_data[1][i] = 0.0f;
		}
	}
}

} // namespace DromeAudio
