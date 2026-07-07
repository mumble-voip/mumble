# mumble-tfar — Mumble со встроенной поддержкой TFAR (Task Force Arrowhead Radio)

Модифицированный клиент Mumble, являющийся **drop-in заменой связки TeamSpeak 3 + TFAR-плагин**
для Arma 3. Игрок запускает этот Mumble вместо TeamSpeak — и получает обычную голосовую связь
плюс полную симуляцию радио TFAR (дальность, стерео-каналы, эффекты радио, интерком, спикеры,
serious-режим и т.д.).

**RVExtension со стороны Arma 3 (`task_force_radio_win64.dll`) не изменялся** — используется
оригинальный мод TFAR как есть. Протокол обмена (shared memory / named pipe
`task_force_radio_pipe`) и все игровые команды поддержаны полностью.

---

## Архитектура

Оригинальный TS3-плагин TFAR (`ts/src`, ~11 тыс. строк) встроен в клиент Mumble
**практически без изменений** — заменён только слой взаимодействия с TeamSpeak:

```
src/mumble/tfar/
├── TeamspeakMumble.cpp   # реализация фасада Teamspeak (бывш. Teamspeak.cpp) поверх Mumble
├── TS3Compat.h           # внутренний интерфейс слоя совместимости
├── TFARBridge.{h,cpp}    # Qt-мост: сигналы Mumble → колбэки ts3plugin_*
├── TFARConfig.{h,cpp}    # страница "TFAR" в настройках Mumble
├── DebugUI.{h,cpp}       # заглушка GLFW-отладчика (замена — /tfar diag в чате)
├── tfar.cmake            # интеграция в сборку
├── radio-sounds/         # звуки радио (копируются рядом с mumble.exe)
├── include/              # TS3 SDK заголовки (типы), dspfilters, clunk, simpleSource
└── <остальные файлы>     # немодифицированное ядро TFAR (CommandProcessor, PlaybackHandler,
                          #   clientData, serverData, RadioEffect, pipe_handler, ...)
```

### Соответствие TS3 API → Mumble

| TeamSpeak 3                                  | Mumble                                                        |
| -------------------------------------------- | ------------------------------------------------------------- |
| `onEditPostProcessVoiceDataEvent` (per-user) | сигнал `AudioOutput::audioSourceFetched` (стерео 48 кГц)      |
| `onEditMixedPlaybackVoiceDataEvent`          | сигнал `AudioOutput::audioOutputAboutToPlay`                  |
| `onEditCapturedVoiceDataEvent`               | сигнал `AudioInput::audioInputEncountered`                    |
| `sendPluginCommand` / `onPluginCommandEvent` | `PluginDataTransmission` (dataID `"TFAR"`), сервер ≥ 1.4.0    |
| `CLIENT_META_DATA` (`<TFAR>...</TFAR>`)      | broadcast `"TFARST"` каждые 4 с + кэш на клиентах             |
| Переименование ника в игровое имя            | «виртуальный ник» в broadcast `"TFARST"` + локальный никнейм в UI |
| `requestMute/UnmuteClients` (локальный мьют) | `ClientUser::setLocalMute`                                    |
| `requestClientMove` (serious-канал)          | `ServerHandler::joinChannel` (пароль → access token)          |
| `CLIENT_INPUT_DEACTIVATED` / VAD (тангента)  | `API::PluginData::overwriteMicrophoneActivation`              |
| `playWaveFile`                               | `AudioOutput::playSample`                                     |
| ID каналов                                   | Mumble channel ID **+ 1** (0 = корневой канал, у TFAR 0 невалиден) |
| ID клиентов (`anyID`, 16 бит)                | session ID Mumble (расширено до 32 бит)                       |
| 3D-движок TS (`systemset3D*`)                | не нужен — TFAR сам делает спатиализацию (Clunk HRTF + X3DAudio) |

### Что важно знать

- **Сервер**: обычный Murmur/Mumble server **версии ≥ 1.4.0** (нужен `PluginDataTransmission`).
- Один сервер = TS `serverConnectionHandlerID` 1.
- `serious_channelName` вида `ID:<число>` означает **TFAR-ID канала** — то же значение, которое
  возвращается игре через `TS_INFO CHANNELID` и печатается в `/tfar diag`
  (= ID канала Mumble + 1). Семантика идентична оригинальному плагину: зашли в нужный
  канал → спросили CHANNELID → прописали в конфиг миссии.
- Порт «/tfar»-команд чата TS: `/tfar diag`, `/tfar pos`, `/tfar full`, `/tfar rstflt`.
- Метаданные/ники сходятся автоматически: состояние рассылается при подключении, смене
  канала, входе нового клиента и каждые 4 секунды сервисным потоком TFAR.

## Сборка (Windows)

Требования — как у обычного Mumble для Windows (MSVC, Qt6, vcpkg/зависимости), см.
`docs/dev/build-instructions`. TFAR включён по умолчанию (`-Dtfar=ON`, только Win32):

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -Dserver=OFF -Dtests=OFF
cmake --build build --target mumble
```

Дополнительно линкуются только системные библиотеки Windows (`wininet`, `x3daudio`).
После сборки рядом с `mumble.exe` автоматически появляется папка `tfar/radio-sounds`.

### CI / Релизы

- **CI** (`.github/workflows/build.yml`) — обычная матрица Mumble; Windows-сборка идёт
  с `-Dtfar=ON`, MSI-инсталлятор включает радио-звуки (`client/tfar/radio-sounds`).
- **Релизы** (`.github/workflows/release-tfar.yml`) — пуш тега `v*` собирает Windows-клиент
  и публикует GitHub-релиз с MSI-инсталлятором и портативным ZIP
  (`mumble.exe` + `tfar/` + DLL). Ручной запуск (workflow_dispatch) кладёт те же файлы
  в артефакты без публикации релиза.
- Проверки стиля (clang-format/CRLF) исключают `src/mumble/tfar/` — это перенесённый
  сторонний код с оригинальным форматированием.

## Использование

1. Установите обычный мод TFAR в Arma 3 (`@task_force_radio`) — как для TeamSpeak.
   TS3-плагин устанавливать **не нужно**.
2. Запустите наш `mumble.exe`, подключитесь к серверу Mumble (≥ 1.4).
3. Запустите Arma 3. При подключении игры в чате Mumble появится «TFAR: Arma 3 connected»,
   прозвучит знакомый сигнал, статус виден в «Настройки → TFAR».
4. Всё остальное — как в TS: автоперемещение в serious-канал, переименование в игровой ник
   (виртуальное), радио на тангентах из игры, эффекты дальности/стерео/интерком.

## Отличия от TS3-версии / ограничения

- Отладочное GL-окно (`/tfar debug`) не портировано — используйте `/tfar diag` и `/tfar pos`.
- Реальный ник в Mumble не меняется (протокол не позволяет) — игровое имя рассылается
  как «виртуальный ник», отображается локально у всех участников и используется всей
  логикой TFAR. Совместимость команд (`TANGENT`, `VOLUME`, ...) полная.
- Проверка обновлений плагина и piwik-телеметрия TS-версии отключены.
- Sample rate микшера Mumble желательно 48000 Гц (как в TS) — при других значениях
  фильтры радио будут слегка неточны.
- `/tfar full` пишет логи в `~/mumble-tfar-logs/` (вместо `%APPDATA%\TS3Client\logs`).
