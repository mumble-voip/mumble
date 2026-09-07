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
- [LINE ON HOLE](https://learnaction.netlify.app/line-on-hole.html)
- [HIDDEN OBJECT ADVENTURE](https://eduquestsjp.pages.dev/hidden-object-adventure.html)
- [VALENTINE S DAY COUPLE DATE](https://learnaction.github.io/valentine-s-day-couple-date.html)
- [LOGIC SLIDE](https://eduquestkr.pages.dev/logic-slide.html)
- [ZENITH RUSH](https://brainquests.pages.dev/zenith-rush.html)
- [CATEGORY ADVENTURE 4](https://brainquests.pages.dev/category-adventure-4.html)
- [MARBLE BLAST](https://brainquests.pages.dev/marble-blast.html)
- [PRISON ESCAPE ONLINE](https://brainquests.pages.dev/prison-escape-online.html)
- [STICKMAN FIGHT PRO](https://eduquestspt.pages.dev/stickman-fight-pro.html)
- [ADDICTION SOLITAIRE](https://brainquests.pages.dev/addiction-solitaire.html)
- [PUZZLE BOX BRAIN FUN](https://eduquestspt.pages.dev/puzzle-box-brain-fun.html)
- [TWO SUPRA DRIFTERS](https://eduquests.pages.dev/two-supra-drifters.html)
- [CHRISTMAS SNOWBALL ARENA](https://learnaction.github.io/christmas-snowball-arena.html)
- [SURVIVAL ISLAND EVO](https://welearnaction.onrender.com/survival-island-evo.html)
- [SHOOT BLOCK RUSH 3D](https://brainquests.pages.dev/shoot-block-rush-3d.html)
- [CATEGORY MAHJONG CONNECT](https://eduquests.onrender.com/category-mahjong-connect.html)
- [THEME WORD SEARCH](https://eduquestspt.pages.dev/theme-word-search.html)
- [SCRAP CAR MERGE](https://eduquests.netlify.app/scrap-car-merge.html)
- [TRIANGLE WAY](https://eduquests.pages.dev/triangle-way.html)
- [FAST BALL JUMP](https://welearnaction.onrender.com/fast-ball-jump.html)
- [CITY GAS STATION SIMULATOR](https://brainquests.pages.dev/city-gas-station-simulator.html)
- [MINIGIANTS IO](https://learnaction.github.io/minigiants-io.html)
- [TAP GALLERY](https://eduquestspt.pages.dev/tap-gallery.html)
- [DREAMY HOME](https://learnaction.netlify.app/dreamy-home.html)
- [DIAMOND SOLITAIRE MAHJONG](https://eduquests.pages.dev/diamond-solitaire-mahjong.html)
- [THE PRISM CITY DETECTIVES](https://learnaction.github.io/the-prism-city-detectives.html)
- [COW JAM FARM PUZZLE](https://eduquestspt.pages.dev/cow-jam-farm-puzzle.html)
- [CARS MERGE](https://welearnaction.onrender.com/cars-merge.html)
- [BIG HEAD](https://ieduquests.web.app/big-head.html)
- [ENCHANTED MAHJONG SAGA](https://eduquestspt.pages.dev/enchanted-mahjong-saga.html)
- [CRAB GUARDS](https://learnaction.github.io/crab-guards.html)
- [KATANA](https://learnaction.netlify.app/katana.html)
- [KNIFEIO](https://brainquests.pages.dev/knifeio.html)
- [BATTLETABS](https://eduquests.github.io/battletabs.html)
- [M5 CITY DRIVER](https://learnaction.github.io/m5-city-driver.html)
- [STICK KILL 3D](https://learnaction.netlify.app/stick-kill-3d.html)
- [PANDA KITCHEN IDLE TYCOON](https://eduquests.netlify.app/panda-kitchen-idle-tycoon.html)
- [BUBBLE MERGE 2048](https://learnaction.github.io/bubble-merge-2048.html)
- [GUINEA PIGGY MATCHING](https://ieduquests.web.app/guinea-piggy-matching.html)
- [KOKO LOCO BLOCK BLAST](https://learnaction.netlify.app/koko-loco-block-blast.html)
- [NUMBER MASTER](https://brainquests.pages.dev/number-master.html)
- [TRUCKTOPOLIS COOKING CHAOS](https://eduquests.github.io/trucktopolis-cooking-chaos.html)
- [HIT KNOCK DOWN](https://eduquestspt.pages.dev/hit-knock-down.html)
- [CATEGORY ARENA254](https://eduquests.onrender.com/category-arena254.html)
- [CATEGORY BOOKMARK](https://eduquests.github.io/category-bookmark.html)
- [INDEX10](https://eduquests.onrender.com/index10.html)
- [TRICKY ARROW 2](https://eduquestspt.pages.dev/tricky-arrow-2.html)
- [RACING MASTER 3D](https://eduquests.pages.dev/racing-master-3d.html)
- [REFLECT BEAM LASER LOGIC](https://brainquests.pages.dev/reflect-beam-laser-logic.html)
- [MOW IT](https://eduquests.github.io/mow-it.html)
- [CATEGORY CASUAL 10](https://brainquests.pages.dev/category-casual-10.html)
- [2248 BLAST](https://eduquests.pages.dev/2248-blast.html)
- [BLOOM SORT 2 BEE PUZZLE](https://eduquestspt.pages.dev/bloom-sort-2-bee-puzzle.html)
- [SUPER SLIME BLACK HOLE](https://eduquestspt.pages.dev/super-slime-black-hole.html)
- [SPIDER NOOB OBSTACLE COURSE](https://eduquestspt.pages.dev/spider-noob-obstacle-course.html)
- [CRAZY BUBBLE BREAKER](https://brainquests.pages.dev/crazy-bubble-breaker.html)
- [MANSION STORY MATCH](https://eduquestspt.pages.dev/mansion-story-match.html)
- [GUN FEST](https://learnaction.github.io/gun-fest.html)
- [FIND RESTORE HIDDEN PUZZLE](https://eduquestspt.pages.dev/find-restore-hidden-puzzle.html)
- [DRAGON JOUST](https://eduquests.pages.dev/dragon-joust.html)
- [INDEX7](https://learnaction.github.io/index7.html)
- [OREPLICATION](https://ieduquests.web.app/oreplication.html)
- [TOCA AVATAR MY HOSPITAL](https://brainquests.pages.dev/toca-avatar-my-hospital.html)
- [MY ARCADE CENTER](https://eduquests.pages.dev/my-arcade-center.html)
- [SQUAD ASSEMBLER](https://eduquestspt.pages.dev/squad-assembler.html)
- [IDLE TRADE ROUTES](https://welearnaction.onrender.com/idle-trade-routes.html)
- [CATEGORY CASUAL 8](https://eduquests.onrender.com/category-casual-8.html)
- [DEEP IN THE LAB CHAPTER 1](https://welearnaction.onrender.com/deep-in-the-lab-chapter-1.html)
- [SUDOKU CLASSIC DAILY BRAIN PUZZLE](https://eduquests.pages.dev/sudoku-classic-daily-brain-puzzle.html)
- [SPIN SHOT SIEGE](https://eduquestspt.pages.dev/spin-shot-siege.html)
- [ARROW SORTING](https://ieduquests.web.app/arrow-sorting.html)
- [BUBBLE SHOOTER HD 3](https://eduquests.netlify.app/bubble-shooter-hd-3.html)
- [VALENTINES HIDDEN ALPHAWORDS](https://eduquests.pages.dev/valentines-hidden-alphawords.html)
- [POPPING CANDIES](https://eduquests.pages.dev/popping-candies.html)
- [GLITCH](https://eduquestspt.pages.dev/glitch.html)
- [AGENTS IO](https://ieduquests.web.app/agents-io.html)
- [BEAUTY WORLD AND FASHION STYLIST](https://learnaction.github.io/beauty-world-and-fashion-stylist.html)
- [MIRRORS PUZZLE](https://eduquests.pages.dev/mirrors-puzzle.html)
- [INDEX19](https://eduquestsfr.pages.dev/index19.html)
- [MAKE AMERICA GREAT AGAIN](https://eduquests.github.io/make-america-great-again.html)
- [COLOR BLOCK JAM 2](https://learnaction.github.io/color-block-jam-2.html)
- [KITTEN NEVER DIES](https://learnaction.netlify.app/kitten-never-dies.html)
- [INDEX17](https://learnaction.github.io/index17.html)
- [CATEGORY SCHOOL](https://ieduquests.web.app/category-school.html)
- [AVATAR MAKE UP](https://learnaction.github.io/avatar-make-up.html)
- [BLANKETS](https://eduquestspt.pages.dev/blankets.html)
- [MR RECKLESS CAR CHASE SIMULATOR](https://eduquests.github.io/mr-reckless-car-chase-simulator.html)
- [CATEGORY BUSINESS137](https://eduquests.pages.dev/category-business137.html)
- [ARROWS PUZZLE ESCAPE](https://brainquests.pages.dev/arrows-puzzle-escape.html)
- [MEMOJI](https://ieduquests.web.app/memoji.html)
- [BLOCK EATING SIMULATOR](https://learnaction.netlify.app/block-eating-simulator.html)
- [SCHOOL SIMULATOR MY SCHOOL](https://welearnaction.onrender.com/school-simulator-my-school.html)
- [ITALIAN ANIMALS CREATE YOUR OWN BRAINROT](https://eduquests.pages.dev/italian-animals-create-your-own-brainrot.html)
- [CATEGORY COOKING](https://ieduquests.web.app/category-cooking.html)
- [GUN BUILDER](https://eduquestspt.pages.dev/gun-builder.html)
- [TILEMAN IO](https://learnaction.github.io/tileman-io.html)
- [JEWEL MONSTERS](https://eduquestspt.pages.dev/jewel-monsters.html)
- [ANIMAL SWIPE](https://ieduquests.web.app/animal-swipe.html)
- [UPHILL RUSH 13](https://eduquestspt.pages.dev/uphill-rush-13.html)
- [SAVE THE CROP](https://ieduquests.web.app/save-the-crop.html)
- [TCG CARD CLICKER](https://eduquestspt.pages.dev/tcg-card-clicker.html)
- [MATCH TEN NUMBER PUZZLE](https://eduquests.pages.dev/match-ten-number-puzzle.html)
- [INDEX16](https://eduquests.pages.dev/index16.html)
- [LOVIE CHICS COACHELLA FESTIVAL](https://eduquests.pages.dev/lovie-chics-coachella-festival.html)
- [X TO Y ALMOST IMPOSSIBLE](https://brainquests.pages.dev/x-to-y-almost-impossible.html)
- [OM NOM RUN](https://eduquestspt.pages.dev/om-nom-run.html)
- [CATEGORY FLASH](https://eduquests.pages.dev/category-flash.html)
- [LABUBU MERGE CLICKER](https://ieduquests.web.app/labubu-merge-clicker.html)
- [CATEGORY BOXING](https://eduquests.pages.dev/category-boxing.html)
- [TOWER DEFENSE](https://eduquests.pages.dev/tower-defense.html)
- [TENTRIX](https://learnaction.netlify.app/tentrix.html)
- [EGGY BEATS](https://eduquests.pages.dev/eggy-beats.html)
- [COLOR 3D BUMP IT UP](https://eduquests.pages.dev/color-3d-bump-it-up.html)
- [CATEGORY POINT AND CLICK123](https://learnaction.github.io/category-point-and-click123.html)
- [CATEGORY CONTROLLER 2](https://eduquests.pages.dev/category-controller-2.html)
- [MAGES SECRET](https://welearnaction.onrender.com/mages-secret.html)
- [FIND THE VAMPIRE](https://ieduquests.web.app/find-the-vampire.html)
- [CATEGORY SECURLY](https://ieduquests.web.app/category-securly.html)
- [MEGA RAMP BIKE RACING TRACKS](https://eduquests.netlify.app/mega-ramp-bike-racing-tracks.html)
- [MECH MONSTER ARENA](https://eduquests.pages.dev/mech-monster-arena.html)
- [DESIGN WITH ME SUPERHERO TUTU OUTFITS](https://eduquests.pages.dev/design-with-me-superhero-tutu-outfits.html)
- [MAZEAN COM](https://eduquestspt.pages.dev/mazean-com.html)
- [DRAGON HUNTER](https://eduquests.netlify.app/dragon-hunter.html)
- [PET FALL](https://welearnaction.onrender.com/pet-fall.html)
- [WHEEL IN THE FACE](https://eduquests.pages.dev/wheel-in-the-face.html)
- [CATEGORY MINECRAFT](https://eduquestspt.pages.dev/category-minecraft.html)
- [CATEGORY BRAIN261](https://eduquestspt.pages.dev/category-brain261.html)
- [INDEX16](https://eduquests.onrender.com/index16.html)
- [CATEGORY CAN T STOP PLAYING212](https://eduquestspt.pages.dev/category-can-t-stop-playing212.html)
- [INDEX33](https://eduquests.pages.dev/index33.html)
