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

#include <cstdio>
#include <cstring>
#include <DromeAudio/AudioDriver.h>
#include <DromeAudio/Exception.h>

using namespace DromeAudio;

static void
printUsageMessage(const char *executableName)
{
	fprintf(stderr, "Usage: %s <filename>\n", executableName);
}

int
main(int argc, char *argv[])
{
	// make sure a filename was given
	const char *filename = (argc == 2) ? argv[1] : NULL;
	if(filename == NULL) {
		printUsageMessage(argv[0]);
		return 1;
	}

	// load sound
	SoundPtr sound;
	try {
		sound = Sound::create(filename);
	} catch(Exception ex) {
		fprintf(stderr, "Couldn't open %s\n", filename);
		return 1;
	}

	// create driver
	AudioDriver *driver;
	try {
		driver = AudioDriver::create(AudioDriver::MODE_READ_WRITE);
	} catch(Exception ex) {
		fprintf(stderr, "Audio driver initialization failed\n");
		return 1;
	}

	printf("Using driver: %s\n", driver->getDriverName());

	// set sound as output
	driver->setOutputSound(sound);

	printf("Playing %s\n", filename);

	while(driver->getOutputSampleIndex() < sound->getNumSamples()) {
		unsigned int currentSecond = driver->getOutputSampleIndex() / driver->getSampleRate();
		unsigned int numSeconds = sound->getNumSamples() / sound->getSampleRate();

		unsigned int currentMinute = currentSecond / 60;
		unsigned int numMinutes = numSeconds / 60;

		currentSecond %= 60;
		numSeconds %= 60;

		printf("%02u:%02u / %02u:%02u\r", currentMinute, currentSecond, numMinutes, numSeconds);
	}

	delete driver;
	return 0;
}
