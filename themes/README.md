# Mumble Theme(s)

This directory contains the theme(s) Mumble ships with by default.

A theme consists of the following components:
- Style definitions (e.g. used colors, border widths, etc.) that are defined as QSS files (Qt's version of CSS)
- Icons
- Icon-Mapping that map a given name (alias) that is used in the application source code to a particular icon in the theme

The heart of a theme is its corresponding `.qrc` file. This file contains the Icon-Mapping and also a mapping for the QSS files. It is responsible of
bringing all required resources into the final binary and making these resources available under the name chosen in it.


## Editing icons

Editing icons is very straight forward. Most of our icons are vector graphics (SVGs) and that is what you should create when assembling a new icon.
These files can be viewed and edited using the Open Source and cross-platform tool [Inkscape](https://inkscape.org/).

By editing these SVG files with Inkscape, the files get bloated with a lot of unnecessary stuff though, so in order to clean them up (again), we use
the [SVGOMG web tool](https://jakearchibald.github.io/svgomg/). Just upload the SVG to their page and accept the default values. That usually produces
good results. Make sure to actually test that the icon still looks the same _inside Mumble_ after you have stripped it in this way.

As already mentioned, every icon needs to be referenced in the corresponding `.qrc` file in order to find its way into the final application. In there
you also have to create a unique name-alias for it. An entry in that file looks like this:
```xml
<file alias="nameToBeUsedInSourceCode">path/to/icon.svg</file>
```

In the application source code you can then refer to your new icon as `skin:nameToBeUsedInSourceCode`.


## Editing styles

Customizing the style of the theme is in principle also easy: The QSS files mostly behave like regular CSS files and as such they can be edited
manually. **This is not recommended** though.

The reason for that is that we auto-generate these style files using [Sass](https://sass-lang.com/) which is a CSS extension language.

Thus in order to make changes to the QSS files, you have to edit the Sass sources and then regenerate the sources using an appropriate Sass compiler.
We recommend the usage of the [Sass command line](https://sass-lang.com/install) tool.

For legacy reasons it is also possible to use the [Prepros](https://prepros.io/) tool. Its support for Mumble themes is no longer actively maintained
and might get removed in the future though.

The Sass sources currently live in the `Mumble/source/` directory. The most interesting files are hidden in the `Imports` directory. In there you will
find definitions for the general theme as well for the Dark and Lite variants of it. These are the files that you would usually edit in order to adapt
the style to your likings.

After you have made changes to the Sass files, you have to generate the respective QSS files from it. In order to do that you have to use the
`sass` CLI as follows:
```bash
sass <sass-file> <output-file>
```

The `<sass-file>` is one of the files found directly in the `source` directory whereas `<output-file>` is the corresponding QSS file one directory
above that. Starting from `Mumble/source/` such an invocation could look like this:
```bash
sass Lite.scss ../Lite.qss
```

To make sure you did not forget to regenerate the QSS for one of the variants, we recommend to simply always regenerate _all_ variants to make sure
they are all updated as needed.

