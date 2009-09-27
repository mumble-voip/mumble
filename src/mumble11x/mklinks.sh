
for F in ASIOInput.h ASIOInput.cpp DirectSound.h DirectSound.cpp WASAPI.h WASAPI.cpp AudioStats.h AudioStats.cpp AudioStats.ui ConfigDialog.h ConfigDialog.cpp ConfigDialog.ui DBus.h GlobalShortcut_win.cpp GlobalShortcut_win.h licenses.h mumble_pch.hpp os_macx.cpp smallft.h smallft.cpp TextToSpeech.h TextToSpeech_win.cpp Usage.h Usage.cpp VersionCheck.h VersionCheck.cpp ViewCert.h ViewCert.cpp ConfigWidget.h ConfigWidget.cpp; do
  [ ! -f $F ] || rm $F
  ln ../mumble/$F
done
