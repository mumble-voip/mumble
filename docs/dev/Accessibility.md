# Accessiblity

## Introduction

In the software context, [accessibility features](https://en.wikipedia.org/wiki/Computer_accessibility)
(often shortened to "a11y") are considerations and tools meant to reduce the hassle of running
a specific application by users with disabilities.

Most often, you will find features specifically crafted or tweaked for visually impaired users,
but other disabilities such as cognitive or motor impairments should also be considered.

## General concepts

The most basic approach to accessibility in software is to use empathy. Ask yourself: "If I had
%disability%, would I still be able to use this application without much effort?". If the
answer is "No", you might want to start investigating on how to improve that situation.

Some "accessibility features" are not even that special, but more of a common sense decision. For example:

* "Use a high contrast between text and background such that users with visual impairments are able to read it"
* "Do not use a color scheme which is hard to read for users with color blindness"
* "Avoid rapidly changing UI elements as to not confuse users who need more time to process and understand those elements"

If your application needs to function in a way that is inherently inaccessible to some users,
consider offering an alternative accessible way for that function or a similar feature.

Make your application keyboard navigable for those, who struggle using a mouse for
various reasons.

### Tools

For some disabilities, external assistive tools exist which help to make any application more
accessible. A prime example for such a tool is a [screen reader](https://en.wikipedia.org/wiki/Screenreader)
mostly used by visually impaired users. Screen readers are able to read UI elements on the screen, once the users
selects them with the mouse or the keyboard. To make screen readers work though, sometimes the
developer needs to add additional information. That information is processed by the screen reader to give
context to certain UI elements, which are normally communicated visually.

## Qt

Qt, the UI toolkit Mumble uses, provides [accessibility features](https://doc.qt.io/qt-5/accessible.html)
and they are fine for 90% of use-cases. The ``accessibleName`` and ``accessibleDescription`` field found
in every ``QWidget`` help to convey much of the needed information for screen readers.

However, in certain situations you may find that Qt does not offer neat ways to implement everything that
is necessary for a good, accessible experience. In those cases, especially when designing custom or
complex ``QWidget``s, you will find yourself tinkering and testing quite a while until the result is
somewhat acceptable.

Feel free to ask for advice at any time.

## Checklist for UI changes in Mumble

When changing a UI element or implementing a new one, make sure that the changes meet all the following criteria:

* [ ] The change **MUST NOT** break tab navigation
  * Make sure that keyboard navigation using tab AND back tab still works
  * Make sure any new interactive UI element is reachable using tab AND back tab
  * You **MUST NOT** create a focus trap, from which the user can not escape via keyboard navigation

* [ ] The change itself **MUST** be keyboard navigable
  * Space and arrow keys **MAY** be used on some elements such as sliders and combo boxes

* [ ] All UI dialogs **MUST** have a sensible tab order
  * Make sure that the tab order roughly matches the visual layout
  * Make exceptions for the tab order, when it makes sense to do so (e.g. focus checkboxes to enable a feature before the options for that feature, independent of the visual order)
  * Changing the tab order can be accomplished using the QtDesigner -> "Edit" -> "Edit Tab Order"

* [ ] Interactive UI elements **MUST** have an ``accessibleName``
  * Generally, buttons and other elements with inherent labels already use those labels as ``accessibleName``. That is usually enough!
  * Overwrite the ``accessibleName`` of buttons and other elements of inherent labels **ONLY** when there is any contextual advantage
  * Buttons with an icon instead of text **MUST** set an ``accessibleName``
  * Sliders, combo boxes, and other elements without an inherent label **MUST** set an ``accessibleName``

* [ ] Labeled interactive UI elements **MUST** correctly set their buddy label
  * Qt offers a way to attach a widget to a label, providing something equivalent to an ``accessibleName``. This is called a "buddy"
  * If you place a label next to an interactive UI element, make sure to set it as buddy using QtCreator -> "Edit" -> "Edit Buddies"

* [ ] Interactive UI elements **SHOULD** have an ``accessibleDescription``, when it makes sense to do so
  * The ``accessibleDescription`` field can be used to explain context to a user that is otherwise only communicated visually
  * The ``accessibleDescription`` field can be used to explain a feature (e.g. voice activity detection) that is otherwise harder to grasp by just reading the name
  * Think of the ``accessibleDescription`` field as the tooltip or "What's this?" for screen readers. Some screen readers might even fall back to the tooltip, if no description is set

* [ ] Abbreviations that are application specific **MUST NOT** be used in ``accessibleName`` or ``accessibleDescription``
  * Generally well known abbreviations such as HTTP, TCP are fine
  * For example, use "voice activity detection" instead of "VAD"

* [ ] ``accessibleName`` and ``accessibleDescriptions`` **MUST** be set in the ``.ui`` files, e.g. using the QtCreator
  * Exception: UI elements which change state and context dynamically at runtime **MAY** be set in code

* [ ] Additional visual context **MUST** be made available to screen reader users
  * Sometimes information is displayed as a plain label or a series of plain labels. Plain labels can not be focused using a screen reader, and therefore the information has to be made available differently
  * For example, the ``ServerInformation.ui`` uses the ``AccessibleGroupBox`` class instead of ``QGroupBox`` to solve that problem
  * Sometimes it might be enough to include that information in the ``accessibleDescription`` of another element

* [ ] New status for users or channels **MUST** be made accessible using the ``userToText`` and ``channelToText`` functions in ``Accessibility.cpp``

* [ ] New actions **MUST** be made available through a menu item. Only adding a toolbar button is not enough. The toolbar is not accessible.

* [ ] All menu items **MUST** follow state-of-the-art design concepts
  * https://learn.microsoft.com/en-us/windows/win32/uxguide/cmd-menus
  * https://developer.apple.com/design/human-interface-guidelines/menus
  * https://develop.kde.org/hig/components/navigation/menubar/

* [ ] Sliders **MAY** use the ``SemanticSlider`` class as a base to enhance the screenreading experience

* [ ] UI elements **MAY** inform the user about other UI elements
  * Sometimes elements depend on other elements. If this dependency is only available visually, an element may say something like "Use the '%buttonname%' button to open a dialog for this"


## Final considerations

This text and checklist was written with the best intentions and the current understanding of accessibility
by the development team. This should in no way be considered final or absolute and there is always room
for improvement. Always use common sense when developing.

Also, feel free to submit corrections, additions or updates to this text or checklist. As technology changes,
our current understanding might become outdated at some point.
