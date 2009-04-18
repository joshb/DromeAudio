/*
 * Copyright (C) 2008-2009 Josh A. Beam <josh@joshbeam.com>
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
#include <DromeAudio/AudioDriverALSA.h>

namespace DromeAudio
{

static const unsigned int RATES[] = { 44100, 22050, 11025 };
static const unsigned int NUM_RATES = sizeof(RATES) / sizeof(RATES[0]);

static void *
alsaRun(void *arg)
{
	((AudioDriverALSA *)arg)->run();

	return NULL;
}

/*
 * AudioDriverALSA class
 */
AudioDriverALSA::AudioDriverALSA(Mode mode)
{
	m_sampleRate = 44100;

	// open output handle
	if(mode != MODE_READ_ONLY) {
		try {
			m_outHandle = pcmOpen(SND_PCM_STREAM_PLAYBACK);
		} catch(Exception ex) {
			throw ex;
		}
	} else {
		m_outHandle = NULL;
	}

	// open input handle
	if(mode != MODE_WRITE_ONLY) {
		try {
			m_inHandle = pcmOpen(SND_PCM_STREAM_CAPTURE);
		} catch(Exception ex) {
			snd_pcm_close(m_outHandle);
			throw ex;
		}
	} else {
		m_inHandle = NULL;
	}

	// create thread
	m_running = true;
	if(pthread_create(&m_thread, NULL, alsaRun, this) != 0) {
		if(m_inHandle != NULL)
			snd_pcm_close(m_inHandle);
		if(m_outHandle != NULL)
			snd_pcm_close(m_outHandle);
		throw Exception("AudioDriverALSA::AudioDriverALSA(): Unable to create thread");
	}
}

AudioDriverALSA::~AudioDriverALSA()
{
	m_running = false;
	pthread_join(m_thread, NULL);

	if(m_inHandle != NULL)
		snd_pcm_drain(m_inHandle);
	if(m_outHandle != NULL)
		snd_pcm_drain(m_outHandle);
}

snd_pcm_t *
AudioDriverALSA::pcmOpen(snd_pcm_stream_t stream)
{
	const unsigned int numChannels = 2;
	snd_pcm_t *handle;

	// open pcm device
	if(snd_pcm_open(&handle, "default", stream, SND_PCM_NONBLOCK) != 0)
		throw Exception("AudioDriverALSA::pcmOpen(): snd_pcm_open failed");

	snd_pcm_hw_params_t *params;
	snd_pcm_hw_params_malloc(&params);

	// fill params
	if(snd_pcm_hw_params_any(handle, params) != 0) {
		snd_pcm_hw_params_free(params);
		snd_pcm_close(handle);
		throw Exception("AudioDriverALSA::pcmOpen(): snd_pcm_hw_params_any failed");
	}

	// set interleaved access
	if(snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED) != 0) {
		snd_pcm_hw_params_free(params);
		snd_pcm_close(handle);
		throw Exception("AudioDriverALSA::pcmOpen(): snd_pcm_hw_params_set_access failed");
	}

	// set floating point sample format
	if(snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_FLOAT) != 0) {
		snd_pcm_hw_params_free(params);
		snd_pcm_close(handle);
		throw Exception("AudioDriverALSA::pcmOpen(): snd_pcm_hw_params_set_format failed");
	}

	// set sample rate
	if(snd_pcm_hw_params_set_rate_near(handle, params, &m_sampleRate, 0) != 0) {
		snd_pcm_hw_params_free(params);
		snd_pcm_close(handle);
		throw Exception("AudioDriverALSA::pcmOpen(): snd_pcm_hw_params_set_rate_near failed");
	}

	// set number of channels
	if(snd_pcm_hw_params_set_channels(handle, params, numChannels) != 0) {
		snd_pcm_hw_params_free(params);
		snd_pcm_close(handle);
		throw Exception("AudioDriverALSA::pcmOpen(): snd_pcm_hw_params_set_channels failed");
	}

	// apply parameters
	if(snd_pcm_hw_params(handle, params) != 0) {
		snd_pcm_hw_params_free(params);
		snd_pcm_close(handle);
		throw Exception("AudioDriverALSA::pcmOpen(): snd_pcm_hw_params failed");
	}
	snd_pcm_hw_params_free(params);

	// prepare PCM
	if(snd_pcm_prepare(handle) != 0) {
		snd_pcm_close(handle);
		throw Exception("AudioDriverALSA::pcmOpen(): snd_pcm_prepare failed");
	}

	return handle;
}

const char *
AudioDriverALSA::getDriverName() const
{
	return "AudioDriverALSA";
}

void
AudioDriverALSA::writeSample(const Sample &sample)
{
	if(!m_outHandle)
		throw Exception("AudioDriverALSA::writeSample(): No output handle");

	float s[2];

	s[0] = sample[0];
	s[1] = sample[1];

	snd_pcm_writei(m_outHandle, s, 1);
}

Sample
AudioDriverALSA::readSample()
{
	if(!m_inHandle)
		throw Exception("AudioDriverALSA::readSample(): No input handle");

	float s[2];
	if(snd_pcm_readi(m_inHandle, s, 1) != 1)
		throw Exception("AudioDriverALSA::readSample(): snd_pcm_readi failed");

	Sample sample;
	sample[0] = s[0];
	sample[1] = s[1];

	return sample;
}

void
AudioDriverALSA::run()
{
	while(m_running) {
		snd_pcm_sframes_t frames = snd_pcm_avail_update(m_outHandle);

		if(frames < 0)
			throw Exception("AudioDriverALSA::ALSACallback(): frames < 0 (%d)", (int)frames);

		writeSamples((unsigned int)frames);
	}
}

} // namespace DromeAudio
