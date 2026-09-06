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
- [SOLITAIRE KLONDIKE](https://iskillquest.pages.dev/solitaire-klondike.html)
- [PIN MASTER SCREW PUZZLE QUEST BRAIN GAMES](https://quizverses.github.io/pin-master-screw-puzzle-quest-brain-games.html)
- [SQUID GAME HUNTER](https://quizverses.pages.dev/squid-game-hunter.html)
- [RUN FROM BABA YAGA](https://learnquester.github.io/run-from-baba-yaga.html)
- [CATEGORY FARMING87](https://studyquesthub.web.app/category-farming87.html)
- [TILE PAIR MATCH](https://studyquesthub.web.app/tile-pair-match.html)
- [WORDS WITH OWL](https://studyquests.github.io/words-with-owl.html)
- [STUDENT AND TEACHER](https://studyplaying.github.io/student-and-teacher.html)
- [PRINCESSES AT HORROR SCHOOL](https://studyplaying.github.io/princesses-at-horror-school.html)
- [INDEX18](https://studyquests.pages.dev/index18.html)
- [NEON BLAST](https://studyplaying.github.io/neon-blast.html)
- [SCREW IT OUT JAM MATCHING COLORED SCREWS](https://studyquests.github.io/screw-it-out-jam-matching-colored-screws.html)
- [CATEGORY TOWER DEFENSE 2](https://studyplaying.github.io/category-tower-defense-2.html)
- [CATEGORY FARMING87](https://studyquests.pages.dev/category-farming87.html)
- [CHECKERS DELUXE EDITION](https://studyplaying.github.io/checkers-deluxe-edition.html)
- [TILE LIVING](https://quizverses.github.io/tile-living.html)
- [ONU LIVE](https://studyquests.github.io/onu-live.html)
- [DRAW BRIDGE CHALLENGE](https://studyquests.github.io/draw-bridge-challenge.html)
- [GOING BALLS ADVENTURE 2](https://studyquests.github.io/going-balls-adventure-2.html)
- [GRIDDLERS DELUXE](https://studyquests.pages.dev/griddlers-deluxe.html)
- [CATEGORY SOLITAIRE](https://studyplaying.github.io/category-solitaire.html)
- [MERGE BALLS NEW YEARS TOYS IN 3D](https://quizverses.github.io/merge-balls-new-years-toys-in-3d.html)
- [INDEX8](https://quizverses.pages.dev/index8.html)
- [QUACKVENTURE](https://studyquests.github.io/quackventure.html)
- [COSMIC AVIATOR](https://studyplaying.github.io/cosmic-aviator.html)
- [CATEGORY OBBY](https://quizverses.pages.dev/category-obby.html)
- [CATEGORY IBOSS](https://studyplaying.github.io/category-iboss.html)
- [CATEGORY RUNNING107](https://studyplaying.github.io/category-running107.html)
- [GIRL RESCUE DRAGON OUT](https://studyquests.github.io/girl-rescue-dragon-out.html)
- [SOLITAIRE FARM SEASONS 3](https://studyplayings.pages.dev/solitaire-farm-seasons-3.html)
- [BOYFRIEND FOR HIRE](https://learnquester.github.io/boyfriend-for-hire.html)
- [CATEGORY BUILDING179](https://studyplaying.github.io/category-building179.html)
- [BUNNY BLOX](https://quizverses-9d2f2.web.app/bunny-blox.html)
- [CATEGORY BIKE63](https://quizverses.github.io/category-bike63.html)
- [COLLEGE GIRL COLORING DRESS UP](https://studyplayings.pages.dev/college-girl-coloring-dress-up.html)
- [PURRFECT BAKERY](https://studyquests.github.io/purrfect-bakery.html)
- [MAGIC PIANO MUSIC](https://studyquests.github.io/magic-piano-music.html)
- [VOLLEY BEAN](https://studyquests.github.io/volley-bean.html)
- [PYRAMIDZ2](https://studyplaying.github.io/pyramidz2.html)
- [FRUIT CATCHER](https://studyquests.github.io/fruit-catcher.html)
- [INDEX2](https://studyplaying.github.io/index2.html)
- [CATEGORY ANIMAL216](https://quizverses.github.io/category-animal216.html)
- [CATEGORY ARENA255](https://studyplaying.github.io/category-arena255.html)
- [SWAT FORCE VS TERRORISTS](https://studyquests.github.io/swat-force-vs-terrorists.html)
- [CATEGORY BASKETBALL 2](https://thelearnquester.web.app/category-basketball-2.html)
- [BRAINROT BOING BOING MERGE](https://studyplayings.web.app/brainrot-boing-boing-merge.html)
- [BLOCK TNT BLAST](https://studyquests.github.io/block-tnt-blast.html)
- [JEWEL COLORING](https://studyquests.github.io/jewel-coloring.html)
- [POLYGON SPACE](https://quizverses.github.io/polygon-space.html)
- [PUZZLE LAB](https://quizverses.github.io/puzzle-lab.html)
- [INDEX15](https://studyplaying.github.io/index15.html)
- [WITCH CRAFT POTION SORT](https://studyquests.github.io/witch-craft-potion-sort.html)
- [HIDDEN OBJECTS BAKERY](https://learnquester.github.io/hidden-objects-bakery.html)
- [STUNT FURY](https://studyquests.github.io/stunt-fury.html)
- [BOMB HEAD HOT POTATO](https://studyquests.github.io/bomb-head-hot-potato.html)
- [CAT MATCH 3](https://studyplayings.pages.dev/cat-match-3.html)
- [WORMS ZONE](https://studyplayings.web.app/worms-zone.html)
- [CATEGORY PUZZLE 6](https://studyquests.github.io/category-puzzle-6.html)
- [CATEGORY PUZZLE 4](https://studyquests.github.io/category-puzzle-4.html)
- [HEXON RUSH](https://studyquests.github.io/hexon-rush.html)
- [CATEGORY IDLE](https://studyquests.github.io/category-idle.html)
- [NINJA OBBY PARKOUR](https://studyplaying.github.io/ninja-obby-parkour.html)
- [STICK HERO BATTLE](https://learnquester.github.io/stick-hero-battle.html)
- [THE TRENDY MERMAID](https://studyquests.github.io/the-trendy-mermaid.html)
- [CHILDCARE MASTER ONLINE](https://studyplayings.web.app/childcare-master-online.html)
- [MOTO X3M DEAD AHEAD](https://studyplayings.web.app/moto-x3m-dead-ahead.html)
- [WOOD HEXA FACTORY](https://quizverses-9d2f2.web.app/wood-hexa-factory.html)
- [KILLER ESCAPE HUGGY EXTREME](https://learnquester.github.io/killer-escape-huggy-extreme.html)
- [CATEGORY PREMIUM PERKS71](https://studyquests.github.io/category-premium-perks71.html)
- [FREECELL](https://studyplayings.pages.dev/freecell.html)
- [POPCORN STACK](https://studyquests.github.io/popcorn-stack.html)
- [CINEMA EMPIRE IDLE TYCOON](https://studyquests.github.io/cinema-empire-idle-tycoon.html)
- [CATEGORY TITANIUMNETWORK](https://quizverses-9d2f2.web.app/category-titaniumnetwork.html)
- [MIND GAMBIT](https://quizverses.github.io/mind-gambit.html)
- [CATEGORY SOLITAIRE](https://thelearnquester.web.app/category-solitaire.html)
- [CATEGORY BATTLE ROYALE](https://quizverses.github.io/category-battle-royale.html)
- [DIAMOND MOSAIC](https://studyplayings.web.app/diamond-mosaic.html)
- [CATEGORY FOOD](https://quizverses.pages.dev/category-food.html)
- [FIND THE CAT CAT SEARCH](https://studyquests.github.io/find-the-cat-cat-search.html)
- [ROPE COLOR SORT 3D](https://studyplayings.pages.dev/rope-color-sort-3d.html)
- [CATEGORY SOLDIER11](https://studyplaying.github.io/category-soldier11.html)
- [INDEX21](https://studyplaying.github.io/index21.html)
- [LIGHT BULB PUZZLE](https://studyplaying.github.io/light-bulb-puzzle.html)
- [STICKMAN MEGA BOSS BATTLES](https://learnquester.github.io/stickman-mega-boss-battles.html)
- [PERFECT SHOT](https://studyplayings.web.app/perfect-shot.html)
- [KOMPOTS KITCHEN](https://quizverses.github.io/kompots-kitchen.html)
- [DALGONA MASTER](https://studyquests.pages.dev/dalgona-master.html)
- [CATEGORY DEFENSE176](https://studyquests.github.io/category-defense176.html)
- [SCREW JAM FUN PUZZLE GAME](https://quizverses.pages.dev/screw-jam-fun-puzzle-game.html)
- [SLIME FARM](https://studyplayings.pages.dev/slime-farm.html)
- [INDEX5](https://quizverses-9d2f2.web.app/index5.html)
- [CATEGORY HERO72](https://quizverses.pages.dev/category-hero72.html)
- [CATEGORY PIXEL313](https://quizverses-9d2f2.web.app/category-pixel313.html)
- [GALAXY CLICKER](https://studyquests.github.io/galaxy-clicker.html)
- [MURDER CASE CLUE 3D](https://quizverses.pages.dev/murder-case-clue-3d.html)
- [JUNGLE SOLITAIRE](https://quizverses.pages.dev/jungle-solitaire.html)
- [CHICKEN WARS MERGE GUNS](https://quizverses.github.io/chicken-wars-merge-guns.html)
- [CATEGORY STRATEGY](https://studyplaying.github.io/category-strategy.html)
- [CATEGORY COOKING46](https://studyquests.github.io/category-cooking46.html)
- [CATEGORY SORTING44](https://thelearnquester.web.app/category-sorting44.html)
- [CATEGORY STICKMAN175](https://thelearnquester.web.app/category-stickman175.html)
- [ARCHERY MASTER](https://studyplayings.pages.dev/archery-master.html)
- [IDLE PET](https://studyquests.pages.dev/idle-pet.html)
- [CRAZY BUBBLE BREAKER](https://quizverses-9d2f2.web.app/crazy-bubble-breaker.html)
- [BUBBLE SHOOTER VINTAGE](https://quizverses-9d2f2.web.app/bubble-shooter-vintage.html)
- [SPRUNKI QUIZ](https://studyplayings.web.app/sprunki-quiz.html)
- [CATEGORY AGILITY](https://learnquester.github.io/category-agility.html)
- [CLAY CRAFT TYCOON](https://quizverses-9d2f2.web.app/clay-craft-tycoon.html)
- [CATEGORY ARENA255](https://quizverses.github.io/category-arena255.html)
- [THE COUNTERFEIT BANK](https://studyquests.github.io/the-counterfeit-bank.html)
- [CONSTRUCTION SET 3D BUILDER](https://studyplaying.github.io/construction-set-3d-builder.html)
- [PHYSICS BALLS](https://studyquesthub.web.app/physics-balls.html)
- [CATEGORY STICKMAN 2](https://thelearnquester.web.app/category-stickman-2.html)
- [CATEGORY 3D1 371](https://quizverses-9d2f2.web.app/category-3d1-371.html)
- [FESTIVAL VIBES MAKEUP](https://studyplaying.github.io/festival-vibes-makeup.html)
- [INDEX10](https://thelearnquester.web.app/index10.html)
- [SITEMAP](https://quizverses-9d2f2.web.app/sitemap.html)
- [SNIPING ALIENS](https://studyplaying.github.io/sniping-aliens.html)
- [CAKE LINK MASTER](https://studyplayings.web.app/cake-link-master.html)
- [CATEGORY DRESS UP](https://studyquests.github.io/category-dress-up.html)
- [NO PAIN NO GAIN RAGDOLL SANDBOX](https://quizverses.pages.dev/no-pain-no-gain-ragdoll-sandbox.html)
- [CATEGORY DRAGON](https://studyplaying.github.io/category-dragon.html)
- [SOLAR SMASH](https://studyplaying.github.io/solar-smash.html)
- [FASHION STYLIST SALON MAKEOVER](https://quizverses.pages.dev/fashion-stylist-salon-makeover.html)
- [TILE CONNECT CLUB](https://quizverses-9d2f2.web.app/tile-connect-club.html)
- [CHILL CLICKER](https://studyplayings.web.app/chill-clicker.html)
- [STACKTRIS 2048](https://quizverses.pages.dev/stacktris-2048.html)
- [PANDA KITCHEN IDLE TYCOON](https://studyquesthub.web.app/panda-kitchen-idle-tycoon.html)
- [CATEGORY CASUAL 10](https://studyplaying.github.io/category-casual-10.html)
- [CATEGORY DRESS UP 3](https://studyquests.github.io/category-dress-up-3.html)
