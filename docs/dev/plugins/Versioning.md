# Versioning

Mumble's plugin framework uses _semantic versioning_ also known as [SemVer](https://semver.org/). That means that a version always has the format
MAJOR.MINOR.PATCH.

Versions that differ in their major version number mean that these versions are _incompatible_ with one another. There was at least one breaking
change between the two versions.

If however the minor version number changes, the versions are still compatible but the higher minor version number indicates new features that were
not available in the old version. However the versions are still compatible on the set of old features.

The patch version number is increased if something in the existing feature sets is changed without affecting the API. Therefore plugins usually don't
have to care about the patch version.

Note that plugins are **expected to follow this scheme as well**.

