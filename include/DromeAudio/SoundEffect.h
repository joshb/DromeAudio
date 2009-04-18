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

#ifndef __DROMEAUDIO_SOUNDEFFECT_H__
#define __DROMEAUDIO_SOUNDEFFECT_H__

#include <DromeAudio/Exception.h>
#include <DromeAudio/Sound.h>

namespace DromeAudio {

/*
 * SoundEffect
 */
class SoundEffect;
typedef RefPtr <SoundEffect> SoundEffectPtr;

class SoundEffect : public Sound
{
	protected:
		SoundPtr m_sound;

		SoundEffect() { }
		SoundEffect(SoundPtr sound) { setSound(sound); }
		virtual ~SoundEffect() { }

	public:
		virtual unsigned char getNumChannels() const;
		virtual unsigned int getSampleRate() const;
		virtual unsigned int getNumSamples() const;

		SoundPtr getSound() const { return m_sound; }
		virtual void setSound(SoundPtr value) { m_sound = value; }

		virtual Sample getSample(unsigned int index) const = 0;
};

/*
 * PitchShiftSoundEffect
 */
class PitchShiftSoundEffect;
typedef RefPtr <PitchShiftSoundEffect> PitchShiftSoundEffectPtr;

class PitchShiftSoundEffect : public SoundEffect
{
	protected:
		float m_factor;

		PitchShiftSoundEffect(SoundPtr sound, float factor);

	public:
		unsigned int getNumSamples() const;

		float getFactor() const { return m_factor; }
		void setFactor(float value)
		{
			if(value <= 0.0f)
				throw Exception("PitchShiftSoundEffect::SetFactor(): Invalid factor value (%f)", value);

			m_factor = value;
		}

		Sample getSample(unsigned int index) const;

		static PitchShiftSoundEffectPtr create(SoundPtr sound, float factor);
};

/*
 * OscillatorSoundEffect
 */
class OscillatorSoundEffect;
typedef RefPtr <OscillatorSoundEffect> OscillatorSoundEffectPtr;

class OscillatorSoundEffect : public SoundEffect
{
	protected:
		float m_frequency;

		OscillatorSoundEffect(SoundPtr sound, float frequency);

	public:
		float getFrequency() const { return m_frequency; }
		void setFrequency(float value)
		{
			if(value < 0.0f)
				m_frequency = 0.0f;
			else
				m_frequency = value;
		}

		Sample getSample(unsigned int index) const;

		OscillatorSoundEffectPtr create(SoundPtr sound, float frequency);
};

/*
 * EchoSoundEffect
 */
class EchoSoundEffect;
typedef RefPtr <EchoSoundEffect> EchoSoundEffectPtr;

class EchoSoundEffect : public SoundEffect
{
	protected:
		float m_delay;
		float m_factor;
		unsigned int m_count;

		EchoSoundEffect(SoundPtr sound, float delay, float factor, unsigned int count);

	public:
		unsigned int getNumSamples() const;

		float getDelay() const { return m_delay; }
		void setDelay(float value) { m_delay = value; }

		float getFactor() const { return m_factor; }
		void setFactor(float value) { m_factor = value; }

		unsigned int getCount() const { return m_count; }
		void setCount(unsigned int value) { m_count = value; }

		Sample getSample(unsigned int index) const;

		static EchoSoundEffectPtr create(SoundPtr sound, float delay, float factor, unsigned int count);
};

} // namespace DromeAudio

#endif /* __DROMEAUDIO_SOUNDEFFECT_H__ */
