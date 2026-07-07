#!/usr/bin/env python3
# MUMBLE-TFAR
# Generates all Storm of The Galaxy branding assets from code (reproducible):
#   icons/mumble.ico, icons/mumble_256x256.png, installer/msix/Assets/*,
#   installer/bannrbmp.bmp, installer/dlgbmp.bmp
# Style follows the project artwork: white clone-trooper helmet with blue
# accents in front of a dark blue panelled wall.
#
# Usage: python3 scripts/generate-branding.py  (from the repository root)

from PIL import Image, ImageDraw, ImageFilter, ImageFont
import os

S = 1024  # master canvas size

NAVY_TOP = (13, 27, 58)
NAVY_BOTTOM = (7, 14, 32)
PANEL_LINE = (4, 9, 22)
PANEL_GLOW = (38, 90, 160)
WHITE = (236, 239, 244)
SHADE = (196, 202, 212)
DARKV = (24, 28, 38)       # visor
BLUE = (43, 92, 205)       # 501st accent
BLUE_D = (28, 60, 140)


def wall(draw, w, h):
    """Dark blue panelled wall like in the artwork."""
    for y in range(h):
        t = y / h
        color = tuple(int(NAVY_TOP[i] + (NAVY_BOTTOM[i] - NAVY_TOP[i]) * t) for i in range(3))
        draw.line([(0, y), (w, y)], fill=color)
    # vertical panels
    panel_width = w // 6
    for i in range(1, 6):
        x = i * panel_width
        draw.rectangle([x - w // 90, 0, x + w // 90, h], fill=PANEL_LINE)
    # one glowing panel (the teal-blue slat in the art)
    draw.rectangle([int(w * 0.70), 0, int(w * 0.79), int(h * 0.45)], fill=(23, 70, 110))
    draw.rectangle([int(w * 0.72), 0, int(w * 0.77), int(h * 0.42)], fill=PANEL_GLOW)


def helmet(draw, cx, cy, s):
    """Simplified clone-trooper helmet, front view.
    cx, cy — center of the helmet, s — helmet height."""
    w = s * 0.92

    def X(fx):
        return cx + fx * w

    def Y(fy):
        return cy + (fy - 0.5) * s

    # --- dome ---
    draw.ellipse([X(-0.5), Y(0.0), X(0.5), Y(0.62)], fill=WHITE, outline=SHADE, width=int(s * 0.012))
    # dome blue stripe (fin), 501st style
    draw.polygon([(X(-0.045), Y(0.0)), (X(0.045), Y(0.0)),
                  (X(0.03), Y(0.30)), (X(-0.03), Y(0.30))], fill=BLUE)
    draw.polygon([(X(-0.03), Y(0.02)), (X(0.03), Y(0.02)),
                  (X(0.02), Y(0.27)), (X(-0.02), Y(0.27))], fill=BLUE_D)

    # --- jaw / faceplate ---
    draw.polygon([
        (X(-0.47), Y(0.42)), (X(0.47), Y(0.42)),
        (X(0.40), Y(0.78)), (X(0.20), Y(1.0)),
        (X(-0.20), Y(1.0)), (X(-0.40), Y(0.78)),
    ], fill=WHITE, outline=SHADE)

    # --- T-visor ---
    # horizontal bar with a slight frown
    draw.polygon([
        (X(-0.37), Y(0.42)), (X(0.37), Y(0.42)),
        (X(0.31), Y(0.53)), (X(0.09), Y(0.49)),
        (X(-0.09), Y(0.49)), (X(-0.31), Y(0.53)),
    ], fill=DARKV)
    # vertical wedge
    draw.polygon([
        (X(-0.09), Y(0.49)), (X(0.09), Y(0.49)),
        (X(0.04), Y(0.86)), (X(-0.04), Y(0.86)),
    ], fill=DARKV)

    # --- cheek tick marks (blue), like in the art ---
    for side in (-1, 1):
        for k in range(3):
            fx = side * (0.17 + k * 0.075)
            draw.polygon([
                (X(fx), Y(0.62)), (X(fx + side * 0.035), Y(0.62)),
                (X(fx + side * 0.055), Y(0.72)), (X(fx + side * 0.02), Y(0.72)),
            ], fill=BLUE)

    # --- chin vents ---
    for side in (-1, 1):
        draw.polygon([
            (X(side * 0.10), Y(0.88)), (X(side * 0.19), Y(0.85)),
            (X(side * 0.17), Y(0.94)), (X(side * 0.09), Y(0.96)),
        ], fill=DARKV)

    # --- ear/aerator discs (blue) ---
    for side in (-1, 1):
        r = s * 0.05
        ex, ey = X(side * 0.435), Y(0.47)
        draw.ellipse([ex - r, ey - r, ex + r, ey + r], fill=BLUE, outline=BLUE_D, width=int(s * 0.01))


def emblem(size=S, rounded=True):
    img = Image.new('RGBA', (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    wall(draw, size, size)
    helmet(draw, size * 0.5, size * 0.54, size * 0.72)
    if rounded:
        mask = Image.new('L', (size, size), 0)
        ImageDraw.Draw(mask).rounded_rectangle([0, 0, size - 1, size - 1], radius=size // 7, fill=255)
        img.putalpha(mask)
    return img


def save_png(img, path, size):
    img.resize((size, size), Image.LANCZOS).save(path)
    print(path)


def find_font(size):
    for candidate in ("/System/Library/Fonts/Supplemental/Arial Bold.ttf",
                      "/System/Library/Fonts/Helvetica.ttc",
                      "C:/Windows/Fonts/arialbd.ttf",
                      "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"):
        if os.path.exists(candidate):
            return ImageFont.truetype(candidate, size)
    return ImageFont.load_default()


def banner(width, height, big):
    """Installer bitmaps: wall + helmet on the right + project name."""
    img = Image.new('RGB', (width, height), NAVY_BOTTOM)
    draw = ImageDraw.Draw(img)
    wall(draw, width, height)
    hs = int(height * (0.86 if not big else 0.55))
    helmet(draw, width - hs * 0.75, height * (0.55 if not big else 0.42), hs)
    if big:
        font = find_font(int(height * 0.085))
        small = find_font(int(height * 0.05))
        draw.text((int(width * 0.05), int(height * 0.62)), "MUMBLE TFAR", font=font, fill=WHITE)
        draw.text((int(width * 0.05), int(height * 0.74)), "STORM OF THE GALAXY", font=small, fill=BLUE)
    return img


def main():
    master = emblem()
    master_square = emblem(rounded=False)

    # preview + generic icon png
    save_png(master, 'icons/mumble_256x256.png', 256)
    save_png(master, 'scripts/branding-preview.png', 512)

    # windows .ico (multi-size)
    master.resize((256, 256), Image.LANCZOS).save(
        'icons/mumble.ico', sizes=[(16, 16), (24, 24), (32, 32), (48, 48), (64, 64), (128, 128), (256, 256)])
    print('icons/mumble.ico')

    # MSIX assets (square tiles use the non-rounded master — Windows rounds itself)
    save_png(master_square, 'installer/msix/Assets/Square150x150Logo.png', 150)
    save_png(master_square, 'installer/msix/Assets/Square310x310Logo.png', 310)
    save_png(master_square, 'installer/msix/Assets/Square44x44Logo.png', 44)
    save_png(master_square, 'installer/msix/Assets/StoreLogo.png', 50)
    wide = Image.new('RGBA', (310, 150), (0, 0, 0, 0))
    dw = ImageDraw.Draw(wide)
    wall(dw, 310, 150)
    helmet(dw, 155, 82, 108)
    wide.save('installer/msix/Assets/Wide310x150Logo.png')
    print('installer/msix/Assets/Wide310x150Logo.png')

    # installer bitmaps (must be 24bpp BMP)
    banner(493, 58, big=False).save('installer/bannrbmp.bmp')
    print('installer/bannrbmp.bmp')
    banner(493, 312, big=True).save('installer/dlgbmp.bmp')
    print('installer/dlgbmp.bmp')


if __name__ == '__main__':
    main()
