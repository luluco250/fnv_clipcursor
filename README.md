# Fallout: New Vegas Inactive Window Scroll Fix 🖱

Fixes the problematic behavior of mouse scrolling not being fully captured by
the game, which results in scrolling other applications in other windows due to
Windows 10's inactive window scroll feature.

This is more noticeable with multi-monitor setups, for example, when scrolling
through a menu, you might notice your browser on another screen scrolls as well.

This is because the mouse cursor seems to not actually be restricted to the
window. It's obvious why this was never a problem before Windows 10, but now it
needs fixing. 😉

This uses the `ClipCursor()` function of the Win32 API to restrict the mouse
to the game window's boundaries, only letting go when it loses focus (such as
alt-tabbing).

This has also been tested with ReShade and thus *should* be compatible with
other overlays as well. 🎮

NVSE source code is not provided as per the license agreement, so to build this
plugin you'll need to grab it [here](http://www.nvse.silverlock.org/) (download
the latest version).

After that, copy `src/common` and `src/nvse` to the folder before where this
project is located (like `src/nvse_plugin_example`). It should look like this:

```
📁...
└📁common
└📁fnv_clipcursor
 └...
 └📄fnv_clipcursor.sln
 └...
└📁nvse
```

Unfortunately due to the dependency on the behavior of MSVC 9.0, this project
needs to be built using Visual C++ 2008 (express or not).

I might look into upgrading it, but that'd require modifying NVSE files.
