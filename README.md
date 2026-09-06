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
- [CATEGORY ROBOT49](https://themindzone.pages.dev/category-robot49.html)
- [ZOMBIE HIGHWAY RAMPAGE](https://quizverses.pages.dev/zombie-highway-rampage.html)
- [CATEGORY CARTOON](https://studyplayings.web.app/category-cartoon.html)
- [ITALIAN BRAINROT DRAG MERGE PUZZLE](https://studyplayings.web.app/italian-brainrot-drag-merge-puzzle.html)
- [AVATAR MASTER FIX UP FACE](https://studyplayings.web.app/avatar-master-fix-up-face.html)
- [INDEX21](https://studyquests.github.io/index21.html)
- [INDEX4](https://thelearnquester.web.app/index4.html)
- [CATEGORY MOUSE1 697](https://studyplayings.web.app/category-mouse1-697.html)
- [CONQUERIO](https://studyplayings.pages.dev/conquerio.html)
- [TICTOC URBAN OUTFITS](https://studyplaying.github.io/tictoc-urban-outfits.html)
- [SQUID GAME ORIGINAL](https://studyquests.github.io/squid-game-original.html)
- [FASHION WORLD SIMULATOR](https://studyplayings.web.app/fashion-world-simulator.html)
- [DARK STONES CARD BATTLE RPG](https://quizverses.pages.dev/dark-stones-card-battle-rpg.html)
- [CATEGORY BUSINESS135](https://learnquester.github.io/category-business135.html)
- [MY LITTLE FARM](https://quizverses-9d2f2.web.app/my-little-farm.html)
- [DREAM PET HOTEL](https://studyquests.pages.dev/dream-pet-hotel.html)
- [TANK BATTLEIO](https://quizverses.github.io/tank-battleio.html)
- [CATEGORY RACING DRIVING 2](https://quizverses.pages.dev/category-racing-driving-2.html)
- [SUPER STAR ANIMAL SALON](https://quizverses.pages.dev/super-star-animal-salon.html)
- [COLORWARSIO CONQUEST GAME](https://studyquests.github.io/colorwarsio-conquest-game.html)
- [CATEGORY FOOTBALL](https://quizverses.pages.dev/category-football.html)
- [CATEGORY HORROR](https://quizverses.github.io/category-horror.html)
- [IDLE BARBER SHOP](https://quizverses.pages.dev/idle-barber-shop.html)
- [COSMIC TETRIZ PUZZLES](https://studyplaying.github.io/cosmic-tetriz-puzzles.html)
- [ANOMALY CONTENT RECORD](https://quizverses.pages.dev/anomaly-content-record.html)
- [MENTAL HOSPITAL ESCAPE](https://studyquesthub.web.app/mental-hospital-escape.html)
- [CATEGORY MERGE](https://studyquesthub.web.app/category-merge.html)
- [INDEX26](https://quizverses.github.io/index26.html)
- [CATEGORY BOARDGAMES](https://studyquests.pages.dev/category-boardgames.html)
- [MATCH COLLECTION](https://studyquests.github.io/match-collection.html)
- [INDEX14](https://learnquester.github.io/index14.html)
- [GOD OF LIGHT](https://quizverses-9d2f2.web.app/god-of-light.html)
- [BLOCKS AND THATS IT](https://studyplayings.web.app/blocks-and-thats-it.html)
- [CATEGORY QUIZ40](https://studyplaying.github.io/category-quiz40.html)
- [STICKMAN ARMY TEAM BATTLE](https://studyplayings.web.app/stickman-army-team-battle.html)
- [IDLE TRADE ROUTES](https://studyplayings.pages.dev/idle-trade-routes.html)
- [CAT RESCUE](https://studyquests.pages.dev/cat-rescue.html)
- [CATEGORY CASUAL 5](https://studyquests.github.io/category-casual-5.html)
- [CATEGORY BASKETBALL](https://quizverses.pages.dev/category-basketball.html)
- [CATEGORY MMO25](https://quizverses.pages.dev/category-mmo25.html)
- [MOTO ROAD RASH 3D 2](https://studyquesthub.web.app/moto-road-rash-3d-2.html)
- [DR PARKING](https://learnquester.github.io/dr-parking.html)
- [CATEGORY WEBGAME](https://studyplaying.github.io/category-webgame.html)
- [AQUA SORT WATER COLOR PUZZLE](https://quizverses-9d2f2.web.app/aqua-sort-water-color-puzzle.html)
- [SUGAR HEROES](https://studyquests.pages.dev/sugar-heroes.html)
- [HYPER NURSE HOSPITAL GAMES](https://studyquests.pages.dev/hyper-nurse-hospital-games.html)
- [MAGNET TRUCK](https://studyquesthub.web.app/magnet-truck.html)
- [DRILL QUEST](https://studyquests.pages.dev/drill-quest.html)
- [CATEGORY JUMPING147](https://studyquesthub.web.app/category-jumping147.html)
- [PURRFECT SCOOPS](https://studyquests.github.io/purrfect-scoops.html)
- [FAMILY PET DOG GAMES](https://studyplaying.github.io/family-pet-dog-games.html)
- [CATEGORY QUIZ](https://studyplayings.web.app/category-quiz.html)
- [STICK WAR SAGA](https://studyplayings.web.app/stick-war-saga.html)
- [KUZBASS HORROR](https://studyplaying.github.io/kuzbass-horror.html)
- [CATEGORY CAT](https://studyquesthub.web.app/category-cat.html)
- [MOLE DIG CLICKER](https://studyquesthub.web.app/mole-dig-clicker.html)
- [CATEGORY JUMPING147](https://quizverses.github.io/category-jumping147.html)
- [HEROBALL ADVENTURES 2](https://studyquests.pages.dev/heroball-adventures-2.html)
- [CATEGORY PHYSICS371](https://quizverses.pages.dev/category-physics371.html)
- [MATH RUNNER](https://studyquests.pages.dev/math-runner.html)
- [EASY OBBY JUMP AND RUN CHALLENGE ONLINE](https://quizverses.github.io/easy-obby-jump-and-run-challenge-online.html)
- [CATEGORY RACING DRIVING](https://studyplayings.web.app/category-racing-driving.html)
- [BOBB S WORLD](https://studyquests.pages.dev/bobb-s-world.html)
- [STACKTRIS 2048](https://quizverses.pages.dev/stacktris-2048.html)
- [CATEGORY 1 PLAYER139](https://learnquester.github.io/category-1-player139.html)
- [CATEGORY MAKEUP](https://studyplayings.pages.dev/category-makeup.html)
- [ZOO SHAP](https://quizverses-9d2f2.web.app/zoo-shap.html)
- [CATEGORY CASUAL 4](https://studyquesthub.web.app/category-casual-4.html)
- [PANDA SHOP SIMULATOR](https://studyquests.pages.dev/panda-shop-simulator.html)
- [MEGA RAMPS ULTIMATE CAR RACES](https://studyplayings.pages.dev/mega-ramps-ultimate-car-races.html)
- [CATEGORY INTERSTELLARUNBLOCKER](https://learnquester.github.io/category-interstellarunblocker.html)
- [CATEGORY THINKY](https://studyplayings.pages.dev/category-thinky.html)
- [BUBBITS](https://studyquests.pages.dev/bubbits.html)
- [HYPERSPACE   QUANTUM FRACTURE FEZ](https://quizverses.github.io/hyperspace---quantum-fracture-fez.html)
- [CAPYBARA JUMP](https://studyquests.pages.dev/capybara-jump.html)
- [SWIPETOWN](https://studyquests.pages.dev/swipetown.html)
- [CATEGORY RUNNING](https://studyquests.github.io/category-running.html)
- [CATEGORY EDUCATIONAL](https://studyquesthub.web.app/category-educational.html)
- [BUBBLE AROUND](https://studyquesthub.web.app/bubble-around.html)
- [INDEX8](https://quizverses.pages.dev/index8.html)
- [SQUID ESCAPE BUT BLOCKWORLD](https://studyplaying.github.io/squid-escape-but-blockworld.html)
- [SLIME ATTACK PUZZLE](https://studyplayings.web.app/slime-attack-puzzle.html)
- [CATEGORY MOUSE1 697](https://studyquests.github.io/category-mouse1-697.html)
- [CATEGORY MISSION207](https://studyquesthub.web.app/category-mission207.html)
- [JELLO BUBBLES](https://quizverses.pages.dev/jello-bubbles.html)
- [ASMR WATER VS FIRE](https://studyquesthub.web.app/asmr-water-vs-fire.html)
- [CUBATORIA MERGE 2048](https://quizverses-9d2f2.web.app/cubatoria-merge-2048.html)
- [TEACHER SIMULATOR](https://studyquests.pages.dev/teacher-simulator.html)
- [COP SIMULATOR](https://studyplayings.web.app/cop-simulator.html)
- [TILES MATCHING](https://studyplayings.web.app/tiles-matching.html)
- [CATEGORY SOCCER](https://studyquesthub.web.app/category-soccer.html)
- [CATEGORY SOCCER](https://quizverses.pages.dev/category-soccer.html)
- [CATEGORY SPACE](https://studyplaying.github.io/category-space.html)
- [TERMS](https://learnquester.github.io/terms.html)
- [CATEGORY CAR 2](https://quizverses.pages.dev/category-car-2.html)
- [CATEGORY HORROR90](https://quizverses.github.io/category-horror90.html)
- [BANK BOOM TUNG TUNG SAHUR](https://studyquesthub.web.app/bank-boom-tung-tung-sahur.html)
- [BATTLE ZONE 2D](https://studyquests.pages.dev/battle-zone-2d.html)
- [CATEGORY POOL](https://quizverses-9d2f2.web.app/category-pool.html)
- [ZENITH RUSH](https://studyplayings.web.app/zenith-rush.html)
- [TAP GO DELUXE](https://quizverses.github.io/tap-go-deluxe.html)
- [DRAW BRIDGE CHALLENGE](https://quizverses.pages.dev/draw-bridge-challenge.html)
- [PATO VS COPS](https://studyquests.pages.dev/pato-vs-cops.html)
- [AGENT HUNT SPY SHOOTER GAME](https://quizverses.github.io/agent-hunt-spy-shooter-game.html)
- [SANTA VS SKRITCH](https://studyplayings.pages.dev/santa-vs-skritch.html)
- [CATEGORY INTERSTELLARNETWORK](https://studyquests.github.io/category-interstellarnetwork.html)
- [WARPING BAT](https://quizverses.pages.dev/warping-bat.html)
- [HIGH HEEL DESIGN](https://studyplaying.github.io/high-heel-design.html)
- [CATEGORY PUZZLE 2](https://studyquesthub.web.app/category-puzzle-2.html)
- [US ARMY CAR GAMES TRUCK DRIVING](https://quizverses.github.io/us-army-car-games-truck-driving.html)
- [CATEGORY FLASH](https://quizverses-9d2f2.web.app/category-flash.html)
- [BRAIN TEST ONE LINE DRAW PUZZLE](https://studyquests.pages.dev/brain-test-one-line-draw-puzzle.html)
- [PHYSICS BOX 2](https://studyplayings.pages.dev/physics-box-2.html)
- [CATEGORY MINECRAFT 2](https://studyplayings.pages.dev/category-minecraft-2.html)
- [DART HERO](https://studyplayings.pages.dev/dart-hero.html)
- [OFFROAD JEEP GAME SIMULATOR](https://studyplayings.pages.dev/offroad-jeep-game-simulator.html)
- [CATEGORY MINECRAFT81](https://quizverses.pages.dev/category-minecraft81.html)
- [TRICKY ARROW 2](https://studyplayings.pages.dev/tricky-arrow-2.html)
- [HYPER NURSE HOSPITAL GAMES](https://quizverses.github.io/hyper-nurse-hospital-games.html)
- [SUPER DOG HERO DASH](https://studyplayings.pages.dev/super-dog-hero-dash.html)
- [CATEGORY BUILDING182](https://quizverses.pages.dev/category-building182.html)
- [STRAWBERRY SHORTCAKE](https://studyplayings.pages.dev/strawberry-shortcake.html)
- [CATEGORY SURVIVAL366](https://studyplayings.pages.dev/category-survival366.html)
- [TILE CONNECT PAIR MATCH PUZZLE](https://studyplaying.github.io/tile-connect-pair-match-puzzle.html)
- [CATEGORY BRAIN260](https://studyplayings.pages.dev/category-brain260.html)
- [MAHJONG CONNECT SPOOKY](https://studyquests.pages.dev/mahjong-connect-spooky.html)
- [STICKMAN RAGDOLL PLAYGROUND](https://learnquester.github.io/stickman-ragdoll-playground.html)
- [BASKET SPORT STARS](https://studyplayings.web.app/basket-sport-stars.html)
- [JENNYS MATH PUZZLE](https://studyquesthub.web.app/jennys-math-puzzle.html)
- [DYNAMONS 11](https://studyquesthub.web.app/dynamons-11.html)
