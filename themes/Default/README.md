A **Mumble theme in Lite and Dark variants**, which is **distributed as the default skin in the Mumble client**.

This theme is a derivative of the [MetroMumble skin](https://github.com/xPoke/MetroMumble).

# Skinning

**Do not modify the qss files directly. Read below.**

To change icons and other graphics, simply change the SVG files of the theme (with an editor such as [Inkscape](https://inkscape.org/en/) or Adobe Illustrator).

See the [wiki article on Mumble skinning](https://wiki.mumble.info/wiki/Skinning).

The theme is written in [Sass](https://en.wikipedia.org/wiki/Sass_%28stylesheet_language%29), a language that processes into CSS (or [QSS](http://doc.qt.io/qt-4.8/stylesheet.html) in this case).

If you want to make your own skin based on it, install a Sass processor such as [Prepros](http://alphapixels.com/prepros/). A Prepros project file is included, so simply add the `mumble-theme` folder as a project.

The file `source/Imports/Base Theme.scss` contains the actual theme QSS, but it is **not recommended** to modify.

In future versions you might want to swap `Base Theme.scss` to a newer version, so it will be easier to update your theme in the future if you leave it alone. 

Instead, you should edit either the **`Lite.scss` or `Dark.scss` files in the `source` folder**, and add your own CSS at the end of the file.

You can also add your **own variants** in the `source/Imports` folder, which contain the color definitions.

When you process these files in Prepros, it will output the complete skin to the **qss files in the parent folder**. If something didn't work out
quite right, it can also happen that Prepros will output **css** files in the `source` folder. In that case you can simply replace the qss files in
the root folder with the generated css files (the syntax is actually the same - only the file extension differs).

The **OSX variant** can also be found in the `source` folder, which imports the regular source files but adds a small [OSX fix](https://github.com/xPoke/MetroMumble/issues/4) and modifies font sizes.

Have fun!
