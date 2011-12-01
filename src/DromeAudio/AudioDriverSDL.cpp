/*
 * Copyright (C) 2008 Josh A. Beam
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

#include <SDL/SDL.h>
#include <DromeAudio/Exception.h>
#include <DromeAudio/Endian.h>
#include <DromeAudio/AudioContext.h>
#include <DromeAudio/AudioDriverSDL.h>

namespace DromeAudio
{

static const unsigned int RATES[] = { 44100, 22050, 11025 };
static const unsigned int NUM_RATES = sizeof(RATES) / sizeof(RATES[0]);

static void
SdlAudioCallback(void *driver, Uint8 *stream, int length)
{
	((AudioDriverSDL *)driver)->SdlCallback(stream, length);
}

/*
 * AudioDriverSDL class
 */
AudioDriverSDL::AudioDriverSDL()
{
	m_Data = NULL;
	m_SampleIndex = 0;
	m_NumSamples = 0;

	// initialize SDL audio if necessary
	if(!(SDL_WasInit(0) & SDL_INIT_AUDIO)) {
		if(SDL_Init(SDL_INIT_AUDIO) == -1)
			throw Exception("AudioDriverSDL::AudioDriverSDL(): SDL_Init failed");
	}

	// set format
	SDL_AudioSpec spec;
	spec.format = AUDIO_S16SYS;
	spec.channels = 2;
	spec.samples = 512;
	spec.callback = SdlAudioCallback;
	spec.userdata = this;

	// find a working sample rate
	m_SampleRate = 0;
	for(unsigned int i = 0; i < NUM_RATES; i++) {
		spec.freq = RATES[i];
		if(SDL_OpenAudio(&spec, NULL) != -1) {
			m_SampleRate = RATES[i];
			break;
		}
	}

	if(m_SampleRate == 0)
		throw Exception("AudioDriverSDL::AudioDriverSDL(): Couldn't find a working sample rate: %s", SDL_GetError());

	SDL_PauseAudio(0);
}

AudioDriverSDL::~AudioDriverSDL()
{
	SDL_CloseAudio();
}

const char *
AudioDriverSDL::GetDriverName() const
{
	return "AudioDriverSDL";
}

void
AudioDriverSDL::WriteSample(const Sample &sample)
{
	if(m_SampleIndex >= m_NumSamples)
		return;

	m_Data[m_SampleIndex * 2 + 0] = (int16_t)(32767.0f * sample[0]);
	m_Data[m_SampleIndex * 2 + 1] = (int16_t)(32767.0f * sample[1]);

	++m_SampleIndex;
}

void
AudioDriverSDL::SdlCallback(uint8_t *stream, int length)
{
	m_Data = (int16_t *)stream;
	m_SampleIndex = 0;
	m_NumSamples = length / 4;

	if(m_AudioContext) {
		m_AudioContext->WriteSamples(this, m_NumSamples);
	} else {
		for(unsigned int i = 0; i < m_NumSamples * 2; i++)
			m_Data = 0;
	}
}

} // namespace DromeAudio
