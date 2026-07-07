// MUMBLE-TFAR
// Project constants for the "Arma 3: Storm of The Galaxy" community build.
// Everything project-specific (server, repository, versioning) lives here.

#ifndef MUMBLE_MUMBLE_TFAR_STORMBRANDING_H_
#define MUMBLE_MUMBLE_TFAR_STORMBRANDING_H_

// Version of this mumble-tfar build. Compared against the "tag_name" of the
// latest GitHub release (leading 'v' is stripped). Bump on every release tag.
#define STORM_TFAR_VERSION "1.0.0"

#define STORM_PROJECT_NAME "Arma 3: Storm of The Galaxy"
#define STORM_TITLE_POSTFIX "Storm of The Galaxy (TFAR)"

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

#endif // MUMBLE_MUMBLE_TFAR_STORMBRANDING_H_
