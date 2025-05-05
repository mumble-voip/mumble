# Backporting translations

Usually when a new stable release is created (and thus there is a new, separate code-branch for it that remains feature-frozen) the
translations into the various languages is incomplete. However after the release of the new stable release, translations tend to be updated step by
step, rendering the translations in the feature-frozen code-branch obsolete. Thus it is necessary to update the translations in the feature-frozen
branch with the latest translations from the active development branch (`master`).

These translation changes can't just be copied over to the other branch since it is very likely that the master branch already contains new strings
that do not apply to the older stable-release (thus their translations should not be backported).

In order to only backport those translations that have a corresponding source string that is actually used in the respective release, the
`backportTranslations.py` can be used. It will take care of figuring out which translations to backport and which to leave untouched.

It can be invoked as
```
python3 backportTranslations.py <from_branch>
```
while having the target branch for the backports checked out. `<from_branch>` is the name of the local branch to fetch the translations from (usually
`master`).

Thus a workflow could look like this:
1. Create a new branch based on the stable release's branch
2. Update your local master branch to include the latest (translation) changes of the upstream master branch
3. Checkout your newly created branch
4. Execute the script as `python3 backportTranslations.py master`
5. Add the changes created by the script
6. Commit and push
7. Create pull request

