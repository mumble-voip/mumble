# Resource management

During the lifetime of a plugin it usually becomes necessary to allocate some resources and pass them across the boundary between Mumble and the
plugin. Every time this happens, the ownership of that resource is transferred with it. That means that the side that is passed the resources is then
also responsible for cleaning them once they are no longer needed.

Note however that memory allocations and deallocations must not happen in different modules. In this case that means that the side that _allocated_
the resource, must also be the one that _deallocates_ it. The problem is only that this is then not the side that actually _uses_ and _owns_ the
resource anymore.

Therefore systems exist to pass a resource back to the original creator in order for it to deallocate it again. By this the ownership is then always
transferred back to that side as well.


## Mumble → Plugin

Certain operations require that Mumble allocates some resources that are then passed to the plugin as pointers. Most notably Strings and Arrays. These
resources must be cleaned up again after the plugin is done using them.

In order for that to happen, the `freeMemory` function in the [Mumble-API](MumbleAPI.md) must be used.

Which resources require freeing is documented in the description of the respective Mumble-API functions. As a rule of thumb though all Strings and all
Arrays require this procedure.


## Plugin → Mumble

There are also cases in which the plugin has to allocate resources to be passed to Mumble. One prominent example of this is the String that is
returned from `mumble_getName`.

Note that because certain languages (e.g. C++) provide a way to declare static Strings that don't require freeing, the Plugin must tell Mumble what
kind of resource this is. This is what the `MumbleStringWrapper` struct is for (defined in the `PluginComponents` header). It contains a `data` field
for holding the `const char *`, a `size` field to hold the String's size and it also holds a `needsReleasing` flag that indicates whether or not the
given String requires freeing.

If `needsReleasing = false` then nothing else needs to be done on the plugin side.

If `needsReleasing = true` Mumble will call `mumble_releaseResource` with the pointer to the given resource as the argument. It is then the job of the
plugin to deallocate the resource. When doing this it must be made sure that the way of allocation matches the way of deallocation. That means that
resources allocated using `new` must be released using `delete`, `new[]` with `delete[]` and resources allocated using `malloc` are to be released
using `free`. If your plugin uses different ways of allocating resources, it is your responsibility to keep track of which resource was allocated in
which way so it can be deallocated properly again.

