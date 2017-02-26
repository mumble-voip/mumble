#!/usr/bin/env bash

# this script cleans svg files with SVGCleaner: http://sourceforge.net/projects/svgcleaner/
# it is assumed svgcleaner-cli in in your PATH, and this script is run in the root of the mumble project
# it is also assumed you are using GNU find.

find . -name "*.svg" -type f -exec svgcleaner-cli {} {} \
	--remove-invisible-elts \
	--remove-empty-containers \
	--apply-transforms-to-defs \
	--remove-duplicated-defs \
	--ungroup-containers \
	--merge-gradients \
	--remove-unreferenced-ids \
	--trim-ids \
	--remove-fill-props \
	--simplify-transform-matrix \
	--remove-unused-xlinks \
	--apply-transforms-to-shapes \
	--group-elts-by-styles \
	--convert-to-relative \
	--remove-unneeded-symbols \
	--remove-tiny-segments \
	--convert-segments \
	--colors-to-rrggbb \
	--rrggbb-to-rgb \
	--convert-basic-shapes \
	--compact-output \
	--transform-precision=5 \
	--coordinates-precision=3 \
	--attributes-precision=3 \
	--sort-defs \
	--remove-inkscape-atts \
	--remove-default-atts \
	--remove-sodipodi-atts \
	--remove-ai-atts \
	--remove-corel-atts \
	--remove-outside-elts \
	--remove-unused-defs \;
