A small collection of scripts that I use to rebuild most of my AUR packages in a
clean chroot, plus a selection of PKGBUILDs.

# Normal usage.

Generally, all one needs to do is call `./build`. Of course, simply doing so
will leave you with an uncomfortable experience for a large majority of the
time.  Let's go through all the common issues.

# The compilation is pegging my HDD/CPU...

...and is leaving me unable to watch my YouTube videos, or whatever else I was
doing.

Operating under the assumption that your computer is not swapping, an easy way
to alleviate the stress placed on your hardware is to prefix your command with
`nice` and `ionice`. For example, I use `ionice -c 2 -n 7 nice -19 ./build ...`
all the time, and that generally helps.

# As quick primer... (MANDATORY READ FOR FOLLOWING SECTIONS)

`./build` looks for the following:

    1- `./<package>/build`, which it will call if present.
    2- Otherwise, it will call `./build_master`.

`./build_master` calls `/usr/bin/makechrootpkg`. If distcc is in use, we instead
call `./custom/makechrootpkg`, a file that is generated based from
`/usr/bin/makechrootpkg` on the spot.

The two `makechrootpkg`s call `/usr/bin/makepkg`.

`./build_master` has two similarly named options for adding parameters to
`makechrootpkg`:

    - `-m <param>`: Adds `<param>` to `makechrootpkg`'s parameters after the
      `--`, so that it is forwarded as-is to `makepkg`.
    - `-mc <param>`: Adds `<param>` to `makechrootpkg`'s parameters before the
      `--`, so that it is proccessed by `makechrootpkg`.

`./build` has two similarly named options for adding parameters to
`./build_master`:

    - `-ep <param>`: Adds `<param>` to `./build_master`'s parameters for the
      every built package.
    - `-fp <param>`: Adds `<param>` to `./build_master`'s parameters for the
      first built package. If at least one `-fp` is specified, none of the
      `-ep`s will be passed to the first package.

Please don't confuse `./build_master` and `./build`!

# One of the builds failed, how do I restart where I left off?

You are in luck, for `./build` comes built in with these three options:

    - `-rp <package>`: Resume on the package `<package>`.
    - `-r`: Resume on the last package that was building.
    - `-ra`: When resuming (i.e., either `-rp` or `-r` is also present), resume
      one package after the package that you are set to resume at, as ordered in
      the package list, otherwise doing nothing.

For example, if you were building `mesa-git-gentz`, and it failed due to some
spurious reason, you might want to run `./build -r`, or `./build -rp
mesa-git-gentz`. If you want to just skip over the package and go to the next
one, you might want to do `./build -r -ra`, or `./build -ra -rp mesa-git-gentz`.

# That's nice and all, but it's wiping my files between rebuilds!

Of course, that's unfortunately what `makepkg -c` does by default, but you can
fix this.

Now, you can stop `makepkg` from re-extracting and re-preparing all your stuff
via `--noextract` and `--noprepare`, respectively.

You can make `makechrootpkg` pass the parameter to `makepkg` by adding the
parameter after the `--`.

So, if you want `./build` to not ruin your shit, you can do the following:
`./build -fp -m -fp --noextract -fp -m -fp --noprepare`.

You probably don't want to use `-ep` instead of `-fp`, as then the next package
afterwards will reuse the prior package's build directory, which will just not
work. `makepkg` will most likely cry. Don't make them cry! One exception is if
you are using `-rs`, but we will get to that latter.

Of course, that's a long command, which is why `./build_master` comes with the
following:

    - `-nw`: Adds `--noextract --noprepare --skipinteg` after the `--`.  Please
      refer to the man page for `makepkg` for what they do. Removes the default
      `-c` that's normally passed to `makechrootpkg`.
    - `-r`: Repackage only please, don't bother rebuilding. Passes a
      `--repackage` and implies `-nw`.
    - `-w`: Replaces the default `-c` that's normally passed to `makechrootpkg`
      with `-C`.  Please refer to the man page for `makepkg` for what that does.
    - `-wn`: "FOR FUCKS SAKE, STOP INSERTING `-c` IN MY PRECIOUS PARAMETERS TO
      `makechrootpkg`."

It should be noted that `./build_master` does not allow any of these parameters
to be listed together, or to be listed twice. Exception is given to using `-r`
with `-rs`.

With that said, not wiping is as simple as `./build -fp -nw`.

# Okay, how do I stop it from going to the next package?

`./build` has the following:

    - `-o`: Build one package then stop. Works with `-rp` and `-r`. Makes `-ep`
      and `-fp` functionaly identical.

# How do I add a package?

First, make a directory for that package.

To get `./build` to use it, use this option:

    - `-p`: Adds this package to the package list. Having at least one `-p`
      disables the built in package list that `./build` normally fall back to.

# How do I stop it from messing with my current chroot! I want to use two!

You can rename the current chroot from the default "chroot" to anything of your
choice using one of the following two commands...

For `./build`: `-cn <chroot name>`.

For `./build_master`: `-cn <chroot name>`.

You probably want to use the former. The latter is used internally by `./build`.
Specifying it via `-ep`/`-fp` will result in `-cn` being passed twice to
`./build_master`, which will just result in an error.

# How do I preserve my build files for reuse latter?

You are in luck, for we have it all! `./build_master` exposes the following:

    - `-rs <resume dir>`: Implies `-nw`. Copies `${CHROOT_DIR}/$USER/build/`
      into `<resume dir>` before building.
    - `-bk <backup dir>`: Does the reverse after the build. Does not imply
      `-nw`.

`./build` exposes these wrappers:
    - `-rs <resume dir>`: Passes `-rs <resume dir>/<basename of package>` to
      `./build_master`.
    - `-bk <backup dir>`: Passes `-bk <backup dir>/<basename of package>` to
      `./build_master`.

# Why does it keep bumping the pkgrel?

Well, this is to try to avoid multiple pkg files of the same version. Generally
you want the newer one, right? Well, we just bump it by one!

Anyways, we provide two options for `./build_master` to modify the behavior:

    - `-dbr`: Don't bump the package release version.
    - `-rr <val>`: Set the pkgrel to `<val>` then return.

# Other misc options

`./build_master`:

    - `-nb`: Don't build, but do everything else, whatever that might be.
    - `-dist`: Enable distcc w/ pump support. Automatically inferred by
      `./build` if enabled in `makepkg.conf`.
    - `-c <chroot dir>`: Set chroot dir, internal use by `./build` only.
    - `-ed`

`./build`:

    - `-lds <size>`: Loop device max size. Passed to `fallocate` directly via
      `-l <size>`.  See `fallocate`'s man page.
    - `-kld`: Keep the loop device mounted please. Implied if build fails.
    - `-ed`
