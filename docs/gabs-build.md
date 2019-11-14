gabs-build(1) -- builds your package(s) in a chroot
===========================================================

## SYNOPSIS

`gabs-build` **-ed** <EPHEMERAL_DIR> [OPTIONS...]

## DESCRIPTION

Builds package(s) in a clean chroot. Originally started as a small script to
automate the building of mesa-git, llvm-svn, et. al, but has quickly spiraled to
automate more and more of the workflow, including things like automatically
running `updpkgsums`. 

For full functionality (like auto-`updpkgsums`ing builtin packages), I'd only
install **gabs-build**'s AUR packages for access to the manpages (should be a
flag in the `PKGBUILD`), and manually `git clone
git@github.com:goddessfreya/gabs.git` into some local directory.

## BASIC USAGE

In the most minimal use case, the only option you must pass in is `-ed`, so that
the script knows where you want your "ephemeral directory," a.k.a. where we
store all the "temporary"-ish files. This includes things like the chroots and
the cloned VCS repos, which you probably don't want to be recreating all the
time. 

For this reason, the "ephemeral directory" should rarely be changed, if ever,
after all, what is the point of recloning all the cloned VCS repos? If you do
ever have to do this, please file a bug report/feature request!

Of course, there are many options, and not using them will leave you with an
uncomfortable experience for a large majority of the time.

## CALL TREE

When you call `gabs-build`, it won't call `makepkg` directly, of course. There
are a couple layers of indirection.

First, `gabs-build` looks for one of the following files, in this order, calling
the first one it spots:

    1- `<PKG_DIR>/build` 
    2- `gabs-build-handler`

It is generally assumed that `<PKG_DIR>/build` will eventually call
`gabs-build-handler`.

`gabs-build-handler` will call
`<EPHEMERAL_DIR>/custom.${CHROOT_NAME}/makechrootpkg`, which is a copy of
`makechrootpkg` that we patch on the spot.

Our patched `makechrootpkg` calls the `/bin/makepkg` in the chroot, which if
distcc is in use becomes a small wrapper around `/bin/makepkg.gabs-inner`.

Chances are you either want to forward some parameter to either `makepkg`,
`makechrootpkg` or `gabs-build-handler`. Please refer to gabs-build(1)'s **-ep**
and **-fp** options and gabs-build-handler(1)'s **-m** and **-mc** options.

## OPTIONS

 * `-p <PACKAGE>`:

    Adds `<PACKAGE>` to the package list. If the package list is empty, i.e.
    `-p` was never passed, `gabs-build` will fallback to the builtin package
    list provided by `gabs-build-config`.

    `<PACKAGE>` is `eval`ed after `gabs-build-config` is loaded, so doing things
    like `-p '${BUILTINS_DIR}/<SOME_BUILTIN_PACKAGE>'` is allowed. Please refer
    to gabs-build-config(5) for more info.

 * `-r`: 

    Resume on the last package that was building.

 * `-rp <PACKAGE>`: 

    Resume on the package `<PACKAGE>`. 
    
    `<PACKAGE>` is `eval`ed after `gabs-build-config` is loaded, so doing things
    like `-rp '${BUILTINS_DIR}/<SOME_BUILTIN_PACKAGE>'` is allowed. Please refer
    to gabs-build-config(5) for more info.

 * `-ra`: 

    When resuming (i.e., either `-rp` or `-r` is also present), resume one
    package after the package that you are set to resume at, as ordered in the
    package list, otherwise doing nothing.

 * `-fp <PARAM>`:

    For the first package that gets built, pass these options to
    `gabs-build-handler` instead of those normally passed in by `-ep`. If you
    don't want `-ep` passed, but don't want something else passed, `-fp --`
    works. For more info, please refer to gabs-build-handler(1).

 * `-ep <PARAM>`:

    For every package (exluding the first if `-fp` is in use), pass these
    options to `gabs-build-handler`. If unused, defaults to `-w`. If you don't
    want `-w` passed, but want don't something else passed, `-ep --` works. For
    more info, please refer to gabs-build-handler(1).

 * `-o`: 

    Only builds one package. Returns afterwards.

 * `-lds <LOOP_DEVICE_SIZE>`:

    Forwarded to `gabs-build-chroot`. See gabs-build-chroot(1).
    
 * `-kld`: 

    Keeps the loop device mounted. Implied if build fails.

 * `-cn <CHROOT_NAME>`:

    Chroot name. Defaults to `chroot`. Only one `gabs-build-*` script may
    operate on a chroot with the same name in the same ephemeral directory at
    the same time.  Was originally in use when `${EPHEMERAL_DIR}` is not a thing
    yet.  

    Different chroots with the same `${EPHEMERAL_DIR}` will share the same
    `PKGBUILD`s, and therefor not require recloning the same VCS packages
    multiple times over. The downside is that if two `gabs-build-handler`s try
    to build the same `PKGBUILD` at the same time, some funky shit might happen.

    If you want to build the same package in different chroots, change `-ed`
    instead.

 * `-rs <RESUME_DIR>`:
    
    Passes `-rs <RESUME_DIR>/<PKG_NAME>.<MD5_HASH_OF_FULL_NAME>` to
    `gabs-build-handler`. Must be a full path. Before building, files will be
    `rsync`ed from this directory into the build directory, allowing builds to
    resume from where they last completed. 

    Admitdly, I've never needed to use this option, as generally a fresh build
    is plenty faster.

 * `-bk <BACKUP_DIR>`: 
 
    Passes `-bk <BACKUP_DIR>/<PKG_NAME>.<MD5_HASH_OF_FULL_NAME>` to
    `gabs-build-handler`. Must be a full path. After building, build files will
    be `rsync`ed into this directory, preserving them for the future. Originally
    made to be used in tangent with `-rs`.

    This option is very usefull for preserving the source code you used to build
    a package with, for things like gdb. If you want to incrementally rebuild a
    package tho, you are probably better off with `-fp -nw`, instead of `-rs` +
    `-bk`, please refer to gabs-build-handler(1).

 * `-ed <EPHEMERAL_DIR>`:

    Sets the ephemeral dir, the directory used for storing "temporary" files.
    Mandatory. Must be a full path. Normally you want to keep this the same for
    all runs, however, if you want to build the same `PKGBUILD` multiple times
    simoultanously, you're going to have to use different `${EPHEMERAL_DIR}`.

    If you want to build the different packages in different chroots, change
    `-cn` instead.

 * `-bcs <BUILD_CONFIG_SCRIPT>`:

    Select an alternative to the default build config script for changing things
    like builtins. Must be a full path. If unused, `gabs-build` will try looking
    for a valid file in these locations, in this order:

                "$(pwd)/gabs-build-config" 
                "${EPHEMERAL_DIR}/gabs-build-config"
                "${HOME}/.config/gabs-build-config" 
                "/etc/gabs-build-config" 
                "${SCRIPTS_DIR}/gabs-build-config"

## ISSUES 

### The compilation is pegging my HDD/CPU...

...and is leaving me unable to watch my YouTube videos, or whatever else I was
doing.

Operating under the assumption that your computer is not swapping, an easy way
to alleviate the stress gabs places on your hardware is to prefix your command
with `nice` and `ionice`. 

For example, I always use `ionice -c 2 -n 7 nice -19 gabs-build ...` when
building.

## EXAMPLES

Simple build:       

    $ gabs-build -ep "$(pwd)/eph"

Please don't wipe and resume:

    $ gabs-build -ep "$(pwd)/epth" -r -fp -nw

Please just repackage, notice how `gabs-build-handler`'s `-m -eRf` as to be
prefaced with two different `-fp`s:

    $ gabs-build -ep "$(pwd)/epth" -r -fp -nw -fp -m -fp -eRf

## AUTHOR

Freya Gentz &lt;zegentzy@protonmail.com&gt;

## SEE ALSO

gabs-swap-llvm(1), gabs-build-chroot(1), gabs-build-config(5),
gabs-build-handler(1)
