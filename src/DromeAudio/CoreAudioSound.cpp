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

#include <cstring>
#include <AudioToolbox/AudioToolbox.h>
#include <DromeAudio/Exception.h>
#include <DromeAudio/CoreAudioSound.h>

namespace DromeAudio {

/*
 * CoreAudioSound class
 */
CoreAudioSound::CoreAudioSound(const char *filename)
{
	// create CFURLRef for the given file path
	CFURLRef url = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault, (UInt8 *)filename, strlen(filename), false);

	// open audio file
	ExtAudioFileRef audioFileRef;
	OSStatus err = ExtAudioFileOpenURL(url, &audioFileRef);
	if(err)
		throw Exception("CoreAudioSound::CoreAudioSound(): Unable to open %s for reading", filename);

	// get audio file format
	AudioStreamBasicDescription inFormat;
	UInt32 size = sizeof(inFormat);
	err = ExtAudioFileGetProperty(audioFileRef, kExtAudioFileProperty_FileDataFormat, &size, &inFormat);
	if(err) {
		ExtAudioFileDispose(audioFileRef);
		throw Exception("CoreAudioSound::CoreAudioSound(): ExtAudioFileGetProperty failed [FileDataFormat]");
	}

	// set output format
	AudioStreamBasicDescription outFormat;
	outFormat.mSampleRate = inFormat.mSampleRate;
	outFormat.mFormatID = kAudioFormatLinearPCM;
	outFormat.mFormatFlags = kLinearPCMFormatFlagIsFloat;
	outFormat.mBytesPerPacket = sizeof(Sample);
	outFormat.mFramesPerPacket = 1;
	outFormat.mBytesPerFrame = sizeof(Sample);
	outFormat.mChannelsPerFrame = 2;
	outFormat.mBitsPerChannel = (sizeof(Sample) / 2) * 8;
	outFormat.mReserved = 0;
	size = sizeof(outFormat);
	err = ExtAudioFileSetProperty(audioFileRef, kExtAudioFileProperty_ClientDataFormat, size, &outFormat);
	if(err) {
		ExtAudioFileDispose(audioFileRef);
		throw Exception("CoreAudioSound::CoreAudioSound(): ExtAudioFileSetProperty failed [ClientDataFormat]");
	}

	// set some properties based on output format
	m_numChannels = outFormat.mChannelsPerFrame;
	m_bytesPerSample = outFormat.mBitsPerChannel / 8;
	m_sampleRate = (unsigned int)outFormat.mSampleRate;

	// get total number of frames
	SInt64 totalFrames;
	size = sizeof(totalFrames);
	err = ExtAudioFileGetProperty(audioFileRef, kExtAudioFileProperty_FileLengthFrames, &size, &totalFrames);
	if(err) {
		ExtAudioFileDispose(audioFileRef);
		throw Exception("CoreAudioSound::CoreAudioSound(): ExtAudioFileGetProperty failed [FileLengthFrames]");
	}
	m_numSamples = (unsigned int)totalFrames;

	// start reading audio data
	UInt32 framesRead = 0;
	m_samples = new Sample[m_numSamples];
	while(framesRead < m_numSamples) {
		// number of frames to read
		UInt32 frames = (UInt32)m_numSamples - framesRead;

		// create audio buffer list for sample block
		AudioBufferList bufList;
		bufList.mNumberBuffers = 1;
		bufList.mBuffers[0].mNumberChannels = inFormat.mChannelsPerFrame;
		bufList.mBuffers[0].mDataByteSize = sizeof(Sample) * frames;
		bufList.mBuffers[0].mData = (m_samples + framesRead);

		// read audio data
		err = ExtAudioFileRead(audioFileRef, &frames, &bufList);
		if(err) {
			delete [] m_samples;
			ExtAudioFileDispose(audioFileRef);
			throw Exception("CoreAudioSound::CoreAudioSound(): ExtAudioFileRead failed (err = %d)", err);
		}

		framesRead += frames;

		// if we read 0 frames, we've reached
		// the end of the file
		if(frames == 0)
			break;
	}

	// close audio file
	ExtAudioFileDispose(audioFileRef);
}

CoreAudioSound::~CoreAudioSound()
{
	delete [] m_samples;
}

unsigned char
CoreAudioSound::getNumChannels() const
{
	return m_numChannels;
}

unsigned int
CoreAudioSound::getSampleRate() const
{
	return m_sampleRate;
}

unsigned int
CoreAudioSound::getNumSamples() const
{
	return m_numSamples;
}

Sample
CoreAudioSound::getSample(unsigned int index) const
{
	index %= m_numSamples;
	return m_samples[index];
}

CoreAudioSoundPtr
CoreAudioSound::create(const char *filename)
{
	return CoreAudioSoundPtr(new CoreAudioSound(filename));
}

} // namespace DromeAudio
