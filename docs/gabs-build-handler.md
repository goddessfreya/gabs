gabs-build-handler(1) -- builds a package in a chroot
===========================================================

## SYNOPSIS

`gabs-build-handler` **-rpd** <REAL_PACKAGE_DIR> **-ed** <EPHEMERAL_DIR> **-c** <CHROOT_DIR> **-cn** <CHROOT_NAME> **-pn** <PKGNAME> [OPTIONS...]

## DESCRIPTION

Internal script that handles actually building in chroot once the chroot is
made. I guess it can be used manually, but don't expect it to be easy. Normally,
you want to use it via `gabs-build`'s `-ep` and `-fp` options, please refer to
gabs-build(1).

## OPTIONS

 * `-m <MAKEPKG_PARAM>`:

    Passes `<MAKEPKG_PARAM>` to `makechrootpkg`, placing it after the `--` so
    it's processed by `makepkg`.

 * `-mc <MAKECHROOTPKG_PARAM>`:

    Passes `<MAKECHROOTPKG_PARAM>` to `makechrootpkg`, placing it before the
    `--` so it's processed by `makechrootpkg`. Mostly used internally, but you
    can add stuff onto it, just make sure not to conflict with what
    `gabs-build-handler` is doing.

 * `-nw`: 
 
    Adds `--noextract --noprepare --skipinteg` to `${MAKEPKG_PARAMS}`.  Please
    refer to makepkg(1).

 * `-w`: 
 
    Adds `-C` to `${MAKEPKG_PARAM}`.  Please refer to makepkg(1).

 * `-r`: 

    Repackages the package only. Stops makepkg from calling `package` by passing
    `--repackage`. Please refer to makepkg(1). Also implies `-nw`.

 * `-dbr`: 

    Don't bump the package release version. 

 * `-rr <val>`: 

    Set the pkgrel to `<val>` then return.

 * `--`:
    
    Does precicely nothing. For use with `gabs-build`'s `-ep` and `-fp`, see
    gabs-build(1).

 * `-nb`:   
 
    Don't build anything, but do everything else, whatever that might be. For
    use with `-rr`.

## `gabs-build`-PROVIDED OPTIONS

The mandatory options above are are just some of the options that `gabs-build`
already passes in. If you are using `gabs-build`, trying to override them via
`-ep` or `-fp` is NOT supported. You've been warned!

 * `-dist`:

    If `distcc` is detected in use, we patch some files to make sure pump mode
    works. Automatically detected by `gabs-build`. If you ever have to pass this
    in manually it is a bug! Report please!

 * `-bcs <BUILD_CONFIG_SCRIPT>`:

    Passed in from `gabs-build`. Please refer to the option of the same name in
    gabs-build(1).

 * `-cn <CHROOT_NAME>`:

    Passed in from `gabs-build`. Please refer to the option of the same name in
    gabs-build(1).

 * `-c <CHROOT_DIR>`:
    
    Passed in from `gabs-build`. This is the directory of the chroot we are
    currently operating on.

 * `-rpd <REAL_PACKAGE_DIR>`:

    Passed in from `gabs-build`. This is the directory of the original
    `PKGBUILD` that got `rsync`ed into the current `$(pwd)`. The current
    `$(pwd)` should be inside `${PKGBUILD_CLONE_DIR}/${PKGNAME}`.

 * `-ed <EPHEMERAL_DIR>`:

    Passed in from `gabs-build`. Please refer to the option of the same name in
    gabs-build(1).

 * `-bk <BACKUP_DIR>`: 
 
    Passed in from `gabs-build`. Please refer to the option of the same name in
    gabs-build(1).

## INTERNALLY-USED OPTIONS

 * `-pr <PREVIOUS_REPAIR>`: 

    When repairing issues in the `PKGBUILD` (currently only `updpkgsums`), this
    is passed in to make sure the same repair isn't repeated.

## EXAMPLES

Generally, you want to pass in options to `gabs-build-handler` via `gabs-build`.
Here is a nice example:

    $ gabs-build -ep "$(pwd)/epth" -r -fp -nw -fp -m -fp -eRf

## AUTHOR

Freya Gentz &lt;zegentzy@protonmail.com&gt;

## SEE ALSO

gabs-swap-llvm(1), gabs-build-chroot(1), gabs-build-config(5), gabs-build(1)
