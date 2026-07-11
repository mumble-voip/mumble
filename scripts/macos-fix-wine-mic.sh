#!/usr/bin/env bash
# MUMBLE-TFAR: grant Wine microphone access on macOS.
#
# Run this on the Mac itself (NOT inside Wine). It locates the Wine .app
# bundle, adds the NSMicrophoneUsageDescription key required by macOS TCC,
# re-signs the bundle (editing Info.plist invalidates the signature) and
# resets the cached Microphone permission so macOS shows the permission
# prompt on the next launch.
#
# See docs/MACOS-WINE-AUDIO.md for the full guide.

set -euo pipefail

USAGE_DESCRIPTION="Voice chat needs microphone access"

say() { printf '%s\n' "$*"; }
die() { printf 'ОШИБКА: %s\n' "$*" >&2; exit 1; }

[ "$(uname -s)" = "Darwin" ] || die "этот скрипт нужно запускать на macOS (не внутри Wine)."

# --- locate the wine binary ---------------------------------------------------
WINE_BIN="${WINE:-}"
if [ -z "$WINE_BIN" ]; then
    for candidate in wine wine64 wine-stable; do
        if command -v "$candidate" >/dev/null 2>&1; then
            WINE_BIN="$(command -v "$candidate")"
            break
        fi
    done
fi
[ -n "$WINE_BIN" ] || die "wine не найден в PATH. Укажите путь явно: WINE=/путь/к/wine $0"

# resolve symlinks (Homebrew installs a symlink into /opt/homebrew/bin)
while [ -L "$WINE_BIN" ]; do
    LINK_TARGET="$(readlink "$WINE_BIN")"
    case "$LINK_TARGET" in
        /*) WINE_BIN="$LINK_TARGET" ;;
        *)  WINE_BIN="$(dirname "$WINE_BIN")/$LINK_TARGET" ;;
    esac
done
say "Найден wine: $WINE_BIN"

# --- find the enclosing .app bundle -------------------------------------------
APP_BUNDLE=""
DIR="$(dirname "$WINE_BIN")"
while [ "$DIR" != "/" ]; do
    case "$DIR" in
        *.app) APP_BUNDLE="$DIR"; break ;;
    esac
    DIR="$(dirname "$DIR")"
done

if [ -z "$APP_BUNDLE" ]; then
    say ""
    say "wine установлен не как .app-бандл (например, formula из Homebrew)."
    say "В этом случае macOS спрашивает разрешение у приложения, из которого"
    say "вы запускаете wine (Terminal, iTerm и т.п.)."
    say ""
    say "Сделайте так:"
    say "  1. Системные настройки -> Конфиденциальность и безопасность -> Микрофон"
    say "  2. Включите Terminal (или iTerm)."
    say "  3. Если его нет в списке — выполните: tccutil reset Microphone"
    say "     и запустите клиент заново: macOS покажет запрос доступа."
    say "  4. Полностью перезапустите wine:  wineserver -k"
    exit 0
fi

PLIST="$APP_BUNDLE/Contents/Info.plist"
[ -f "$PLIST" ] || die "не найден $PLIST"
say "Бандл Wine: $APP_BUNDLE"

BUNDLE_ID="$(/usr/libexec/PlistBuddy -c 'Print :CFBundleIdentifier' "$PLIST" 2>/dev/null || true)"

# --- add NSMicrophoneUsageDescription if missing -------------------------------
if /usr/libexec/PlistBuddy -c 'Print :NSMicrophoneUsageDescription' "$PLIST" >/dev/null 2>&1; then
    say "NSMicrophoneUsageDescription уже есть в Info.plist — пропускаю."
else
    cp "$PLIST" "$PLIST.backup"
    say "Резервная копия: $PLIST.backup"
    /usr/libexec/PlistBuddy -c "Add :NSMicrophoneUsageDescription string \"$USAGE_DESCRIPTION\"" "$PLIST"
    say "Добавлен NSMicrophoneUsageDescription."

    # Editing Info.plist invalidates the code signature; re-sign ad hoc.
    say "Переподписываю бандл (ad-hoc)…"
    codesign --force --deep --sign - "$APP_BUNDLE" \
        || die "codesign не удался. Запустите вручную: codesign --force --deep --sign - \"$APP_BUNDLE\""
fi

# --- reset the cached TCC decision so macOS asks again -------------------------
if [ -n "$BUNDLE_ID" ]; then
    say "Сбрасываю кэш разрешения микрофона для $BUNDLE_ID…"
    tccutil reset Microphone "$BUNDLE_ID" >/dev/null 2>&1 \
        || say "(не удалось сбросить точечно — при необходимости выполните: tccutil reset Microphone)"
fi

say ""
say "Готово. Полностью перезапустите wine (wineserver -k) и запустите клиент."
say "macOS должна показать запрос доступа к микрофону — нажмите «Разрешить»."
say "Проверить/включить вручную: Системные настройки -> Конфиденциальность и"
say "безопасность -> Микрофон."
