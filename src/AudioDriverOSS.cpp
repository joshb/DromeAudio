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

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <DromeAudio/Exception.h>
#include <DromeAudio/AudioDriverOSS.h>

namespace DromeAudio
{

static const int RATES[] = { 44100, 22050, 11025 };
static const int NUM_RATES = sizeof(RATES) / sizeof(RATES[0]);

static void *
ossRun(void *arg)
{
	((AudioDriverOSS *)arg)->run();

	return NULL;
}

/*
 * AudioDriverOSS class
 */
AudioDriverOSS::AudioDriverOSS(Mode mode)
{
	const char *device = "/dev/dsp";

	// set open flags for the given mode
	int flags;
	switch(mode) {
		default:
		case MODE_WRITE_ONLY:
			flags = O_WRONLY;
			break;
		case MODE_READ_ONLY:
			flags = O_RDONLY;
			break;
		case MODE_READ_WRITE:
			flags = O_RDWR;
			break;
	}

	// open device
	m_fd = open(device, flags, 0);
	if(m_fd == -1)
		throw Exception("AudioDriverOSS::AudioDriverOSS(): Unable to open %s", device);

	// set sample format
	int format = AFMT_S16_NE;
	if(ioctl(m_fd, SNDCTL_DSP_SETFMT, &format) == -1) {
		close(m_fd);
		throw Exception("AudioDriverOSS::AudioDriverOSS(): Unable to set sample format");
	}

	// set number of channels
	int channels = 2;
	if(ioctl(m_fd, SNDCTL_DSP_CHANNELS, &channels) == -1) {
		close(m_fd);
		throw Exception("AudioDriverOSS::AudioDriverOSS(): Unable to set number of channels");
	}

	// set sample rate
	int rate;
	for(int i = 0; i < NUM_RATES; i++) {
		rate = RATES[i];
		if(ioctl(m_fd, SNDCTL_DSP_SPEED, &rate) == -1)
			rate = 0;
		else
			break;
	}
	if(rate == 0) {
		close(m_fd);
		throw Exception("AudioDriverOSS::AudioDriverOSS(): Unable to set sample rate");
	}

	m_sampleRate = rate;

	// create thread
	m_running = true;
	if(pthread_create(&m_thread, NULL, ossRun, this) != 0) {
		close(m_fd);
		throw Exception("AudioDriverOSS::AudioDriverOSS(): Unable to create thread");
	}
}

AudioDriverOSS::~AudioDriverOSS()
{
	m_running = false;
	pthread_join(m_thread, NULL);

	close(m_fd);
}

const char *
AudioDriverOSS::getDriverName() const
{
	return "AudioDriverOSS";
}

void
AudioDriverOSS::writeSample(const Sample &sample)
{
	int16_t s[2];

	s[0] = (int16_t)(sample[0] * 32767.0f);
	s[1] = (int16_t)(sample[1] * 32767.0f);

	write(m_fd, s, sizeof(s));
}

Sample
AudioDriverOSS::readSample()
{
	int16_t s[2];

	read(m_fd, s, sizeof(s));

	return Sample::fromInt16(s, 2);
}

void
AudioDriverOSS::run()
{
	while(m_running)
		writeSamples(1);
}

} // namespace DromeAudio
