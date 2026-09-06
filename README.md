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
- [SISYPHUS SIMULATOR](https://learnquester.github.io/sisyphus-simulator.html)
- [HUNGRY NOOB CAFE SIMULATOR](https://theskillquest.pages.dev/hungry-noob-cafe-simulator.html)
- [CATEGORY STRATEGY 2](https://quizverses.github.io/category-strategy-2.html)
- [SURVIVAL ON RAFT MULTIPLAYER](https://quizverses.github.io/survival-on-raft-multiplayer.html)
- [LEXY](https://studyquesthub.web.app/lexy.html)
- [CATEGORY STICKMAN](https://learnquester.github.io/category-stickman.html)
- [CATEGORY FPS174](https://studyquesthub.web.app/category-fps174.html)
- [TRICKY ARROW 2](https://studyplayings.pages.dev/tricky-arrow-2.html)
- [SUPERMARKET SHOPPING FOR KIDS](https://studyquests.pages.dev/supermarket-shopping-for-kids.html)
- [ALIEN HUNTERS](https://studyquests.github.io/alien-hunters.html)
- [FACE CHANGES](https://studyplaying.github.io/face-changes.html)
- [SUPERHEROES AND THE WAND](https://quizverses.pages.dev/superheroes-and-the-wand.html)
- [HAPPY FARM THE CROP](https://studyplaying.github.io/happy-farm-the-crop.html)
- [CAR COLLISION MASTER](https://quizverses-9d2f2.web.app/car-collision-master.html)
- [CATEGORY MOUSE1 707](https://quizverses-9d2f2.web.app/category-mouse1-707.html)
- [ISOMETRIC ESCAPE 2](https://studyquests.github.io/isometric-escape-2.html)
- [INDEX34](https://studyquests.github.io/index34.html)
- [CATEGORY EDUCATIONAL](https://learnquester.github.io/category-educational.html)
- [CARS MERGE](https://quizverses.github.io/cars-merge.html)
- [SAVE THE CROP](https://studyplaying.github.io/save-the-crop.html)
- [BALING BUM](https://quizverses.github.io/baling-bum.html)
- [IDLE PIZZA BUSINESS](https://quizverses.github.io/idle-pizza-business.html)
- [CATEGORY STICKMAN](https://thelearnquester.web.app/category-stickman.html)
- [SPRUNKI MATCH](https://studyplaying.github.io/sprunki-match.html)
- [MAGIC FOREST MERGE THE SECRETS](https://learnquester.github.io/magic-forest-merge-the-secrets.html)
- [HYPERSPACE   QUANTUM FRACTURE FEZ](https://studyquests.github.io/hyperspace---quantum-fracture-fez.html)
- [3D CHESS MASTER](https://studyquests.pages.dev/3d-chess-master.html)
- [CATEGORY IO](https://quizverses-9d2f2.web.app/category-io.html)
- [INDEX6](https://learnquester.github.io/index6.html)
- [RAGDOLL ARENA 2 PLAYER](https://studyplayings.pages.dev/ragdoll-arena-2-player.html)
- [CATEGORY BUSINESS135](https://studyquests.pages.dev/category-business135.html)
- [BLOON POP](https://learnquester.github.io/bloon-pop.html)
- [CATEGORY RESTAURANT64](https://learnquester.github.io/category-restaurant64.html)
- [TRICKY CHALLENGES MINI GAMES](https://studyplaying.github.io/tricky-challenges-mini-games.html)
- [SERIOUS HEAD 2](https://quizverses.github.io/serious-head-2.html)
- [ONET MONSTER BOOK](https://studyquests.github.io/onet-monster-book.html)
- [PUSHIO](https://quizverses.github.io/pushio.html)
- [CATEGORY MATH29](https://quizverses-9d2f2.web.app/category-math29.html)
- [MAKE AMERICA GREAT AGAIN](https://quizverses.pages.dev/make-america-great-again.html)
- [BFFS LUXURY LOUNGEWEAR](https://quizverses.pages.dev/bffs-luxury-loungewear.html)
- [CATEGORY UNBLOCKED GAMES](https://learnquester.github.io/category-unblocked-games.html)
- [JAVELIN BATTLE](https://quizverses.pages.dev/javelin-battle.html)
- [FRUIT CAFE MATCH 3](https://quizverses.github.io/fruit-cafe-match-3.html)
- [CATEGORY MAHJONG](https://learnquester.github.io/category-mahjong.html)
- [DOOMSDAY SURVIVAL RPG SHOOTER](https://studyquests.github.io/doomsday-survival-rpg-shooter.html)
- [SHIP CONTROL 3D](https://studyplayings.pages.dev/ship-control-3d.html)
- [BED WARS](https://quizverses.github.io/bed-wars.html)
- [CATEGORY INTERSTELLAR](https://learnquester.github.io/category-interstellar.html)
- [CATEGORY 3D1 383](https://quizverses-9d2f2.web.app/category-3d1-383.html)
- [ROBLOX CRAFT RUN](https://quizverses.pages.dev/roblox-craft-run.html)
- [MINI GAMES PUZZLE COLLECTION](https://studyplaying.github.io/mini-games-puzzle-collection.html)
- [GEOMETRY WAVE HERO](https://studyquesthub.web.app/geometry-wave-hero.html)
- [MAGIC SORT](https://quizverses.github.io/magic-sort.html)
- [CATEGORY PENALTY](https://studyquests.github.io/category-penalty.html)
- [CATEGORY MATCH 3 2](https://quizverses.github.io/category-match-3-2.html)
- [INDEX24](https://quizverses.github.io/index24.html)
- [ONLINE PORTAL](https://cryptotify.github.io/)
- [INDEX17](https://thelearnquester.web.app/index17.html)
- [WAVE CHIC OCEAN FASHION FRENZY](https://studyquests.pages.dev/wave-chic-ocean-fashion-frenzy.html)
- [CATEGORY BATTLE524](https://learnquester.github.io/category-battle524.html)
- [CATEGORY DRAWING](https://thelearnquester.web.app/category-drawing.html)
- [DINO HUNTER KING](https://quizverses.pages.dev/dino-hunter-king.html)
- [DANCE ON HOTSTEPS MOBILE](https://studyquests.github.io/dance-on-hotsteps-mobile.html)
- [CATEGORY HORDE SURVIVAL67](https://studyquests.github.io/category-horde-survival67.html)
- [CATEGORY BATTLE](https://thelearnquester.web.app/category-battle.html)
- [GLADIATOR FIGHTS](https://learnquester.github.io/gladiator-fights.html)
- [EXCAVATOR SIMULATOR 3D](https://learnquester.github.io/excavator-simulator-3d.html)
- [BOMBER BATTLE ARENA](https://studyplaying.github.io/bomber-battle-arena.html)
- [ONLINE PORTAL](https://quizverses-9d2f2.web.app/)
- [JUICY MATCH](https://studyquests.github.io/juicy-match.html)
- [FARM MATCH SEASONS 2](https://quizverses.pages.dev/farm-match-seasons-2.html)
- [FUN IQ PUZZLE](https://studyplaying.github.io/fun-iq-puzzle.html)
- [TILE SORT MATCH 3](https://studyplaying.github.io/tile-sort-match-3.html)
- [CYBER MONDAY](https://quizverses.pages.dev/cyber-monday.html)
- [CATEGORY IDLE](https://learnquester.github.io/category-idle.html)
- [PLANET HOPPER](https://learnquester.github.io/planet-hopper.html)
- [BILLIARD DIAMOND CHALLENGE](https://learnquester.github.io/billiard-diamond-challenge.html)
- [PRIVACY](https://brainquests.vercel.app/privacy.html)
- [CATEGORY BATTLE](https://quizverses-9d2f2.web.app/category-battle.html)
- [PORT SHIPPING TYCOON](https://studyplaying.github.io/port-shipping-tycoon.html)
- [SNOW RIDER 3D NOSTALGIA](https://studyquests.github.io/snow-rider-3d-nostalgia.html)
- [GRANNYS CLASSROOM NIGHTMARE](https://learnquester.github.io/grannys-classroom-nightmare.html)
- [SUPERPIXELINT](https://studyplaying.github.io/superpixelint.html)
- [DEAD BRAIN](https://studyquests.github.io/dead-brain.html)
- [CATEGORY CASUAL 9](https://learnquester.github.io/category-casual-9.html)
- [MINI GRAND THEFT CITY](https://quizverses.github.io/mini-grand-theft-city.html)
- [RUN FROM BABA YAGA](https://studyplaying.github.io/run-from-baba-yaga.html)
- [CATEGORY ESCAPE187](https://thelearnquester.web.app/category-escape187.html)
- [BUSY BEE HIVE](https://quizverses.github.io/busy-bee-hive.html)
- [CATEGORY INCREMENTAL388](https://learnquester.github.io/category-incremental388.html)
- [TOW N GO](https://quizverses.github.io/tow-n-go.html)
- [CATEGORY FASHION105](https://learnquester.github.io/category-fashion105.html)
- [HIGH SPEED CRAZY BIKE](https://studyquests.github.io/high-speed-crazy-bike.html)
- [CATEGORY FPS 2](https://thelearnquester.web.app/category-fps-2.html)
- [ISOMETRIC ESCAPE](https://studyquests.github.io/isometric-escape.html)
- [HEXA TILE TRIO](https://studyquests.github.io/hexa-tile-trio.html)
- [CATEGORY CASUAL 5](https://studyquests.github.io/category-casual-5.html)
- [TANK BATTLEIO](https://studyplaying.github.io/tank-battleio.html)
- [CITYQUEST](https://studyquests.github.io/cityquest.html)
- [BLOOM SORT 2 BEE PUZZLE](https://learnquester.github.io/bloom-sort-2-bee-puzzle.html)
- [FROGIO](https://studyquests.github.io/frogio.html)
- [CATEGORY TOWER DEFENSE](https://learnquester.github.io/category-tower-defense.html)
- [TONY ARCHER](https://learnquester.github.io/tony-archer.html)
- [ADDICTION MINI SOLITAIRE](https://quizverses.github.io/addiction-mini-solitaire.html)
- [CATEGORY FPS174](https://thelearnquester.web.app/category-fps174.html)
- [SITEMAP](https://learnquester.github.io/sitemap.html)
- [CATEGORY GAMES](https://learnquester.github.io/category-games.html)
- [3D SUPER ROLLING BALL RACE](https://studyplaying.github.io/3d-super-rolling-ball-race.html)
- [INDEX21](https://quizverses-9d2f2.web.app/index21.html)
- [BRICKS BALLS BREAKER](https://studyplayings.pages.dev/bricks-balls-breaker.html)
- [CATEGORY GROW GAMES](https://thelearnquester.web.app/category-grow-games.html)
- [CATEGORY CASUAL971](https://learnquester.github.io/category-casual971.html)
- [ARROW HIT](https://quizverses.github.io/arrow-hit.html)
- [DEVIL DASH](https://studyplaying.github.io/devil-dash.html)
- [CATEGORY AVOID](https://learnquester.github.io/category-avoid.html)
- [AVENGER GUARD](https://studyplaying.github.io/avenger-guard.html)
- [CAKE LINK MASTER](https://studyquests.github.io/cake-link-master.html)
- [SNAKES](https://learnquester.github.io/snakes.html)
- [MOW IT](https://studyquests.github.io/mow-it.html)
- [CATEGORY RACING DRIVING](https://learnquester.github.io/category-racing-driving.html)
- [MAHJONG CRIMES PUZZLE STORY](https://studyplaying.github.io/mahjong-crimes-puzzle-story.html)
- [INDEX21](https://studyplayings.pages.dev/index21.html)
- [ONLINE PORTAL](https://brainquests.onrender.com/)
- [GUN BUILDER](https://quizverses.github.io/gun-builder.html)
- [CATEGORY FOOTBALL](https://thelearnquester.web.app/category-football.html)
- [STICKMAN MEGA BOSS BATTLES](https://learnquester.github.io/stickman-mega-boss-battles.html)
- [CATEGORY GUN241](https://quizverses-9d2f2.web.app/category-gun241.html)
- [CATEGORY MEDIEVAL15](https://studyquests.pages.dev/category-medieval15.html)
- [CATEGORY BRAIN261](https://learnquester.github.io/category-brain261.html)
- [BRAWL STARS BRAVE ADVENTURE](https://studyquests.github.io/brawl-stars-brave-adventure.html)
