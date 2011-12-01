/*
 * Copyright (C) 2008-2010 Josh A. Beam
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

#include <alsa/asoundlib.h>
#include <DromeAudio/Exception.h>
#include <DromeAudio/Endian.h>
#include <DromeAudio/AudioContext.h>
#include <DromeAudio/AudioDriverALSA.h>

namespace DromeAudio
{

static const unsigned int RATES[] = { 44100, 22050, 11025 };
static const unsigned int NUM_RATES = sizeof(RATES) / sizeof(RATES[0]);

static void alsaAudioCallback(snd_async_handler_t *handler)
{
	AudioDriverALSA *driver = (AudioDriverALSA *)snd_async_handler_get_callback_private(handler);
	driver->alsaCallback();
}

/*
 * AudioDriverALSA class
 */
AudioDriverALSA::AudioDriverALSA()
{
	const unsigned int numChannels = 2;

	m_sampleIndex = 0;
	m_numSamples = 0;

	// open pcm device
	if(snd_pcm_open((snd_pcm_t **)&m_handle, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK) != 0)
		throw Exception("AudioDriverALSA::AudioDriverALSA(): snd_pcm_open failed");

	snd_pcm_hw_params_t *params;
	snd_pcm_hw_params_malloc(&params);

	// fill params
	if(snd_pcm_hw_params_any((snd_pcm_t *)m_handle, params) != 0)
		throw Exception("AudioDriverALSA::AudioDriverALSA(): snd_pcm_hw_params_any failed");

	// set interleaved access
	if(snd_pcm_hw_params_set_access((snd_pcm_t *)m_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED) != 0)
		throw Exception("AudioDriverALSA::AudioDriverALSA(): snd_pcm_hw_params_set_access failed");

	// set floating point sample format
	if(snd_pcm_hw_params_set_format((snd_pcm_t *)m_handle, params, SND_PCM_FORMAT_FLOAT) != 0)
		throw Exception("AudioDriverALSA::AudioDriverALSA(): snd_pcm_hw_params_set_format failed");

	// set sample rate
	m_sampleRate = 44100;
	if(snd_pcm_hw_params_set_rate_near((snd_pcm_t *)m_handle, params, &m_sampleRate, 0) != 0)
		throw Exception("AudioDriverALSA::AudioDriverALSA(): snd_pcm_hw_params_set_rate_near failed");

	// set number of channels
	if(snd_pcm_hw_params_set_channels((snd_pcm_t *)m_handle, params, numChannels) != 0)
		throw Exception("AudioDriverALSA::AudioDriverALSA(): snd_pcm_hw_params_set_channels failed");

	// apply parameters
	if(snd_pcm_hw_params((snd_pcm_t *)m_handle, params) != 0)
		throw Exception("AudioDriverALSA::AudioDriverALSA(): snd_pcm_hw_params failed");
	snd_pcm_hw_params_free(params);

	// prepare PCM
	if(snd_pcm_prepare((snd_pcm_t *)m_handle) != 0)
		throw Exception("AudioDriverALSA::AudioDriverALSA(): snd_pcm_prepare failed");

	// setup async
	if(snd_async_add_pcm_handler((snd_async_handler_t **)&m_asyncHandler, (snd_pcm_t *)m_handle, alsaAudioCallback, this) != 0)
		throw Exception("AudioDriverALSA::AudioDriverALSA(): snd_async_add_pcm_handler failed");

	// the async handler will be called for the first time
	// after we write all of the initial output frames
	float f[2] = { 0.0f, 0.0f };
	snd_pcm_sframes_t frames = snd_pcm_avail_update((snd_pcm_t *)m_handle);
	if(frames < 0)
		throw Exception("AudioDriverALSA::AudioDriverALSA(): frames < 0 (%d)", (int)frames);
	for(snd_pcm_sframes_t i = 0; i < frames; ++i)
		snd_pcm_writei((snd_pcm_t *)m_handle, f, 1);
}

AudioDriverALSA::~AudioDriverALSA()
{
	snd_async_del_handler((snd_async_handler_t *)m_asyncHandler);
	snd_pcm_drain((snd_pcm_t *)m_handle);
}

const char *
AudioDriverALSA::getDriverName() const
{
	return "AudioDriverALSA";
}

void
AudioDriverALSA::writeSample(const Sample &sample)
{
	if(m_sampleIndex >= m_numSamples)
		return;

	m_data[m_sampleIndex * 2 + 0] = sample[0];
	m_data[m_sampleIndex * 2 + 1] = sample[1];

	++m_sampleIndex;
}

void
AudioDriverALSA::alsaCallback()
{
	snd_pcm_sframes_t frames = snd_pcm_avail_update((snd_pcm_t *)m_handle);
	if(frames < 0)
		throw Exception("AudioDriverALSA::ALSACallback(): frames < 0 (%d)", (int)frames);

	m_sampleIndex = 0;
	m_numSamples = (unsigned int)frames;
	m_data = new float[m_numSamples * 2];

	if(m_audioContext) {
		m_audioContext->writeSamples(this, m_numSamples);
	} else {
		for(unsigned int i = 0; i < m_numSamples * 2; i++)
			m_data[i] = 0.0f;
	}

	snd_pcm_writei((snd_pcm_t *)m_handle, m_data, m_numSamples);

	delete [] m_data;
}

} // namespace DromeAudio
