// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_SCREEN_SHARING

#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>

#include "CaptureSourceLister.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtGui/QScreen>

static constexpr int THUMBNAIL_WIDTH = 160;
static constexpr int THUMBNAIL_HEIGHT = 90;

/// Convert a CoreGraphics image to a QImage using a CGBitmapContext.
/// The result uses Format_ARGB32_Premultiplied (BGRA / premultiplied alpha).
static QImage qImageFromCGImage(CGImageRef cgImage) {
  if (!cgImage)
    return {};

  const size_t width = CGImageGetWidth(cgImage);
  const size_t height = CGImageGetHeight(cgImage);

  QImage image(static_cast<int>(width), static_cast<int>(height),
               QImage::Format_ARGB32_Premultiplied);
  image.fill(Qt::transparent);

  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
  CGContextRef ctx = CGBitmapContextCreate(
      image.bits(), width, height,
      8, // bits per component
      static_cast<size_t>(image.bytesPerLine()), colorSpace,
      kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
  CGColorSpaceRelease(colorSpace);
  if (!ctx)
    return {};

  CGContextDrawImage(ctx,
                     CGRectMake(0, 0, static_cast<CGFloat>(width),
                                static_cast<CGFloat>(height)),
                     cgImage);
  CGContextRelease(ctx);
  return image;
}

static QPixmap pixmapFromCGImage(CGImageRef cgImage) {
  if (!cgImage)
    return {};
  QImage img = qImageFromCGImage(cgImage);
  if (img.isNull())
    return {};
  return QPixmap::fromImage(img.scaled(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation));
}

static QString cfStringToQString(CFStringRef str) {
  if (!str)
    return {};
  // Fast path: ASCII/UTF-8 buffer
  const char *cStr = CFStringGetCStringPtr(str, kCFStringEncodingUTF8);
  if (cStr)
    return QString::fromUtf8(cStr);
  // Slow path: allocate
  CFIndex length = CFStringGetLength(str);
  QByteArray buf(static_cast<int>(length * 4 + 1), '\0');
  if (CFStringGetCString(str, buf.data(), buf.size(), kCFStringEncodingUTF8))
    return QString::fromUtf8(buf.constData());
  return {};
}

QList<CaptureSource> listCaptureSources() {
  QList<CaptureSource> sources;

  // --- Screens ---
  const QList<QScreen *> screens = QGuiApplication::screens();
  for (int i = 0; i < screens.size(); ++i) {
    QScreen *screen = screens.at(i);
    CaptureSource s;
    s.type = CaptureSource::Type::EntireScreen;
    s.screenIndex = i;
    s.displayName = QObject::tr("Display %1 (%2×%3)")
                        .arg(i + 1)
                        .arg(screen->size().width())
                        .arg(screen->size().height());

    // Grab thumbnail using Qt — works for screens on macOS at picker time.
    QPixmap px = screen->grabWindow(0);
    if (!px.isNull()) {
      s.thumbnail = QPixmap::fromImage(
          px.toImage().scaled(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT,
                              Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    sources.append(s);
  }

  // --- Windows (CoreGraphics) ---
  CFArrayRef windowList = CGWindowListCopyWindowInfo(
      kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements,
      kCGNullWindowID);
  if (!windowList)
    return sources;

  CFIndex count = CFArrayGetCount(windowList);
  for (CFIndex i = 0; i < count; ++i) {
    auto *info =
        static_cast<CFDictionaryRef>(CFArrayGetValueAtIndex(windowList, i));
    if (!info)
      continue;

    // Only include normal application windows (layer 0).
    auto *layerRef =
        static_cast<CFNumberRef>(CFDictionaryGetValue(info, kCGWindowLayer));
    if (!layerRef)
      continue;
    int layer = 0;
    CFNumberGetValue(layerRef, kCFNumberIntType, &layer);
    if (layer != 0)
      continue;

    // Skip Window Server (system compositor) entries.
    auto *ownerNameRef = static_cast<CFStringRef>(
        CFDictionaryGetValue(info, kCGWindowOwnerName));
    const QString ownerName = cfStringToQString(ownerNameRef);
    if (ownerName == QLatin1String("Window Server"))
      continue;

    // Window ID.
    auto *windowNumRef =
        static_cast<CFNumberRef>(CFDictionaryGetValue(info, kCGWindowNumber));
    if (!windowNumRef)
      continue;
    uint32_t windowId = 0;
    CFNumberGetValue(windowNumRef, kCFNumberSInt32Type, &windowId);

    // Window name (may be empty for some apps).
    auto *windowNameRef =
        static_cast<CFStringRef>(CFDictionaryGetValue(info, kCGWindowName));
    const QString windowName = cfStringToQString(windowNameRef);

    // Display name: "AppName - WindowTitle" or just "AppName".
    QString displayName = ownerName;
    if (!windowName.isEmpty() && windowName != ownerName)
      displayName += QLatin1String(" - ") + windowName;

    // Thumbnail via CoreGraphics window capture.
    CGImageRef cgImg = CGWindowListCreateImage(
        CGRectNull, kCGWindowListOptionIncludingWindow,
        static_cast<CGWindowID>(windowId), kCGWindowImageDefault);
    QPixmap thumbnail = pixmapFromCGImage(cgImg);
    if (cgImg)
      CGImageRelease(cgImg);

    CaptureSource s;
    s.type = CaptureSource::Type::Window;
    s.nativeWindowId = static_cast<quintptr>(windowId);
    s.displayName = displayName;
    s.thumbnail = thumbnail;
    sources.append(s);
  }

  CFRelease(windowList);
  return sources;
}

QImage grabCaptureSource(const CaptureSource &source) {
  if (source.type == CaptureSource::Type::EntireScreen) {
    const QList<QScreen *> screens = QGuiApplication::screens();
    if (source.screenIndex < 0 || source.screenIndex >= screens.size())
      return {};
    QPixmap px = screens.at(source.screenIndex)->grabWindow(0);
    if (px.isNull())
      return {};
    return px.toImage().convertToFormat(QImage::Format_RGBA8888);
  }

  // Window capture — must use CoreGraphics on macOS.
  // Qt's QScreen::grabWindow(WId) does NOT capture specific windows on macOS
  // because WId is an NSView pointer, not a CGWindowID.
  CGImageRef cgImage = CGWindowListCreateImage(
      CGRectNull, kCGWindowListOptionIncludingWindow,
      static_cast<CGWindowID>(source.nativeWindowId), kCGWindowImageDefault);
  if (!cgImage)
    return {};

  QImage img = qImageFromCGImage(cgImage);
  CGImageRelease(cgImage);
  return img.convertToFormat(QImage::Format_RGBA8888);
}

#endif // USE_SCREEN_SHARING
