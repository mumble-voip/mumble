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
- [CATEGORY PREMIUM PERKS71](https://themindplay.pages.dev/category-premium-perks71.html)
- [BUBBLE SHOOTER BUTTERFLY](https://quizverses.pages.dev/bubble-shooter-butterfly.html)
- [HIDE AND LUIG](https://themindplays.pages.dev/hide-and-luig.html)
- [CATEGORY FASHION105](https://studyquests.pages.dev/category-fashion105.html)
- [CATEGORY FLASH 2](https://quizverses.pages.dev/category-flash-2.html)
- [INDEX41](https://themindzone.pages.dev/index41.html)
- [MUSHROOM BLOCKS](https://studyplayings.web.app/mushroom-blocks.html)
- [CATEGORY RESTAURANT64](https://themindzone.pages.dev/category-restaurant64.html)
- [RIFT OF HELL DEMONS WAR](https://themindzone.pages.dev/rift-of-hell-demons-war.html)
- [CATEGORY SECURLY BYPASS](https://quizverses.pages.dev/category-securly-bypass.html)
- [CATEGORY AGILITY](https://themindzone.pages.dev/category-agility.html)
- [CATEGORY CAN T STOP PLAYING215](https://iskillplay.web.app/category-can-t-stop-playing215.html)
- [BLOCKS BREAKER](https://studyquesthub.web.app/blocks-breaker.html)
- [JELLY MONSTERS LINK PUZZLE](https://themindplays.pages.dev/jelly-monsters-link-puzzle.html)
- [EMERGENCY OPERATOR](https://thelearnquester.web.app/emergency-operator.html)
- [INDEX32](https://iskillplay.web.app/index32.html)
- [MOTO TRIALS RUSH](https://themindzone.pages.dev/moto-trials-rush.html)
- [SAND SORT COLOR PUZZLE GAME](https://studyquesthub.web.app/sand-sort-color-puzzle-game.html)
- [TATTOO MASTER](https://studyplayings.web.app/tattoo-master.html)
- [ARCHERS RAGDOLL PHYSICS](https://learnquester.github.io/archers-ragdoll-physics.html)
- [CATEGORY CASUAL 11](https://iskillquest.pages.dev/category-casual-11.html)
- [CATEGORY BIKE 2](https://quizverses.pages.dev/category-bike-2.html)
- [CATEGORY COOKING46](https://studyquesthub.web.app/category-cooking46.html)
- [COLOR COCKTAIL](https://learnquester.github.io/color-cocktail.html)
- [CATEGORY CASUAL 3](https://thequizzone.pages.dev/category-casual-3.html)
- [KITTY MATCH 3 PUZZLE GAME](https://studyplayings.pages.dev/kitty-match-3-puzzle-game.html)
- [SPACE CLEANER](https://quizverses-9d2f2.web.app/space-cleaner.html)
- [CATEGORY UNBLOCKED](https://learnquester.github.io/category-unblocked.html)
- [WORLDGUESSR](https://studyquesthub.web.app/worldguessr.html)
- [STICKMAN MEGA BOSS BATTLES](https://themindplays.pages.dev/stickman-mega-boss-battles.html)
- [INDEX6](https://thelearnquester.web.app/index6.html)
- [STACK UP](https://themindzone.pages.dev/stack-up.html)
- [OBBY PUMP UP YOUR MUSCLES 1 PER SECOND](https://themindplays.pages.dev/obby-pump-up-your-muscles-1-per-second.html)
- [TILE MATCH CAFE](https://themindskillplayplay.pages.dev/tile-match-cafe.html)
- [INDEX28](https://themindzone.pages.dev/index28.html)
- [SUPERHEROES AND THE WAND](https://quizverses.pages.dev/superheroes-and-the-wand.html)
- [SLITHORIA](https://themindzone.pages.dev/slithoria.html)
- [DINOSAUR SHIFTING RUN](https://themindplays.pages.dev/dinosaur-shifting-run.html)
- [HUNTING UNDERWATER SPEARFISHING](https://themindskillplayplay.pages.dev/hunting-underwater-spearfishing.html)
- [RUNIC BLOCK COLLAPSE](https://themindplays.pages.dev/runic-block-collapse.html)
- [MY ARCADE CENTER](https://studyplayings.pages.dev/my-arcade-center.html)
- [OFFICE SPIDER SOLITAIRE](https://themindplays.pages.dev/office-spider-solitaire.html)
- [CATEGORY ANIMAL216](https://studyquesthub.web.app/category-animal216.html)
- [CATEGORY IDLE445](https://themindplays.pages.dev/category-idle445.html)
- [INDEX22](https://studyquests.github.io/index22.html)
- [WAVE DASH GEOMETRY ARROW](https://studyquesthub.web.app/wave-dash-geometry-arrow.html)
- [SWORD PLAY NINJA SLICE RUNNER](https://studyquesthub.web.app/sword-play-ninja-slice-runner.html)
- [MINICRAFT WINTERBLOCK](https://themindzone.pages.dev/minicraft-winterblock.html)
- [CATEGORY CASUAL](https://studyquesthub.web.app/category-casual.html)
- [ASSOCIATION CONNECT WORD](https://thelearnquesters.pages.dev/association-connect-word.html)
- [PRINCESS RUN 3D](https://studyplayings.pages.dev/princess-run-3d.html)
- [IDLE DRIVE MERGE UPGRADE DRIVE](https://themindplay.github.io/idle-drive-merge-upgrade-drive.html)
- [CATEGORY PLATFORM260](https://themindplay.github.io/category-platform260.html)
- [STEAL A FISH](https://themindplays.pages.dev/steal-a-fish.html)
- [BUBBLE SHOOTER NEON](https://studyquesthub.web.app/bubble-shooter-neon.html)
- [STICKMAN VS ZOMBIES WORLDCRAFT](https://thequizzone.pages.dev/stickman-vs-zombies-worldcraft.html)
- [DINO SHOOTER PRO](https://learnquester.github.io/dino-shooter-pro.html)
- [MAHJONG BUILD FARM](https://quizverses-9d2f2.web.app/mahjong-build-farm.html)
- [MY CASTLE MERGE STORY](https://studyquesthub.web.app/my-castle-merge-story.html)
- [SINGLE STROKE LINE DRAW](https://thelearnquesters.pages.dev/single-stroke-line-draw.html)
- [SUPER RACING GT DRAG PRO](https://themindplay.github.io/super-racing-gt-drag-pro.html)
- [GRID BLAST](https://thelearnquester.web.app/grid-blast.html)
- [OMEGA LAYERS](https://studyplayings.pages.dev/omega-layers.html)
- [MOJO MATCH 3D](https://learnquesters.pages.dev/mojo-match-3d.html)
- [CATEGORY MOUSE1 697](https://studyplayings.web.app/category-mouse1-697.html)
- [HEXA SORT](https://themindzone.pages.dev/hexa-sort.html)
- [WINTER SOLITAIRE TRIPEAKS](https://iskillquest.pages.dev/winter-solitaire-tripeaks.html)
- [OBBY PRISON RUN](https://themindzone.pages.dev/obby-prison-run.html)
- [MERGE FLOWERS](https://thelearnquesters.pages.dev/merge-flowers.html)
- [PRISMROLL 3D](https://learnquesters.pages.dev/prismroll-3d.html)
- [HIDE AND SEEK BLUE MONSTER](https://iskillquest.pages.dev/hide-and-seek-blue-monster.html)
- [GRINDCRAFT](https://themindplays.pages.dev/grindcraft.html)
- [HAPPY MONSTERS 2](https://iskillplay.web.app/happy-monsters-2.html)
- [MONSTER COLLECT RUN](https://themindplay.github.io/monster-collect-run.html)
- [CATEGORY BRAIN260](https://learnquester.pages.dev/category-brain260.html)
- [CATEGORY CASUAL 2](https://studyquesthub.web.app/category-casual-2.html)
- [MATCH TEN NUMBER PUZZLE](https://themindplays.pages.dev/match-ten-number-puzzle.html)
- [TOILET RUSH](https://themindplays.pages.dev/toilet-rush.html)
- [CRYPTO GALS TIKTOK FASHION](https://studyplayings.pages.dev/crypto-gals-tiktok-fashion.html)
- [INFINITE CRAFT](https://studyquesthub.web.app/infinite-craft.html)
- [MONSTER MERGE LEGENDS ALIVE](https://iskillquest.pages.dev/monster-merge-legends-alive.html)
- [TILES MATCHING](https://themindskillplayplay.pages.dev/tiles-matching.html)
- [DAILY SOLITAIRE MAHJONG CLASSIC](https://themindskillplayplay.pages.dev/daily-solitaire-mahjong-classic.html)
- [BUBBLE RACE PARTY](https://learnquester.pages.dev/bubble-race-party.html)
- [CATEGORY SIMULATION 2](https://themindplay.github.io/category-simulation-2.html)
- [MINI SHOOTERS](https://quizverses-9d2f2.web.app/mini-shooters.html)
- [BOLTS AND NUTS SORTING](https://themindplays.pages.dev/bolts-and-nuts-sorting.html)
- [CATEGORY DIRT BIKE18](https://themindplays.pages.dev/category-dirt-bike18.html)
- [MAHJONG SOLITAIRE ZODIAC](https://thelearnquester.web.app/mahjong-solitaire-zodiac.html)
- [TANK SNIPER 3D](https://themindskillplayplay.pages.dev/tank-sniper-3d.html)
- [SPRUNKI MINI GAMES](https://themindplays.pages.dev/sprunki-mini-games.html)
- [ART MASTER ORIGINS](https://thelearnquesters.pages.dev/art-master-origins.html)
- [BARBEE MET GALA TRANSFORMATION](https://quizverses.pages.dev/barbee-met-gala-transformation.html)
- [CATEGORY TANK](https://learnquester.pages.dev/category-tank.html)
- [SQUID CHALLENGE PLAY TO SURVIVE](https://learnquesters.pages.dev/squid-challenge-play-to-survive.html)
- [OBBY ESCAPE BARRYS JAIL PARKOUR](https://quizverses-9d2f2.web.app/obby-escape-barrys-jail-parkour.html)
- [SPIDER ROPE HERO CITY FIGHT](https://studyquesthub.web.app/spider-rope-hero-city-fight.html)
- [SITEMAP](https://learnquesters.pages.dev/sitemap.html)
- [MIRACLE MAHJONG](https://iskillquest.pages.dev/miracle-mahjong.html)
- [MOTO STUNT BIKER](https://thelearnquesters.pages.dev/moto-stunt-biker.html)
- [WALKERS ATTACK](https://themindplays.pages.dev/walkers-attack.html)
- [CATEGORY DRIFTING116](https://iskillquest.pages.dev/category-drifting116.html)
- [LOVE CATS ROPE](https://themindplay.github.io/love-cats-rope.html)
- [HUNGRY NOOB CAFE SIMULATOR](https://learnquester.github.io/hungry-noob-cafe-simulator.html)
- [HIGH SPEED CRAZY BIKE](https://studyplayings.web.app/high-speed-crazy-bike.html)
- [CATEGORY BYEPASSHUB](https://learnquester.pages.dev/category-byepasshub.html)
- [CATEGORY MATCH 3](https://learnquester.github.io/category-match-3.html)
- [PET MERGE](https://themindzone.pages.dev/pet-merge.html)
- [CATEGORY AGILITY 3](https://studyquesthub.web.app/category-agility-3.html)
- [MOTO STUNTS DRIVING RACING](https://studyquesthub.web.app/moto-stunts-driving-racing.html)
- [WOODS OF NEVIA FOREST SURVIVAL](https://themindzone.pages.dev/woods-of-nevia-forest-survival.html)
- [RAINBOW BALLS 2048](https://themindskillplayplay.pages.dev/rainbow-balls-2048.html)
- [SPIDER SOLITAIRE](https://studyplayings.pages.dev/spider-solitaire.html)
- [STEAL A FISH](https://quizverses.pages.dev/steal-a-fish.html)
- [HEX PLANET IDLE](https://themindskillplayplay.pages.dev/hex-planet-idle.html)
- [STICKMAN ARMY TEAM BATTLE](https://studyplayings.web.app/stickman-army-team-battle.html)
- [BUBBLE SHOOTER VINTAGE](https://themindplays.pages.dev/bubble-shooter-vintage.html)
- [CATEGORY MISSION206](https://iskillquest.pages.dev/category-mission206.html)
- [GLUTTONY](https://studyquesthub.web.app/gluttony.html)
- [TRUCK STACK COLORS](https://studyquesthub.web.app/truck-stack-colors.html)
- [INDEX7](https://studyquests.github.io/index7.html)
- [NEKOS ADVENTURE](https://studyplayings.web.app/nekos-adventure.html)
- [FIRESIDE SOLITAIRE](https://quizverses.pages.dev/fireside-solitaire.html)
- [DROP BRICKS BREAKER](https://themindplays.pages.dev/drop-bricks-breaker.html)
- [MOJICON GARDEN CONNECT](https://thelearnquester.web.app/mojicon-garden-connect.html)
- [SKY BALLS 3D](https://themindplays.pages.dev/sky-balls-3d.html)
- [SUPER CLONER 3D](https://learnquester.pages.dev/super-cloner-3d.html)
- [TINY FOOTBALL CUP 2026](https://learnquester.github.io/tiny-football-cup-2026.html)
- [CATEGORY 204828](https://theskillquest.pages.dev/category-204828.html)
- [ZOMBIE SURVIVAL](https://themindskillplayplay.pages.dev/zombie-survival.html)
