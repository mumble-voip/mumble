# IDE integration

## Visual Studio

[Visual Studio supports cmake projects](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=vs-2019) when you install the
*C++ cmake tools for Windows* in the Visual Studio Installer.

* Start Visual Studio
* Open the project folder (with the open folder option)
* In the cmake configuration settings specify the cmake toolchain file
  This file should be at `%USERPROFILE%/vcpkg/scripts/buildsystems/vcpkg.cmake` after using `get-mumble_deps.sh`.
* As cmake command argument add `-DVCPKG_TARGET_TRIPLET=x64-windows-static-md -Dstatic=ON -DIce_HOME=%USERPROFILE%/vcpkg/installed/x64-windows-static-md`
* Save and cmake should generate the build files, which will take a bit of time
* Use the build all action to build the project
* On success the built binaries will be placed in `out\build\<configuration-name>\`

Note: Visual Studio may wrongfully identify the error `The system was unable to find the specified registry key or value.` despite the build
succeeding. This is due to a VS script that outputs `ERROR` as text early on in the build process. The queried registry key is not required for the
build to succeed. Check the text Output or for other errors instead and **ignore this specific error**.

## Other IDEs with cmake support

IDE's such as Qt Creator, Visual Studio and VS Code (Code OSS) are capable of handling the configure and build step as part of the normal operation of
the IDE. The needed cmake options are called "Configure Arguments" and would need to be added for the project or workspace settings in the IDE when
the source folder is opened as a cmake project.
