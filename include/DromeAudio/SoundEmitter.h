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

#ifndef __DROMEAUDIO_SOUNDEMITTER_H__
#define __DROMEAUDIO_SOUNDEMITTER_H__

#include <DromeAudio/Sound.h>

namespace DromeAudio {

class SoundEmitter;
typedef RefPtr <SoundEmitter> SoundEmitterPtr;

/** \brief Plays a Sound when attached to an AudioContext.
 */
class SoundEmitter : public Sound
{
	/**
	 * \example DromeAudioPlayer.cpp
	 */

	protected:
		unsigned int m_sampleRate;
		SoundPtr m_sound;

		bool m_loop;
		bool m_paused;
		float m_volume;
		float m_balance;

		unsigned int m_sampleIndex;

		SoundEmitter(unsigned int sampleRate);
		virtual ~SoundEmitter() { }

		float getSampleIndexFactor() const;

	public:
		uint8_t getNumChannels() const;

		/**
		 * Gets the sample rate of the emitter. This is the rate at which the emitter's Sound is played, which may not be the same as the sound's sample rate. This should be set to the target sample rate of the AudioContext that the emitter is attached to.
		 * @return Sample rate.
		 */
		unsigned int getSampleRate() const;

		/**
		 * Sets the sample rate of the emitter.
		 * @param value Sample rate.
		 */
		void setSampleRate(unsigned int value);

		/**
		 * Gets the Sound associated with the emitter.
		 * @return SoundPtr to the emitter's associated Sound.
		 */
		SoundPtr getSound() const;

		/**
		 * Sets the Sound associated with the emitter.
		 * @param value SoundPtr to the emitter's associated Sound.
		 */
		void setSound(SoundPtr value);

		unsigned int getNumSamples() const;

		/**
		 * Gets the loop value of the emitter. This indicates whether the emitter will loop once the end of its associated Sound has been reached.
		 * @return True if the emitter loops.
		 */
		bool getLoop() const;

		/**
		 * Sets the loop value of the emitter.
		 * @param value True if the emitter should loop.
		 */
		void setLoop(bool value);

		/**
		 * Gets the paused state of the emitter. When the emitter is paused, its sample index will not be incremented automatically when the next sample is retrieved from it.
		 * @return True if the emitter is paused.
		 */
		bool getPaused() const;

		/**
		 * Sets the paused state of the emitter.
		 * @param value True to pause the emitter.
		 */
		void setPaused(bool value);

		/**
		 * Gets the volume of the emitter. This is a factor from 0 to 1 that Sample objects returned by the emitter will be multiplied by.
		 * @return Volume value with a range of [0, 1].
		 */
		float getVolume() const;

		/**
		 * Sets the volume of the emitter.
		 * @param value Volume value with a range of [0, 1].
		 */
		void setVolume(float value);

		/**
		 * Gets the balance of the emitter. This is a value from -1 to 1, with -1 indicating that samples returned by the emitter will be contained completely in the left channel, 0 being normal, and 1 indicating that returned samples will be contained completely in the right channel.
		 * @return Balance value with a range of [-1, 1].
		 */
		float getBalance() const;

		/**
		 * Sets the balance of the emitter.
		 * @param value Balance value with a range of [-1, 1].
		 */
		void setBalance(float value);

		/**
		 * Gets the current sample index of the emitter. This is the index of the next Sample from the emitter's associated Sound that will be returned.
		 * @return Current sample index.
		 */
		unsigned int getSampleIndex() const;

		/**
		 * Sets the current sample index of the emitter.
		 * @param value New sample index.
		 */
		void setSampleIndex(unsigned int value);

		/**
		 * Gets a value indicating whether the emitter is done playing. This usually occurs when the emitter reaches the end of its associated Sound and is not supposed to loop.
		 * @return True if the emitter is done playing its associated Sound.
		 */
		bool isDone();

		virtual Sample getSample(unsigned int sampleIndex) const;

		/**
		 * Gets the next Sample of the emitter's associated Sound to be played.
		 * @return Next Sample to be played.
		 */
		virtual Sample getNextSample();

		/**
		 * Creates a new SoundEmitter.
		 * @param sampleRate The sample rate of the emitter to be created.
		 * @return SoundEmitterPtr to new SoundEmitter.
		 */
		static SoundEmitterPtr create(unsigned int sampleRate);
};

} // namespace DromeAudio

#endif /* __DROMEAUDIO_SOUNDEMITTER_H__ */
