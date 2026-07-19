// MUMBLE-TFAR
// Project constants for the "Arma 3: Storm of The Galaxy" community build.
// Everything project-specific (server, repository, versioning) lives here.

#ifndef MUMBLE_MUMBLE_TFAR_STORMBRANDING_H_
#define MUMBLE_MUMBLE_TFAR_STORMBRANDING_H_

// Version of this mumble-tfar build. Compared against the "tag_name" of the
// latest GitHub release (leading 'v' is stripped). Bump on every release tag.
// The client also announces it to the server in the Version message as
// " storm-voice/<version>" appended to the release string; mumble-tfar servers
// reject clients whose version does not match "tfarrequiredversion".
// The Windows installer (MSI/EXE) version is derived from this value with the
// major component offset by one (see the packaging section of
// src/mumble/CMakeLists.txt), so releases upgrade each other and the legacy
// "1.7.x" product registrations cleanly.
#define STORM_TFAR_VERSION "1.5.0"

// Product name. "Mumble" is only used for nominative references ("based on
// Mumble", protocol compatibility) — the product itself is branded Storm Voice.
#define STORM_APP_NAME "Storm Voice"
#define STORM_APP_NAME_FULL "Storm Voice — TFAR"

#define STORM_PROJECT_NAME "Arma 3: Storm of The Galaxy"
#define STORM_TITLE_POSTFIX "Storm of The Galaxy"

// Default community server (seeded into the favorites on first launch).
#define STORM_SERVER_NAME "Storm of The Galaxy"
#define STORM_SERVER_HOST "stormofgalaxy.com"
#define STORM_SERVER_PORT 64738
#define STORM_SERVER_URL "mumble://stormofgalaxy.com/"

// Project repository — releases and documentation.
#define STORM_GITHUB_REPO "StormofTheGalaxy/mumble-tfar"
#define STORM_GITHUB_URL "https://github.com/StormofTheGalaxy/mumble-tfar"
#define STORM_RELEASES_URL "https://github.com/StormofTheGalaxy/mumble-tfar/releases/latest"
#define STORM_RELEASES_API "https://api.github.com/repos/StormofTheGalaxy/mumble-tfar/releases/latest"
#define STORM_DOCS_URL "https://github.com/StormofTheGalaxy/mumble-tfar/blob/master/docs/TFAR-SETUP.md"
// Community favorites list ("favorites.json" in the repository root); synced
// into the favorites database on every start (see StormFavorites).
#define STORM_FAVORITES_URL "https://raw.githubusercontent.com/StormofTheGalaxy/mumble-tfar/master/favorites.json"
#define STORM_MACOS_AUDIO_URL "https://github.com/StormofTheGalaxy/mumble-tfar/blob/master/docs/MACOS-WINE-AUDIO.md"

#endif // MUMBLE_MUMBLE_TFAR_STORMBRANDING_H_
