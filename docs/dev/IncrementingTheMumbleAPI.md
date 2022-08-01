# Incrementing the Mumble API version

## Overview
[Plugins](/docs/dev/plugins/) are used to extend the Mumble client with custom functionality. They are compiled against multiple [header files](/plugins/) which
contain all function and structure signatures that can be used by the plugin. If you are commiting breaking API changes (such as additions or deletions) to these function signatures
on the Mumble side, the API version will probably have to be increased. This is done to ensure plugins will have access to these functions when they
are compiled against the new API version, while old plugins, using previous API versions, will still work.

## Instructions
If your commit includes breaking API changes, the following steps have to be followed:

1) Create a new ``Mumble_API_v_<MA>_<MI>_x.h`` file in [plugins/](/plugins/).
	* Replace ``<MA>`` with the API major version and ``<MI>`` with the API minor version.
	* Usually, you will have to increase the ``<MI>`` version by 1 compared to the previous latest version. So if the previous API string was ``1.2.0``, you would probably want to increase the second number by one and create the file ``MumbleAPI_v_1_3_x.h``.
	* The last "patch" digit of the version string is reserved for non-breaking API changes such as small fixes. Since non-breaking changes do not require a major or minor API increment, we do not worry about the "patch" digit in this document.
1) Make sure you correctly set the ``MUMBLE_PLUGIN_API_MAJOR_MACRO``, ``MUMBLE_PLUGIN_API_MINOR_MACRO`` and [include guard](https://en.wikipedia.org/wiki/Include_guard) to the new API version. 
	* You probably want to set the ``MUMBLE_PLUGIN_API_PATCH_MACRO`` to ``0``.
1) Rename the ``struct`` within the file to match the new API version string.
	* There is a ``typedef`` at the end of the file which has to use the new ``struct`` name, too.
1) Make your desired changes to the function signatures contained in the header file.
1) Open the [MumbleAPI_structs.h](/src/mumble/MumbleAPI_structs.h) file and add your new API header file as first include.
	* Move the previous first include, the last most recent API version, into the ``NO_AUXILIARY_DEFINITIONS`` section.
	* This will make sure auxiliary variables and macros always contain the most recent version on the mumble side.
1) Add an ``else`` case to the [Plugin.cpp](/src/mumble/Plugin.cpp) file in the ``Plugin::init`` method.
	* You will want to return the newly created ``struct``, if the API version requested by the plugin matches your new one.
1) Create a new function signature in [API.h](/src/mumble/API.h). It should return your new ``struct`` and be of the name ``getMumbleAPI_v_<MA>_<MI>_x``.
1) Amend the previous ``API_v_<MA>_x_x.cpp`` file, or create a new one in [this folder](/src/mumble/).
	* For the foreseeable future, reuse the previous cpp file. Creating a new cpp file will probably only be useful on extensive API re-designs.
	* If you have created a new ``API_v_<MA>_x_x.cpp``, you must add it to the [CMakeLists.txt](/src/mumble/CMakeLists.txt).
1) Implement the ``getMumbleAPI_v_<MA>_<MI>_x`` function inside the ``API_v_<MA>_x_x.cpp`` file and return a struct with all functions you want to expose in your API version.
1) When adding a new function or new function signature, copy an existing "C FUNCTION WRAPPER" inside the ``API_v_<MA>_x_x.cpp`` file. That wrapper should then call your new function.
1) Implement your new API changes in the ``API_v_<MA>_x_x.cpp`` file.
	* If you change the signature of an old function, it is probably best to call the new function in the old implementation with default values.
	* This way, old plugins will still be working more or less as expected.
