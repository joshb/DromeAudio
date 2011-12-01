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

#import "DromeAudioPlayerCocoaAppDelegate.h"

using namespace DromeAudio;

@implementation DromeAudioPlayerCocoaAppDelegate

@synthesize window;
@synthesize playButton;
@synthesize stopButton;
@synthesize positionSlider;
@synthesize positionLabel;
@synthesize echoWindow;
@synthesize echoDelayTextField;
@synthesize echoDelayStepper;
@synthesize echoFactorSlider;
@synthesize echoCountTextField;
@synthesize echoCountStepper;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	// create driver
	driver = AudioDriver::create();
	
	// create context
	context = new AudioContext(driver->getSampleRate());
	driver->setAudioContext(context);
	
	// create timer
	timer = [NSTimer timerWithTimeInterval:0.1f target:self selector:@selector(timerFireMethod:) userInfo:nil repeats:YES];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
	return YES;
}

- (void)timerFireMethod:(NSTimer *)theTimer
{
	if(!emitter)
		return;
		
	[positionSlider setIntegerValue:emitter->getSampleIndex()];
	
	unsigned int currentSecond = emitter->getSampleIndex() / emitter->getSampleRate();
	unsigned int numSeconds = emitter->getSound()->getNumSamples() / emitter->getSound()->getSampleRate();
	
	unsigned int currentMinute = currentSecond / 60;
	unsigned int numMinutes = numSeconds / 60;

	currentSecond %= 60;
	numSeconds %= 60;

	char tmp[16];
	snprintf(tmp, sizeof(tmp), "%02u:%02u / %02u:%02u", currentMinute, currentSecond, numMinutes, numSeconds);
	[positionLabel setStringValue:[NSString stringWithUTF8String:tmp]];

}

- (IBAction)seek:(NSObject *)sender
{
	if(!emitter)
		return;
		
	emitter->setSampleIndex([positionSlider integerValue]);
}

- (IBAction)play:(NSObject *)sender
{
	if(!emitter)
		return;

	context->attachSoundEmitter(emitter);
	
	[playButton setEnabled:NO];
	[stopButton setEnabled:YES];
}

- (IBAction)stop:(NSObject *)sender
{
	if(!emitter)
		return;

	context->detachSoundEmitter(emitter);
	emitter->setSampleIndex(0);
	
	[playButton setEnabled:YES];
	[stopButton setEnabled:NO];
}

- (IBAction)showEchoWindow:(NSObject *)sender
{
	[echoWindow makeKeyAndOrderFront:self];
}

- (IBAction)echoDelayChanged:(NSObject *)sender
{
	if(!echoEffect)
		return;
		
	NSStepper *stepper = (NSStepper *)sender;
	echoEffect->setDelay([stepper floatValue]);
	[echoDelayTextField setStringValue:[stepper stringValue]];
}

- (IBAction)echoFactorChanged:(NSObject *)sender
{
	if(!echoEffect)
		return;
		
	NSSlider *slider = (NSSlider *)sender;
	echoEffect->setFactor([slider floatValue]);
}

- (IBAction)echoCountChanged:(NSObject *)sender
{
	if(!echoEffect)
		return;
		
	NSStepper *stepper = (NSStepper *)sender;
	echoEffect->setCount([stepper intValue]);
	[echoCountTextField setStringValue:[stepper stringValue]];
}

- (void)playFile:(const char *)filePath
{
	[self stop:nil];
	
	// load file
	SoundPtr sound = Sound::create(filePath);
	
	// create echo effect
	echoEffect = EchoSoundEffect::create(sound, 0.5f, 0.5f, 0);
	[echoDelayStepper setFloatValue:echoEffect->getDelay()];
	[echoDelayTextField setFloatValue:echoEffect->getDelay()];
	[echoFactorSlider setFloatValue:echoEffect->getFactor()];
	[echoCountStepper setIntValue:echoEffect->getCount()];
	[echoCountTextField setIntValue:echoEffect->getCount()];

	// create sound emitter
	emitter = context->playSound(echoEffect);
	
	// set control states
	[playButton setEnabled:NO];
	[stopButton setEnabled:YES];
	[positionSlider setEnabled:YES];
	[positionSlider setMaxValue:(double)sound->getNumSamples()];
	[positionSlider setIntegerValue:0];
}

- (IBAction)openFile:(NSObject *)sender
{
	NSArray *fileTypes = [NSArray arrayWithObjects:@"aac", @"aif", @"aifc", @"aiff", @"m4a", @"mp3", @"wav", nil];
	
	NSOpenPanel *panel = [NSOpenPanel openPanel];
	[panel setCanChooseFiles:YES];
	[panel setCanChooseDirectories:NO];
	[panel setAllowedFileTypes:fileTypes];
	[panel beginSheetModalForWindow:window completionHandler:^(NSInteger result) {
		if(result) {
			// play file
			const char *filePath = [[[panel URL] path] UTF8String];
			[self playFile:filePath];
		}
	}];
}

@end
