#include <cstdio>
#include <cstring>
#include <DromeAudio/AudioContext.h>
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
		driver = AudioDriver::create();
	} catch(Exception ex) {
		fprintf(stderr, "Audio driver initialization failed\n");
		return 1;
	}

	printf("Using driver: %s\n", driver->getDriverName());

	// create context
	AudioContext *context = new AudioContext(driver->getSampleRate());
	driver->setAudioContext(context);

	printf("Playing %s\n", filename);

	// create sound emitter and loop until it's done playing
	SoundEmitterPtr emitter = context->playSound(sound);
	emitter->setLoop(false);
	while(emitter->isDone() == false) {
		unsigned int currentSecond = emitter->getSampleIndex() / emitter->getSampleRate();
		unsigned int numSeconds = emitter->getSound()->getNumSamples() / emitter->getSound()->getSampleRate();

		unsigned int currentMinute = currentSecond / 60;
		unsigned int numMinutes = numSeconds / 60;

		currentSecond %= 60;
		numSeconds %= 60;

		printf("%02u:%02u / %02u:%02u\r", currentMinute, currentSecond, numMinutes, numSeconds);
	}

	// clean up
	delete driver;
	delete context;

	printf("\nDone\n");
	return 0;
}
