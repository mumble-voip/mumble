// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// CMake only compiles this file on Apple platforms, so use __APPLE__ (compiler-native)
// rather than Q_OS_MAC (Qt-defined) which requires a Qt header to be included first.
#if defined(__APPLE__) && defined(USE_SCREEN_SHARING)

#	import <Foundation/Foundation.h>
#	import <ScreenCaptureKit/ScreenCaptureKit.h>

#	include "SCKitCapture.h"

#	include <QtGui/QImage>

#	include <algorithm>
#	include <cmath>

static constexpr int SCKIT_FPS = 15;

// ---------------------------------------------------------------------------
// Objective-C delegate — implements picker + stream + output protocols.
// Instantiated by sckit_startWithNativePicker and released by sckit_stop.
// ---------------------------------------------------------------------------
@interface SCKitDelegate
	: NSObject< SCContentSharingPickerObserver, SCStreamDelegate, SCStreamOutput >

@property(nonatomic, strong) SCStream *stream;
@property(nonatomic, copy) void (^onStarted)();
@property(nonatomic, copy) void (^onCancelled)();
@property(nonatomic, copy) void (^onError)(NSString *);
@property(nonatomic, copy) void (^onFrame)(QImage);
@property(nonatomic, assign) CGSize lastContentSize;

- (void)startStreamWithFilter:(SCContentFilter *)filter;

@end

@implementation SCKitDelegate

// This file is built without ARC, so the properties have to be released here.
- (void)dealloc {
	[_stream release];
	[_onStarted release];
	[_onCancelled release];
	[_onError release];
	[_onFrame release];
	[super dealloc];
}

/// Called when the user picks a source or changes their selection.
- (void)contentSharingPicker:(SCContentSharingPicker *)picker
		  didUpdateWithFilter:(SCContentFilter *)filter
					forStream:(SCStream *)stream {
	(void) picker;
	(void) stream;
	if (self.stream) {
		// Update an already-running stream (user switched source mid-session).
		// Push both the new content filter and a matching configuration so the
		// output resolution is correct for the newly selected source.
		SCStreamConfiguration *config = [self configForFilter:filter];
		[self.stream updateContentFilter:filter completionHandler:nil];
		[self.stream updateConfiguration:config completionHandler:nil];
	} else {
		[self startStreamWithFilter:filter];
	}
}

/// Called when the user dismisses the picker without choosing anything.
/// When the picker was shown without a pre-existing stream, `stream` is nil.
- (void)contentSharingPicker:(SCContentSharingPicker *)picker
		didCancelForStream:(SCStream *)stream {
	(void) picker;
	(void) stream;
	if (!self.stream && self.onCancelled) {
		dispatch_async(dispatch_get_main_queue(), self.onCancelled);
	}
}

/// Called when presenting the picker fails at the system level.
- (void)contentSharingPickerStartDidFailWithError:(NSError *)error {
	NSString *msg = error.localizedDescription;
	if (self.onError) {
		dispatch_async(dispatch_get_main_queue(), ^{
			self.onError(msg);
		});
	}
}

/// Builds an SCStreamConfiguration for an explicit pixel size.
/// If size is zero (unknown), falls back to 1920×1080.
/// Both dimensions are rounded down to an even number (required by YUV420P).
- (SCStreamConfiguration *)configForSize:(CGSize)size {
	if (CGSizeEqualToSize(size, CGSizeZero))
		size = CGSizeMake(1920, 1080);
	// Round down to even dimensions.
	size.width  = std::floor(size.width / 2) * 2;
	size.height = std::floor(size.height / 2) * 2;

	SCStreamConfiguration *config = [[SCStreamConfiguration alloc] init];
	config.pixelFormat            = kCVPixelFormatType_32BGRA;
	config.minimumFrameInterval   = CMTimeMake(1, SCKIT_FPS);
	config.showsCursor            = YES;
	config.width                  = static_cast< size_t >(size.width);
	config.height                 = static_cast< size_t >(size.height);
	return [config autorelease];
}

/// Builds an SCStreamConfiguration sized to the given filter's content rect.
/// Falls back to 1920×1080 for display-level or desktop-independent filters
/// where contentRect is zero or infinite — the actual window size is discovered
/// on the first frame via SCStreamFrameInfoContentRect and the config is updated.
- (SCStreamConfiguration *)configForFilter:(SCContentFilter *)filter {
	CGSize contentSize = filter.contentRect.size;
	// CGRectInfinite is returned for desktopIndependentWindow filters; treat as unknown.
	if (CGSizeEqualToSize(contentSize, CGRectInfinite.size))
		contentSize = CGSizeZero;
	return [self configForSize:contentSize];
}

/// Creates and starts an SCStream for the given filter.
- (void)startStreamWithFilter:(SCContentFilter *)filter {
	SCStreamConfiguration *config = [self configForFilter:filter];

	NSError *addError = nil;
	dispatch_queue_t queue = dispatch_queue_create("info.mumble.screenshare.sckit", DISPATCH_QUEUE_SERIAL);

	SCStream *stream = [[SCStream alloc] initWithFilter:filter configuration:config delegate:self];
	self.stream      = stream;
	[stream release];
	[self.stream addStreamOutput:self type:SCStreamOutputTypeScreen sampleHandlerQueue:queue error:&addError];
	// The stream keeps the queue for as long as it needs it
	dispatch_release(queue);
	if (addError) {
		NSString *msg = addError.localizedDescription;
		if (self.onError)
			dispatch_async(dispatch_get_main_queue(), ^{
				self.onError(msg);
			});
		self.stream = nil;
		return;
	}

	[self.stream startCaptureWithCompletionHandler:^(NSError *startError) {
		if (startError) {
			NSString *msg = startError.localizedDescription;
			if (self.onError)
				dispatch_async(dispatch_get_main_queue(), ^{
					self.onError(msg);
				});
			self.stream = nil;
		} else {
			if (self.onStarted)
				dispatch_async(dispatch_get_main_queue(), self.onStarted);
		}
	}];
}

// ---------------------------------------------------------------------------
// SCStreamOutput — frame delivery
// ---------------------------------------------------------------------------
- (void)stream:(SCStream *)stream
		didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
					   ofType:(SCStreamOutputType)type {
	(void) stream;
	if (type != SCStreamOutputTypeScreen)
		return;

	CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
	if (!pixelBuffer)
		return;

	CVPixelBufferLockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);

	CGRect rect;
	int width            = static_cast< int >(CVPixelBufferGetWidth(pixelBuffer));
	int height           = static_cast< int >(CVPixelBufferGetHeight(pixelBuffer));
	const int stride     = static_cast< int >(CVPixelBufferGetBytesPerRow(pixelBuffer));
	const uchar *basePtr = static_cast< const uchar * >(CVPixelBufferGetBaseAddress(pixelBuffer));

	// The content may only fill part of the buffer (e.g. when a window got smaller). Its rectangle is given in
	// points, so it has to be scaled to pixels with the frame's scale factor, which is 1 on non-Retina displays.
	CFArrayRef attachments = CMSampleBufferGetSampleAttachmentsArray(sampleBuffer, false);
	if (attachments && CFArrayGetCount(attachments) > 0) {
		CFDictionaryRef attachment = (CFDictionaryRef) CFArrayGetValueAtIndex(attachments, 0);
		CFDictionaryRef contentRect =
			(CFDictionaryRef) CFDictionaryGetValue(attachment, (__bridge const void *) SCStreamFrameInfoContentRect);
		CFNumberRef scaleFactor =
			(CFNumberRef) CFDictionaryGetValue(attachment, (__bridge const void *) SCStreamFrameInfoScaleFactor);

		CGFloat scale = 1;
		if (scaleFactor)
			CFNumberGetValue(scaleFactor, kCFNumberCGFloatType, &scale);

		if (contentRect && CGRectMakeWithDictionaryRepresentation(contentRect, &rect)) {
			// Never read beyond the buffer, whatever the frame info says
			width  = std::min(width, static_cast< int >(std::round(rect.size.width * scale))) & ~1;
			height = std::min(height, static_cast< int >(std::round(rect.size.height * scale))) & ~1;
		}
	}

	// kCVPixelFormatType_32BGRA on little-endian (Apple Silicon / Intel macOS)
	// maps to QImage::Format_ARGB32 (stored as 0xAARRGGBB in memory as BB GG RR AA).
	QImage frame(basePtr, width, height, stride, QImage::Format_ARGB32);
	// Deep-copy before unlocking the pixel buffer.
	QImage copy = frame.copy().convertToFormat(QImage::Format_RGBA8888);

	CVPixelBufferUnlockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);

	dispatch_async(dispatch_get_main_queue(), ^{
		if (self.onFrame)
			self.onFrame(copy);
	});
}

// ---------------------------------------------------------------------------
// SCStreamDelegate — error handling
// ---------------------------------------------------------------------------
- (void)stream:(SCStream *)stream didStopWithError:(NSError *)error {
	(void) stream;
	if (error && self.onError) {
		NSString *msg = error.localizedDescription;
		dispatch_async(dispatch_get_main_queue(), ^{
			self.onError(msg);
		});
	}
}

@end

// ---------------------------------------------------------------------------
// Global session — only one SCKit session active at a time.
// ---------------------------------------------------------------------------
static SCKitDelegate *g_delegate = nil;

// ---------------------------------------------------------------------------
// C++ interface (sckit_stop declared first so sckit_startWithNativePicker can call it)
// ---------------------------------------------------------------------------

void sckit_stop();

void sckit_startWithNativePicker(std::function< void() > onStarted,
								 std::function< void() > onCancelled,
								 std::function< void(QString) > onError,
								 std::function< void(QImage) > onFrame) {
	sckit_stop(); // Clean up any previous session.

	SCKitDelegate *delegate = [[SCKitDelegate alloc] init];
	delegate.onStarted      = [onStarted]() { onStarted(); };
	delegate.onCancelled    = [onCancelled]() { onCancelled(); };
	delegate.onError        = [onError](NSString *msg) {
		onError(QString::fromNSString(msg));
	};
	delegate.onFrame        = [onFrame](QImage img) { onFrame(img); };
	g_delegate              = delegate;

	SCContentSharingPicker *picker = SCContentSharingPicker.sharedPicker;
	[picker addObserver:delegate];
	picker.active = YES;

	// Request screen recording permission first; this also populates SCShareableContent
	// which the picker needs internally.  Only then show the picker.
	[SCShareableContent getShareableContentWithCompletionHandler:^(SCShareableContent *content,
																   NSError *permError) {
		(void) content;
		dispatch_async(dispatch_get_main_queue(), ^{
			if (permError) {
				onError(QString::fromNSString(permError.localizedDescription));
				return;
			}
			[SCContentSharingPicker.sharedPicker present];
		});
	}];
}

void sckit_stop() {
	if (g_delegate) {
		SCKitDelegate *delegate = g_delegate;
		g_delegate              = nil;

		SCContentSharingPicker *picker = SCContentSharingPicker.sharedPicker;
		[picker removeObserver:delegate];
		picker.active = NO;

		if (delegate.stream) {
			// The stream may deliver frames to the delegate until it has stopped, so only release the
			// delegate (and with it the stream) then.
			[delegate.stream stopCaptureWithCompletionHandler:^(NSError *error) {
				(void) error;
				[delegate release];
			}];
		} else {
			[delegate release];
		}
	}
}

#endif // __APPLE__ && USE_SCREEN_SHARING
