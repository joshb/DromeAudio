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

#ifndef __DROMEAUDIO_MIXER_H__
#define __DROMEAUDIO_MIXER_H__

#include <DromeAudio/Sound.h>

namespace DromeAudio {

enum MixerMode
{
	MIXER_MODE_ADD = 0,
	MIXER_MODE_SUBTRACT,
	MIXER_MODE_MULTIPLY
};

class Mixer;
typedef RefPtr <Mixer> MixerPtr;

class Mixer : public Sound
{
	protected:
		MixerMode m_mode;
		SoundPtr m_sound1;
		SoundPtr m_sound2;

		Mixer(MixerMode mode);

	public:
		unsigned char getNumChannels() const;
		unsigned int getSampleRate() const;
		unsigned int getNumSamples() const;

		SoundPtr getSound1() const { return m_sound1; }
		void setSound1(SoundPtr value) { m_sound1 = value; }

		SoundPtr getSound2() const { return m_sound2; }
		void setSound2(SoundPtr value) { m_sound2 = value; }

		Sample getSample(unsigned int index) const;

		static MixerPtr create(MixerMode mode = MIXER_MODE_ADD);
};

} // namespace DromeAudio

#endif /* __DROMEAUDIO_MIXER_H__ */
