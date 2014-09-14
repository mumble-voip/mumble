#! /bin/bash
# Updates mumble_en.ts from source and adds a commit.
# Expects to be executed in src/mumble/
file="mumble_en.ts"

if ! [ -f $file ];
then
    echo "Could not find $file . Aborted."
    exit 1
fi;

echo "Translation update" > commitmessage.tmp
echo "" >> commitmessage.tmp
lupdate -no-ui-lines -disable-heuristic similartext -locations relative -no-obsolete mumble.pro -ts $file | tee -a commitmessage.tmp

# Duplicate single numerusform entries in mumble_en.ts to work around #1195
perl -pi.bak -e 's!(^\s*)(<numerusform></numerusform>)$!\1\2\2!' $file


git commit -F commitmessage.tmp $file
rm commitmessage.tmp

echo "Probably done. Please _manually_ check the commit before pushing"
