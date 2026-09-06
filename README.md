![Mumble screenshot](screenshots/Mumble.png)

# Mumble - Open Source voice-chat software

[![https://www.mumble.info](https://img.shields.io/badge/Website-https%3A%2F%2Fwww.mumble.info-blue?style=for-the-badge)](https://www.mumble.info)

[![#mumble:matrix.org](https://img.shields.io/matrix/mumble:matrix.org?label=%23mumble:matrix.org&style=for-the-badge)](https://matrix.to/#/#mumble:matrix.org)

[![Codacy](https://img.shields.io/codacy/grade/262a5e20c83a40599050e22e700d8a3e?label=Codacy&style=for-the-badge)](https://app.codacy.com/manual/mumble-voip/mumble)
[![Azure](https://img.shields.io/azure-devops/build/Mumble-VoIP/c819eb06-7b22-4ef3-bbcd-860094454eb3/1?label=Azure&style=for-the-badge)](https://dev.azure.com/Mumble-VoIP/Mumble)
[![Cirrus CI](https://img.shields.io/cirrus/github/mumble-voip/mumble?label=Cirrus%20CI&style=for-the-badge)](https://cirrus-ci.com/github/mumble-voip/mumble)
[![Travis CI](https://img.shields.io/travis/com/mumble-voip/mumble?label=Travis%20CI&style=for-the-badge)](https://travis-ci.com/mumble-voip/mumble)

Mumble is an Open Source, low-latency and high-quality voice-chat program
written on top of Qt and Opus.

There are two modules in Mumble; the client (mumble) and the server (mumble-server formerly known as murmur).
The client works on Windows, Linux, FreeBSD, OpenBSD, and macOS,
while the server should work on anything Qt can be installed on.

The documentation of the project can be found on [the website](https://www.mumble.info/documentation/).


## Contributing

We always welcome contributions to the project. If you have some code that you would like to contribute, please go ahead and create a PR. While doing so,
please try to make sure that you follow our [commit guidelines](COMMIT_GUIDELINES.md).

If you are new to the Mumble project, you may want to check out the general [introduction to the Mumble source code](docs/dev/TheMumbleSourceCode.md).

### Translating

Mumble supports various languages. We are always looking for qualified people to contribute translations.

We are using Weblate as a translation platform. [Register on Weblate](https://hosted.weblate.org/accounts/register/), and join [our translation project](https://hosted.weblate.org/projects/mumble/).

### Writing plugins

Mumble supports general-purpose plugins that can provide functionality that is not implemented in the main Mumble application. You can find more
information on how this works and how these have to be created in the [plugin documentation](docs/dev/plugins/README.md).

## Building

For information on how to build Mumble, check out [the dedicated documentation](docs/dev/build-instructions/README.md).

Make sure to switch to the appropriate branch in this repository to get the correct build documentation. The current ``master`` branch contains
the unstable code for a future release of Mumble. If you want to build an already released stable version of Mumble, e.g. ``1.5.735``, select the
corresponding branch, e.g. ``1.5.x``, in the dropdown menu above. Alternatively, use the documentation in the respective release tarball.


## Reporting issues

If you want to report a bug or create a feature request, you can open a new issue (after you have checked that there is none already) on
[GitHub](https://github.com/mumble-voip/mumble/issues/new/choose).


## Code Signing

We graciously acknowledge that this program uses free code signing provided by
[SignPath.io](https://signpath.io?utm_source=foundation&utm_medium=github&utm_campaign=mumble), and a free code signing certificate by the
[SignPath Foundation](https://signpath.org?utm_source=foundation&utm_medium=github&utm_campaign=mumble).

## Windows

### Running Mumble

After installation, you should have a new Mumble folder in your
Start Menu, from which you can start Mumble.

### Running Mumble-Server

Double-click the icon to start ``mumble-server``. There will be a small icon on your
taskbar from which you can view the log.

To set the superuser password, run ``mumble-server`` with the parameters `--set-su-pw <password>`.


## MacOS

### Running Mumble

To install Mumble, drag the application from the downloaded
disk image into your `/Applications` folder.

### Running Mumble-Server

``mumble-server`` is distributed separately from the Mumble client on MacOS.
It is called Static OS X Server and can be downloaded from the main webpage.

Once downloaded it can be run in the same way as on any other Unix-like system.
For more information please see the "Running mumble-server" in the Linux/Unix section below.


## Linux/Unix

### Running Mumble

If you have installed Mumble through your distribution package
repository, you should be able to find Mumble in your start menu. No
additional steps are necessary.

### Running Mumble-Server

``mumble-server`` should be run from the command line, so start a shell (command prompt)
and go to wherever you installed Mumble. Run ``mumble-server`` as

```
mumble-server [--set-su-pw <password>] [--ini <inifile>] [--foreground] [--verbose]

--set-su-pw     Set a new password for the user SuperUser, which is hardcoded to
                bypass ACLs. Keep this password safe. Until you set a password,
                the SuperUser is disabled. If you use this option, mumble-server will
                set the password in the database and then exit.

--ini           Use an inifile other than mumble-server.ini, use this to run several instances
                of mumble-server from the same directory. Make sure each instance is using
                a separate database.

--foreground    Run in the foreground, logging to standard output.

--verbose       More verbose logging.
```

#### Docker image

Our official Docker image along with instructions on how to use it can be found at https://github.com/mumble-voip/mumble-docker


### Server configuration

You can find an up-to-date ``mumble-server`` configuration template in [this repository](auxiliary_files/mumble-server.ini).
Further server configuration documentation can be found [on the project website](https://www.mumble.info/documentation/administration/).

### OpenGL Overlay

The OpenGL overlay works by intercepting the call to switch buffers, and just
before the buffer switch, we draw our nice GUI.

To load a game with the overlay enabled, start the game like this:
```bash
LD_PRELOAD=/path/to/libmumble.so.1.1 gamename
```

If you have Mumble installed through the binary packages, this can be done by
simply typing:
```bash
mumble-overlay gamename
```

## Sponsors

[![Powered by DartNode](https://dartnode.com/branding/DN-Open-Source-sm.png)](https://dartnode.com "Powered by DartNode - Free VPS for Open Source")

<!-- Verified on DartNode: DNOS-A23FFD68 -->


## 🌐 Web Resources & Interactive Index
- [GET A COOL GUN](https://studyquesthub.web.app/get-a-cool-gun.html)
- [MERGE PIXEL](https://theskillquest.pages.dev/merge-pixel.html)
- [BRAINROT HOLE](https://studyquests.github.io/brainrot-hole.html)
- [HAPPY TOWN](https://quizverses.github.io/happy-town.html)
- [NOOB FUN FISHING](https://quizverses-9d2f2.web.app/noob-fun-fishing.html)
- [BATTLE ISLAND 2](https://studyquests.github.io/battle-island-2.html)
- [CATEGORY UNBLOCKER](https://quizverses.pages.dev/category-unblocker.html)
- [ELLIE AND BEN CHRISTMAS EVE](https://quizverses.github.io/ellie-and-ben-christmas-eve.html)
- [CATEGORY RUNNING](https://quizverses-9d2f2.web.app/category-running.html)
- [CATEGORY PIXEL313](https://quizverses-9d2f2.web.app/category-pixel313.html)
- [WILD TANKS](https://studyquests.github.io/wild-tanks.html)
- [BANK ROBBERY 3](https://studyquests.github.io/bank-robbery-3.html)
- [DRIVE TO SURVIVE](https://studyquests.github.io/drive-to-survive.html)
- [CATEGORY SKILL256](https://quizverses-9d2f2.web.app/category-skill256.html)
- [TILE HEXA SORT](https://studyquests.github.io/tile-hexa-sort.html)
- [CONTACT](https://thelearnquester.web.app/contact.html)
- [FRUIT JAM MERGE PUZZLE GAME](https://learnquester.github.io/fruit-jam-merge-puzzle-game.html)
- [CATEGORY CUTE](https://studyquests.github.io/category-cute.html)
- [SLENDER BOY ESCAPE ROBBIE](https://studyquests.github.io/slender-boy-escape-robbie.html)
- [ARCHERS RANDOM](https://quizverses-9d2f2.web.app/archers-random.html)
- [SORT MASTER](https://learnquester.github.io/sort-master.html)
- [KNOCK AND RUN 100 DOORS ESCAPE](https://studyquests.github.io/knock-and-run-100-doors-escape.html)
- [CATEGORY POOL](https://quizverses-9d2f2.web.app/category-pool.html)
- [CATEGORY DRESS UP 2](https://thelearnquester.web.app/category-dress-up-2.html)
- [SISYPHUS SIMULATOR](https://learnquester.github.io/sisyphus-simulator.html)
- [FARM MERGE HARVEST](https://learnquester.github.io/farm-merge-harvest.html)
- [CATEGORY SIMULATION 2](https://quizverses-9d2f2.web.app/category-simulation-2.html)
- [PONGOAL](https://learnquester.github.io/pongoal.html)
- [SOPHIES FARM](https://quizverses-9d2f2.web.app/sophies-farm.html)
- [CATEGORY MERGE](https://thelearnquester.web.app/category-merge.html)
- [VAULT BREAKER](https://learnquester.github.io/vault-breaker.html)
- [MERMAID PRINCESS AVATER CASTLE](https://learnquester.github.io/mermaid-princess-avater-castle.html)
- [PUZZLE TRAILS](https://learnquester.github.io/puzzle-trails.html)
- [CATEGORY FASHION105](https://thelearnquester.web.app/category-fashion105.html)
- [HOTEL FEVER TYCOON](https://learnquester.github.io/hotel-fever-tycoon.html)
- [CATEGORY HAPARA](https://thelearnquester.web.app/category-hapara.html)
- [SAMURAI MADNESS](https://learnquester.github.io/samurai-madness.html)
- [CATEGORY HUB](https://thelearnquester.web.app/category-hub.html)
- [PALM ISLAND SOLITAIRE](https://learnquester.github.io/palm-island-solitaire.html)
- [CATEGORY ESCAPE](https://studyquests.github.io/category-escape.html)
- [CATEGORY MAKEUP](https://thelearnquester.web.app/category-makeup.html)
- [TAP AWAY BLOCK PUZZLE 3D](https://studyquests.github.io/tap-away-block-puzzle-3d.html)
- [MEGA SHARK](https://learnquester.github.io/mega-shark.html)
- [CATEGORY CAR 2](https://quizverses.github.io/category-car-2.html)
- [CELEBRITY FACE DANCE](https://quizverses-9d2f2.web.app/celebrity-face-dance.html)
- [CATEGORY COOKING46](https://studyquests.github.io/category-cooking46.html)
- [PIPE CONNECT](https://quizverses-9d2f2.web.app/pipe-connect.html)
- [HERO RAGDOLL FIGHTING](https://learnquester.github.io/hero-ragdoll-fighting.html)
- [CATEGORY GROW GAMES](https://quizverses.github.io/category-grow-games.html)
- [CATEGORY GITHUB IO](https://studyquests.github.io/category-github-io.html)
- [TOILET ROLL](https://learnquester.github.io/toilet-roll.html)
- [LITTLE CANDY BAKERY](https://learnquester.github.io/little-candy-bakery.html)
- [PULL THE PINS](https://quizverses.pages.dev/pull-the-pins.html)
- [FPS TOY REALISM](https://quizverses-9d2f2.web.app/fps-toy-realism.html)
- [PYRAMID JEWELS](https://quizverses-9d2f2.web.app/pyramid-jewels.html)
- [JETSTREAM ESCAPE](https://learnquester.github.io/jetstream-escape.html)
- [WORD STARS](https://learnquester.github.io/word-stars.html)
- [ANTS PARTY](https://learnquester.github.io/ants-party.html)
- [4 HEXA](https://quizverses-9d2f2.web.app/4-hexa.html)
- [CATEGORY MERGE GAME](https://thelearnquester.web.app/category-merge-game.html)
- [REAL FREEKICK 3D](https://learnquester.github.io/real-freekick-3d.html)
- [CATEGORY DEFENSE176](https://quizverses.github.io/category-defense176.html)
- [TANKS](https://learnquester.github.io/tanks.html)
- [CATEGORY SHOOTER 2](https://thelearnquester.web.app/category-shooter-2.html)
- [CONNECT IMAGE](https://learnquester.github.io/connect-image.html)
- [HEXA PUZZLE MASTER](https://quizverses.github.io/hexa-puzzle-master.html)
- [CATEGORY SIMULATION 2](https://thelearnquester.web.app/category-simulation-2.html)
- [CATEGORY JIGSAW](https://studyquests.github.io/category-jigsaw.html)
- [CATEGORY CASUAL971](https://quizverses.github.io/category-casual971.html)
- [REAL CAR PARKING AND STUNT](https://learnquester.github.io/real-car-parking-and-stunt.html)
- [INDEX7](https://thelearnquester.web.app/index7.html)
- [HYPER CARS RAMP CRASH](https://learnquester.github.io/hyper-cars-ramp-crash.html)
- [CATEGORY BUBBLE SHOOTER](https://thelearnquester.web.app/category-bubble-shooter.html)
- [CATEGORY ADVENTURE 3](https://studyquests.github.io/category-adventure-3.html)
- [CATEGORY MANAGEMENT210](https://quizverses.github.io/category-management210.html)
- [OFFROAD JEEP GAME SIMULATOR](https://learnquester.github.io/offroad-jeep-game-simulator.html)
- [SANTA VS SKRITCH](https://learnquester.github.io/santa-vs-skritch.html)
- [CATEGORY CASUAL 3](https://quizverses.github.io/category-casual-3.html)
- [HIDDEN OBJECTS BAKERY](https://learnquester.github.io/hidden-objects-bakery.html)
- [MARBLE BUBBLE LEGEND](https://learnquester.github.io/marble-bubble-legend.html)
- [OBBY CHAMPIONS](https://learnquester.github.io/obby-champions.html)
- [HIDDEN OBJECTS ISLAND SECRETS](https://quizverses.github.io/hidden-objects-island-secrets.html)
- [DOMINO ADVENTURE](https://studyquests.github.io/domino-adventure.html)
- [CATEGORY BYEPASSHUB](https://thelearnquester.web.app/category-byepasshub.html)
- [CATEGORY HORROR90](https://studyquests.github.io/category-horror90.html)
- [CATEGORY SOCCER60](https://thelearnquester.web.app/category-soccer60.html)
- [CATEGORY MERGE224](https://thelearnquester.web.app/category-merge224.html)
- [BMG CRASHDAY 2025](https://learnquester.github.io/bmg-crashday-2025.html)
- [CATEGORY STRATEGY 2](https://quizverses.github.io/category-strategy-2.html)
- [WORD SEARCH WITH HINTS](https://studyquests.github.io/word-search-with-hints.html)
- [CINEMA EMPIRE IDLE TYCOON](https://studyquests.github.io/cinema-empire-idle-tycoon.html)
- [CATEGORY SECURLY](https://thelearnquester.web.app/category-securly.html)
- [SOLITAIRE STORY TRIPEAKS 6](https://quizverses-9d2f2.web.app/solitaire-story-tripeaks-6.html)
- [BOMBAMAN 3D](https://learnquester.github.io/bombaman-3d.html)
- [CATEGORY OBSTACLE](https://quizverses-9d2f2.web.app/category-obstacle.html)
- [EMOJI DROP THEMES](https://learnquester.github.io/emoji-drop-themes.html)
- [CATEGORY CAR](https://quizverses.github.io/category-car.html)
- [INDEX36](https://studyquests.github.io/index36.html)
- [CATEGORY RAGDOLL57](https://studyquests.github.io/category-ragdoll57.html)
- [TILEMAN IO](https://studyplayings.pages.dev/tileman-io.html)
- [TOWER OF HELL OBBY BLOX](https://quizverses.pages.dev/tower-of-hell-obby-blox.html)
- [CATEGORY 1 PLAYER139](https://studyquests.github.io/category-1-player139.html)
- [BRAIN PUZZLE TRICKY CHOICES](https://learnquester.github.io/brain-puzzle-tricky-choices.html)
- [CATEGORY RACING DRIVING 2](https://studyquests.github.io/category-racing-driving-2.html)
- [SMASH DEFENSE](https://learnquester.github.io/smash-defense.html)
- [COLOR COCKTAIL](https://learnquester.github.io/color-cocktail.html)
- [CATEGORY PREMIUM PERKS71](https://thelearnquester.web.app/category-premium-perks71.html)
- [CATEGORY DRESS UP](https://learnquester.github.io/category-dress-up.html)
- [QUIZ 10 SECONDS MATH](https://studyquests.github.io/quiz-10-seconds-math.html)
- [CATEGORY PUZZLE 5](https://studyquests.github.io/category-puzzle-5.html)
- [WORLD SOLITAIRE TRIPEAKS ](https://learnquester.github.io/world-solitaire-tripeaks-.html)
- [PIXEL SHOOT](https://studyquests.github.io/pixel-shoot.html)
- [SOKOBAN PR](https://studyquests.github.io/sokoban-pr.html)
- [INDEX8](https://studyquests.github.io/index8.html)
- [BUBBLE AROUND](https://quizverses-9d2f2.web.app/bubble-around.html)
- [JUST DICE RANDOM TOWER DEFENCE](https://quizverses.github.io/just-dice-random-tower-defence.html)
- [CATEGORY MERGE224](https://quizverses.github.io/category-merge224.html)
- [CRUSH THE EGGS](https://learnquester.github.io/crush-the-eggs.html)
- [CATEGORY CUTE62](https://studyquests.github.io/category-cute62.html)
- [ARROW CUBE ESCAPE](https://studyquests.github.io/arrow-cube-escape.html)
- [AIDAN IN DANGER](https://learnquester.github.io/aidan-in-danger.html)
- [GEMSTONE GLAM](https://studyplayings.pages.dev/gemstone-glam.html)
- [CATEGORY INCREMENTAL388](https://thelearnquester.web.app/category-incremental388.html)
- [CATEGORY MINECRAFT](https://learnquester.github.io/category-minecraft.html)
- [CATEGORY CONTROLLER 2](https://thelearnquester.web.app/category-controller-2.html)
- [XYTRIAN RUNNER](https://quizverses-9d2f2.web.app/xytrian-runner.html)
- [BASKETBALL STARS 2026](https://quizverses.pages.dev/basketball-stars-2026.html)
- [ZOMBIE SURVIVAL](https://studyplayings.pages.dev/zombie-survival.html)
- [CATEGORY SANDBOX40](https://studyquests.github.io/category-sandbox40.html)
- [STEAL ITEMS IO](https://studyplaying.github.io/steal-items-io.html)
