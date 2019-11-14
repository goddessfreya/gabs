gabs-build-chroot(1) -- manages the creation of chroots in loopback devices backed by files
===========================================================

## SYNOPSIS

`gabs-build-chroot` [OPTIONS...]

## DESCRIPTION

`gabs-build-chroot` is a small script in use by `gabs-build` to manage chroots.

## OPTIONS

 * `-c`:
    
    Cleanup the specified chroot, unmounting all directories using the loopback
    devices using this file, then removing those loopback devices. Processed
    before `-s`.

 * `-s`:
    
    Opens a loopback device using this file, then mounts it at the specified
    directory.

 * `-rm`:

    (Re)make the file, deleting the old one if it exists. For use in-tangent
    with `-s`, otherwise no-op.

 * `-lds <SIZE>`:

    Passed to `fallocate` directly as `-l <SIZE>`. See fallocate(1). Defaults to
    50G. For use in-tangent with `-rm`, otherwise no-op.

 * `-cn <CHROOT_NAME>`:

    Chroot name. Defaults to "chroot". For use with `-ed`. If `-cn` is present,
    `-cm` and `-cf` are auto-calculated. Please refer to `-cn` in gabs-build(1)
    for more details.

 * `-ed <EPHEMERAL_DIR>`:

    Ephemeral dir. For use with `-ed`. If present, `-cm` and `-cf` are
    auto-calculated. Please refer to `-ed` in gabs-build(1) for more details.

 * `-cm <CHROOT_MOUNT>`:

    Where to mount the chroot. Can be auto-calculated if `-ed` is passed in.

 * `-cf <CHROOT_FILE>`:

    Which file to mount/create. Can be auto-calculated if `-ed` is passed in,

## AUTHOR

Freya Gentz &lt;zegentzy@protonmail.com&gt;

## SEE ALSO

gabs-build(1)
