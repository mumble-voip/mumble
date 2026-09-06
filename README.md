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
- [MAGIC BUBBLES](https://theskillquest.pages.dev/magic-bubbles.html)
- [CATEGORY TRAFFIC34](https://studyquesthub.web.app/category-traffic34.html)
- [ZOMBIES BATTLE FOR SURVIVAL](https://studyplaying.github.io/zombies-battle-for-survival.html)
- [BOXING FIGHTER](https://studyquests.github.io/boxing-fighter.html)
- [CATEGORY WEBGAME](https://studyquests.github.io/category-webgame.html)
- [FRUITE SWIPE](https://studyplaying.github.io/fruite-swipe.html)
- [HAWAII MATCH 5](https://studyquests.github.io/hawaii-match-5.html)
- [TILE CONNECT CLUB](https://studyplaying.github.io/tile-connect-club.html)
- [WHEEL OF BINGO](https://learnquester.github.io/wheel-of-bingo.html)
- [DOTS MASTER](https://studyquests.github.io/dots-master.html)
- [HIDDEN OBJECT MY HOTEL](https://studyplayings.web.app/hidden-object-my-hotel.html)
- [IDLE TRADE ISLE](https://studyquests.github.io/idle-trade-isle.html)
- [HAPPY MONSTERS 2](https://studyplayings.pages.dev/happy-monsters-2.html)
- [SMASH DEFENSE](https://learnquester.github.io/smash-defense.html)
- [SKYDOM](https://studyquests.github.io/skydom.html)
- [FRUIT MERGE RELOADED](https://studyquesthub.web.app/fruit-merge-reloaded.html)
- [TRAFFIC TAP PUZZLE](https://learnquester.github.io/traffic-tap-puzzle.html)
- [CATEGORY BATTLE CATEGORY](https://studyplaying.github.io/category-battle-category.html)
- [TILE CONNECT PAIR MATCH PUZZLE](https://studyplaying.github.io/tile-connect-pair-match-puzzle.html)
- [BUBBLE BLASTERS](https://studyquests.github.io/bubble-blasters.html)
- [CATEGORY SANDBOX41](https://studyplayings.web.app/category-sandbox41.html)
- [BUBBLE SHOOTER FREE 3](https://studyquests.github.io/bubble-shooter-free-3.html)
- [AIDAN IN DANGER](https://studyquests.pages.dev/aidan-in-danger.html)
- [FLAG PUZZLE JAM COLLECT FLAGS](https://learnquester.github.io/flag-puzzle-jam-collect-flags.html)
- [CATEGORY SOLDIER11](https://studyquests.github.io/category-soldier11.html)
- [WORLD FLAGS TRIVIA](https://studyquests.github.io/world-flags-trivia.html)
- [NINJA SPELL MATCH](https://studyplaying.github.io/ninja-spell-match.html)
- [WATER SORT](https://studyplaying.github.io/water-sort.html)
- [CATEGORY STICKMAN175](https://studyquests.github.io/category-stickman175.html)
- [MERMAID PRINCESS AVATER CASTLE](https://learnquester.github.io/mermaid-princess-avater-castle.html)
- [CATEGORY STICKMAN](https://studyquests.github.io/category-stickman.html)
- [HAPPY EGG CATCH](https://studyquests.pages.dev/happy-egg-catch.html)
- [MAGIC PRINCESS DRESS UP DOLL](https://studyplaying.github.io/magic-princess-dress-up-doll.html)
- [WHATS GRANDMA HIDING](https://studyplayings.web.app/whats-grandma-hiding.html)
- [BULLET HEROES](https://quizverses.pages.dev/bullet-heroes.html)
- [MOTO TRAFFIC RIDER](https://quizverses.pages.dev/moto-traffic-rider.html)
- [NEW YEARS MIRACLES CONNECT THE BALLS](https://studyquests.github.io/new-years-miracles-connect-the-balls.html)
- [CATEGORY WEB PROXY](https://studyquests.github.io/category-web-proxy.html)
- [DRIVER MASTER SIMULATOR](https://quizverses.pages.dev/driver-master-simulator.html)
- [MERGE SESAME](https://studyquests.github.io/merge-sesame.html)
- [BUILD A QUEEN 2025](https://studyquests.pages.dev/build-a-queen-2025.html)
- [OBBY TSUNAMI ESCAPE 1 BY CAR](https://studyplayings.web.app/obby-tsunami-escape-1-by-car.html)
- [HALLOWEEN MATCH TRIO](https://quizverses.pages.dev/halloween-match-trio.html)
- [CATEGORY TITANIUMNETWORK PROXIES](https://studyquests.github.io/category-titaniumnetwork-proxies.html)
- [SEAFARING MEMORY CHALLENGE](https://studyplaying.github.io/seafaring-memory-challenge.html)
- [KAWAII FRIENDS TILES MATCHER](https://studyquests.pages.dev/kawaii-friends-tiles-matcher.html)
- [EAT DONUTS](https://studyplaying.github.io/eat-donuts.html)
- [KEY QUEST](https://learnquester.github.io/key-quest.html)
- [AVOID THE SPIKES](https://studyquests.github.io/avoid-the-spikes.html)
- [SNIPER FOR BRAINROT](https://quizverses.pages.dev/sniper-for-brainrot.html)
- [BIG BLOCK BLAST](https://learnquester.github.io/big-block-blast.html)
- [HIDE AND SEEK HORROR ESCAPE](https://studyquests.pages.dev/hide-and-seek-horror-escape.html)
- [THE SPECIMEN ZERO](https://studyplayings.web.app/the-specimen-zero.html)
- [MAHJONG CONNECT SPOOKY](https://studyplayings.web.app/mahjong-connect-spooky.html)
- [WORLD SOCCER](https://studyplayings.web.app/world-soccer.html)
- [CATEGORY RACING DRIVING](https://studyquests.pages.dev/category-racing-driving.html)
- [STICKMAN ARMY TEAM BATTLE](https://studyplayings.web.app/stickman-army-team-battle.html)
- [FASHIONISTA CHRISTMAS EVE PARTY](https://quizverses.pages.dev/fashionista-christmas-eve-party.html)
- [FOOD SORT PUZZLE](https://studyplayings.web.app/food-sort-puzzle.html)
- [CATEGORY TITANIUMNETWORK](https://studyquests.github.io/category-titaniumnetwork.html)
- [HIGH HEELS COLLECT RUN](https://studyquests.github.io/high-heels-collect-run.html)
- [DEAD PARADISE](https://quizverses.pages.dev/dead-paradise.html)
- [CROWN CANNON](https://studyquests.github.io/crown-cannon.html)
- [ARCHERY RAGDOLL](https://learnquester.github.io/archery-ragdoll.html)
- [MUTANT RUN](https://studyplayings.pages.dev/mutant-run.html)
- [GEOMETRY WAVE HERO](https://studyquesthub.web.app/geometry-wave-hero.html)
- [COLOR HOOP SORT](https://studyquests.github.io/color-hoop-sort.html)
- [STICKMAN GUYS DEFENSE](https://studyplaying.github.io/stickman-guys-defense.html)
- [CATEGORY UNBLOCK](https://studyplaying.github.io/category-unblock.html)
- [CHESS ONLINE](https://studyquesthub.web.app/chess-online.html)
- [COLOR BLOCK JAM](https://studyquests.github.io/color-block-jam.html)
- [WOLF LIFE SIMULATOR](https://quizverses.pages.dev/wolf-life-simulator.html)
- [MOTO CABBIE SIMULATOR](https://studyquests.github.io/moto-cabbie-simulator.html)
- [IDLE BARBER SHOP](https://quizverses.pages.dev/idle-barber-shop.html)
- [CATEGORY SIMULATION 4](https://studyquests.github.io/category-simulation-4.html)
- [MY FIRE STATION WORLD](https://studyplaying.github.io/my-fire-station-world.html)
- [COUNTRYSIDE TRUCK DRIVE](https://studyquesthub.web.app/countryside-truck-drive.html)
- [MAKEUP TRENDS THEN AND NOW](https://studyquests.github.io/makeup-trends-then-and-now.html)
- [CATEGORY MAHJONG CONNECT](https://studyquests.pages.dev/category-mahjong-connect.html)
- [PIN PUZZLE SAVE THE SHEEP](https://studyplayings.pages.dev/pin-puzzle-save-the-sheep.html)
- [BUS DRIVER SIMULATOR 3D](https://quizverses.pages.dev/bus-driver-simulator-3d.html)
- [MINI GAMES CASUAL COLLECTION](https://studyquests.pages.dev/mini-games-casual-collection.html)
- [CATEGORY CUTE62](https://studyquests.github.io/category-cute62.html)
- [CATEGORY BUBBLE SHOOTER](https://studyquests.github.io/category-bubble-shooter.html)
- [GUN RACING](https://studyquests.github.io/gun-racing.html)
- [MOJICON EMOJI CONNECT](https://studyquests.pages.dev/mojicon-emoji-connect.html)
- [ORGANIZER MASTER](https://quizverses.pages.dev/organizer-master.html)
- [UNCLE BULLET 007](https://quizverses.pages.dev/uncle-bullet-007.html)
- [MATCH COLLECTION](https://studyquests.github.io/match-collection.html)
- [OFFROAD JEEP GAME SIMULATOR](https://studyplayings.pages.dev/offroad-jeep-game-simulator.html)
- [LUDO STAR](https://quizverses.pages.dev/ludo-star.html)
- [CATEGORY DESTROY](https://studyplaying.github.io/category-destroy.html)
- [DEATH BALL](https://studyplayings.web.app/death-ball.html)
- [HIGH HEELS 2](https://studyquesthub.web.app/high-heels-2.html)
- [CATEGORY CASUAL 13](https://studyquests.github.io/category-casual-13.html)
- [BRAINROT CLICKER](https://studyquests.github.io/brainrot-clicker.html)
- [TEACHER SIMULATOR](https://quizverses.pages.dev/teacher-simulator.html)
- [NETQUEL COM](https://quizverses.pages.dev/netquel-com.html)
- [PINTURILLO](https://studyplayings.pages.dev/pinturillo.html)
- [CATEGORY IDLE448](https://studyplaying.github.io/category-idle448.html)
- [KIOMET COM](https://quizverses.pages.dev/kiomet-com.html)
- [QUIZMANIA TRIVIA GAME](https://studyquests.github.io/quizmania-trivia-game.html)
- [OBBY VS ZOMBIES](https://learnquester.github.io/obby-vs-zombies.html)
- [JAILBREAK ESCAPE FROM PRISON](https://studyplaying.github.io/jailbreak-escape-from-prison.html)
- [BOXTERIA](https://studyplayings.web.app/boxteria.html)
- [CHICKEN SHOOTER IO](https://quizverses.pages.dev/chicken-shooter-io.html)
- [SPA EMPIRE](https://studyquests.pages.dev/spa-empire.html)
- [CATEGORY FPS 2](https://studyquests.github.io/category-fps-2.html)
- [BEARS VS ART](https://studyplayings.pages.dev/bears-vs-art.html)
- [FIRE AND WATER BIRDS](https://studyquests.github.io/fire-and-water-birds.html)
- [SLIDING PUZZLE](https://quizverses.pages.dev/sliding-puzzle.html)
- [PRACTICE ON ME](https://studyquests.github.io/practice-on-me.html)
- [CRAZY BUNNIES](https://learnquester.github.io/crazy-bunnies.html)
- [CATEGORY DRESS UP](https://studyquests.github.io/category-dress-up.html)
- [NUBIK IN THE MONSTER WORLD](https://quizverses.pages.dev/nubik-in-the-monster-world.html)
- [CATEGORY TOWER DEFENSE 2](https://studyquests.github.io/category-tower-defense-2.html)
- [SPRUNKI GETS SURGERY](https://studyplaying.github.io/sprunki-gets-surgery.html)
- [GELATINO](https://studyquesthub.web.app/gelatino.html)
- [COLOR WOOD ANIMAL JAM](https://quizverses.pages.dev/color-wood-animal-jam.html)
- [STREET TRAFFIC RACER](https://studyquests.github.io/street-traffic-racer.html)
- [CATEGORY MANAGEMENT209](https://studyquests.pages.dev/category-management209.html)
- [CUTE ANIMAL WORLD](https://studyquests.github.io/cute-animal-world.html)
- [CATEGORY CASUAL 4](https://studyquests.github.io/category-casual-4.html)
- [STICK FIGHT THE CHAOS](https://studyquests.pages.dev/stick-fight-the-chaos.html)
- [SQUID GAME PLAYGROUND SHOOTER](https://quizverses.pages.dev/squid-game-playground-shooter.html)
- [CRAZY ALIEN ADVENTURE](https://studyplaying.github.io/crazy-alien-adventure.html)
- [MY CASTLE MERGE STORY](https://studyplayings.web.app/my-castle-merge-story.html)
- [SUPERMARKET CASHIER SIMULATOR](https://studyquests.github.io/supermarket-cashier-simulator.html)
- [CATEGORY DESTROY256](https://studyplayings.pages.dev/category-destroy256.html)
- [GRAFFITI TAGS SPRAY PAINTING](https://studyquests.pages.dev/graffiti-tags-spray-painting.html)
