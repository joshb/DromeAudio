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

#import <Cocoa/Cocoa.h>
#import <DromeAudio/DromeAudio>

@interface DromeAudioPlayerCocoaAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
	NSButton *playButton;
	NSButton *stopButton;
	NSSlider *positionSlider;
	NSTextField *positionLabel;
	NSTimer *timer;
	
	NSWindow *echoWindow;
	NSTextField *echoDelayTextField;
	NSStepper *echoDelayStepper;
	NSSlider *echoFactorSlider;
	NSTextField *echoCountTextField;
	NSStepper *echoCountStepper;
	
	DromeAudio::AudioDriver *driver;
	DromeAudio::AudioContext *context;
	DromeAudio::SoundEmitterPtr emitter;
	DromeAudio::EchoSoundEffectPtr echoEffect;
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSButton *playButton;
@property (assign) IBOutlet NSButton *stopButton;
@property (assign) IBOutlet NSSlider *positionSlider;
@property (assign) IBOutlet NSTextField *positionLabel;
@property (assign) IBOutlet NSWindow *echoWindow;
@property (assign) IBOutlet NSTextField *echoDelayTextField;
@property (assign) IBOutlet NSStepper *echoDelayStepper;
@property (assign) IBOutlet NSSlider *echoFactorSlider;
@property (assign) IBOutlet NSTextField *echoCountTextField;
@property (assign) IBOutlet NSStepper *echoCountStepper;

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender;
- (void)timerFireMethod:(NSTimer *)theTimer;
- (IBAction)seek:(NSObject *)sender;
- (IBAction)play:(NSObject *)sender;
- (IBAction)stop:(NSObject *)sender;
- (IBAction)showEchoWindow:(NSObject *)sender;
- (IBAction)echoDelayChanged:(NSObject *)sender;
- (IBAction)echoFactorChanged:(NSObject *)sender;
- (IBAction)echoCountChanged:(NSObject *)sender;

- (void)playFile:(const char *)filePath;

- (IBAction)openFile:(NSObject *)sender;

@end
