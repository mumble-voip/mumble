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
- [CATEGORY ADVENTURE](https://theskillquest.pages.dev/category-adventure.html)
- [GUN CLONE](https://studyquests.github.io/gun-clone.html)
- [CATEGORY RUNNING](https://studyplayings.pages.dev/category-running.html)
- [SWORD AND SPIN](https://quizverses.github.io/sword-and-spin.html)
- [CATEGORY SIMULATION 2](https://quizverses.pages.dev/category-simulation-2.html)
- [CATEGORY THINKY 2](https://studyplayings.web.app/category-thinky-2.html)
- [HOME RUSH THE FISH WAR](https://thelearnquester.web.app/home-rush-the-fish-war.html)
- [US ARMY CAR GAMES TRUCK DRIVING](https://quizverses.github.io/us-army-car-games-truck-driving.html)
- [ZOMBIE RODEO MULTIPLICATION](https://quizverses.github.io/zombie-rodeo-multiplication.html)
- [OBBY CHAMPIONS](https://quizverses.github.io/obby-champions.html)
- [FRUIT MERGE JUICY DROP GAME](https://studyquests.pages.dev/fruit-merge-juicy-drop-game.html)
- [CATEGORY JIGSAW](https://quizverses.github.io/category-jigsaw.html)
- [CRAZYSTEVEIO](https://quizverses.github.io/crazysteveio.html)
- [LINKLINK](https://learnquester.github.io/linklink.html)
- [POLYGON SPACE](https://quizverses.github.io/polygon-space.html)
- [SHIP CONTROL 3D](https://studyplayings.pages.dev/ship-control-3d.html)
- [TRIANGLES](https://studyquests.pages.dev/triangles.html)
- [SUMMER MAZE](https://quizverses.github.io/summer-maze.html)
- [ANIMATION COLORING ALPHABET LORE](https://quizverses.github.io/animation-coloring-alphabet-lore.html)
- [CATEGORY SNAKE40](https://studyquests.pages.dev/category-snake40.html)
- [CATEGORY BLOODY29](https://quizverses.github.io/category-bloody29.html)
- [CATEGORY FASHION105](https://thelearnquester.web.app/category-fashion105.html)
- [ONLINE PORTAL](https://studyquests.pages.dev/)
- [MY CAKE SHOP BAKE SERVE](https://quizverses.github.io/my-cake-shop-bake-serve.html)
- [BARBEE SUMMER VACATION](https://studyquesthub.web.app/barbee-summer-vacation.html)
- [ITALIAN BRAINROT PUZZLE](https://studyquesthub.web.app/italian-brainrot-puzzle.html)
- [OIL DIGGING](https://studyplayings.pages.dev/oil-digging.html)
- [CATEGORY MINECRAFT 2](https://thelearnquester.web.app/category-minecraft-2.html)
- [FROGIO](https://quizverses.github.io/frogio.html)
- [METAXIS](https://quizverses.github.io/metaxis.html)
- [DARTS JAM](https://quizverses.github.io/darts-jam.html)
- [RICH CHOICE RUN](https://quizverses.github.io/rich-choice-run.html)
- [COLOR YARN SORT](https://quizverses.github.io/color-yarn-sort.html)
- [ORDER OF OPERATION CHALLENGE](https://studyplayings.pages.dev/order-of-operation-challenge.html)
- [DEFORM IT](https://studyquesthub.web.app/deform-it.html)
- [ESCAPE OR DIE TROLL DEVIL LEVELS](https://studyplayings.pages.dev/escape-or-die-troll-devil-levels.html)
- [BLOCK CUT CLEANER](https://quizverses.github.io/block-cut-cleaner.html)
- [CATEGORY CASUAL 2](https://quizverses.github.io/category-casual-2.html)
- [CATEGORY POOL 3](https://quizverses.github.io/category-pool-3.html)
- [RED STICKMAN VS CRAFTMANS](https://studyquesthub.web.app/red-stickman-vs-craftmans.html)
- [CLEAN HOUSE CLEARING TRASH AND DIRT](https://quizverses.github.io/clean-house-clearing-trash-and-dirt.html)
- [MATCH MASTER](https://studyquesthub.web.app/match-master.html)
- [CATEGORY MONSTER206](https://learnquester.github.io/category-monster206.html)
- [MONSTER ARENA](https://quizverses.github.io/monster-arena.html)
- [WINTER WOLF](https://quizverses.github.io/winter-wolf.html)
- [GO CHICKEN GO](https://studyquesthub.web.app/go-chicken-go.html)
- [UNSCREW WOOD PUZZLE](https://quizverses.github.io/unscrew-wood-puzzle.html)
- [MAHJONG LINES](https://studyquesthub.web.app/mahjong-lines.html)
- [CATEGORY CASUAL 8](https://quizverses.github.io/category-casual-8.html)
- [BATTLE ARENA RACE TO WIN](https://studyquesthub.web.app/battle-arena-race-to-win.html)
- [CATEGORY SHOOTER](https://quizverses.github.io/category-shooter.html)
- [NEKOS ADVENTURE](https://studyquesthub.web.app/nekos-adventure.html)
- [EASTER EGGVENTURE](https://studyquesthub.web.app/easter-eggventure.html)
- [CATEGORY GAMES](https://learnquester.github.io/category-games.html)
- [RUN 3D](https://studyplayings.pages.dev/run-3d.html)
- [SUPER SWING](https://studyquests.pages.dev/super-swing.html)
- [CATEGORY CASUAL 6](https://studyquests.pages.dev/category-casual-6.html)
- [TRAFFIC COP 3D](https://studyplayings.pages.dev/traffic-cop-3d.html)
- [WORDS FROM WORDS](https://quizverses.github.io/words-from-words.html)
- [FEED ME MONSTERS IDLE BATTLE](https://studyplayings.pages.dev/feed-me-monsters-idle-battle.html)
- [ADVERSATOR](https://quizverses.github.io/adversator.html)
- [MAD TRUCK](https://studyplaying.github.io/mad-truck.html)
- [DOGGI](https://studyquesthub.web.app/doggi.html)
- [MINI GRAND THEFT CITY](https://quizverses.github.io/mini-grand-theft-city.html)
- [CATEGORY MOUSE](https://quizverses.github.io/category-mouse.html)
- [STICKMAN DISMOUNT SIMULATOR](https://learnquester.github.io/stickman-dismount-simulator.html)
- [CATEGORY PUZZLE 2](https://quizverses.github.io/category-puzzle-2.html)
- [FISH LOVE PINS](https://studyquests.pages.dev/fish-love-pins.html)
- [PET SALON](https://quizverses.github.io/pet-salon.html)
- [CATEGORY RACING127](https://quizverses.github.io/category-racing127.html)
- [GIFT MERGE SANTA WORLD TOUR](https://learnquester.github.io/gift-merge-santa-world-tour.html)
- [BUBBLE SHOOTER HAWAII](https://quizverses.github.io/bubble-shooter-hawaii.html)
- [FIERCE BATTLE BREAKOUT](https://studyquesthub.web.app/fierce-battle-breakout.html)
- [LITTLE LILY HALLOWEEN PREP](https://learnquester.github.io/little-lily-halloween-prep.html)
- [LAVA JUMP](https://quizverses.github.io/lava-jump.html)
- [WITCH CRAFT POTION SORT](https://quizverses.github.io/witch-craft-potion-sort.html)
- [BLOCKAPOLYPSE ZOMBIE SHOOTER](https://studyquesthub.web.app/blockapolypse-zombie-shooter.html)
- [MY DINOSAUR LAND](https://studyplayings.pages.dev/my-dinosaur-land.html)
- [WORM OUT BRAIN TEASER GAMES](https://quizverses.github.io/worm-out-brain-teaser-games.html)
- [SQUAD ASSEMBLER](https://studyplayings.pages.dev/squad-assembler.html)
- [4 HEXA](https://quizverses.github.io/4-hexa.html)
- [ONLINE PORTAL](https://cryptotify9.onrender.com/)
- [OBBY 1 PET EVERY SECONDS](https://learnquester.github.io/obby-1-pet-every-seconds.html)
- [PLANET HOPPER](https://quizverses.github.io/planet-hopper.html)
- [CATEGORY BIKE](https://studyquests.pages.dev/category-bike.html)
- [CATEGORY PUZZLE 6](https://quizverses.github.io/category-puzzle-6.html)
- [MATH QUEST](https://studyquesthub.web.app/math-quest.html)
- [CATEGORY DEFENSE](https://studyplaying.github.io/category-defense.html)
- [PIMPLE SQUEEZE](https://quizverses.github.io/pimple-squeeze.html)
- [SITEMAP](https://brainquests.pages.dev/sitemap.html)
- [BUBBLE UP](https://studyquesthub.web.app/bubble-up.html)
- [CATEGORY BIKE](https://studyplaying.github.io/category-bike.html)
- [CATEGORY FARMING87](https://quizverses.github.io/category-farming87.html)
- [CATEGORY AIRPLANE](https://studyplaying.github.io/category-airplane.html)
- [NETQUEL COM](https://quizverses.github.io/netquel-com.html)
- [3D ACRYLIC NAIL NAIL ART GAME](https://quizverses.github.io/3d-acrylic-nail-nail-art-game.html)
- [PRIVACY](https://brainquests-fb2c5.web.app/privacy.html)
- [WORDLING DAILY WORD CHALLENGE](https://learnquester.github.io/wordling-daily-word-challenge.html)
- [CONTACT](https://learnquester.github.io/contact.html)
- [INDEX7](https://quizverses.github.io/index7.html)
- [CATEGORY CAN T STOP PLAYING212](https://studyquests.pages.dev/category-can-t-stop-playing212.html)
- [SUPER DOG HERO DASH](https://studyplayings.pages.dev/super-dog-hero-dash.html)
- [CATEGORY THINKY](https://studyplaying.github.io/category-thinky.html)
- [CRAZY ZOO SWIPE MATCH 3 PUZZLE GAME](https://quizverses.github.io/crazy-zoo-swipe-match-3-puzzle-game.html)
- [SHELF SWEEP](https://learnquester.github.io/shelf-sweep.html)
- [DREAM ROOM MAKEOVER](https://studyplayings.pages.dev/dream-room-makeover.html)
- [HEXANAUT IO](https://studyplayings.pages.dev/hexanaut-io.html)
- [CATEGORY BOARDGAMES](https://thelearnquester.web.app/category-boardgames.html)
- [INDEX8](https://quizverses.github.io/index8.html)
- [CATEGORY CARTOON76](https://quizverses.github.io/category-cartoon76.html)
- [MOJICON EMOJI CONNECT](https://studyquests.pages.dev/mojicon-emoji-connect.html)
- [PATO VS COPS](https://studyquests.pages.dev/pato-vs-cops.html)
- [ITALIAN BRAINROT BABY CLICKER](https://quizverses.github.io/italian-brainrot-baby-clicker.html)
- [FREECELL](https://learnquester.github.io/freecell.html)
- [INDEX3](https://thelearnquester.web.app/index3.html)
- [CATEGORY MMO25](https://studyplaying.github.io/category-mmo25.html)
- [VAMPIRIC ROULETTE ROMANCE](https://studyplayings.pages.dev/vampiric-roulette-romance.html)
- [CATEGORY STICKMAN](https://quizverses.github.io/category-stickman.html)
- [INDEX7](https://learnquester.github.io/index7.html)
- [FILL THE BOTTLE](https://studyplayings.pages.dev/fill-the-bottle.html)
- [CATEGORY CASUAL 8](https://studyquests.pages.dev/category-casual-8.html)
- [CATEGORY CASUAL 10](https://quizverses.github.io/category-casual-10.html)
- [MAHJONG RIDDLES EGYPT](https://quizverses.github.io/mahjong-riddles-egypt.html)
- [SAVE STRANDING FISH](https://studyplayings.pages.dev/save-stranding-fish.html)
- [GRAFFITI TAGS SPRAY PAINTING](https://quizverses.github.io/graffiti-tags-spray-painting.html)
- [ASMR NAIL TREATMENT](https://quizverses.github.io/asmr-nail-treatment.html)
- [CATEGORY RELAXING221](https://thelearnquester.web.app/category-relaxing221.html)
- [FLIP IT 3D](https://studyplaying.github.io/flip-it-3d.html)
- [DOGE MATCH](https://quizverses.github.io/doge-match.html)
- [PULL THE PINS](https://studyplayings.pages.dev/pull-the-pins.html)
