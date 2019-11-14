gabs-swap-llvm(1) -- change the system's active LLVM in use
===========================================================

## SYNOPSIS

`gabs-swap-llvm` <NUMBER_OF_BITS> <NEW_DIR>

## DESCRIPTION

For use in-tangent with llvm-git-gentz, `gabs-swap-llvm` let's you swap out the
system's current llvm-git-gentz for another. This lets you avoid having to
`pacman -U` the new one, which can be slow for the bigger multi-gigabyte LLVM
builds.

## USAGE

Pretty simple, pass in the number of bits (i.e. either 32 or 64) following by
the directory to the new LLVM.

## EXAMPLES

    $ gabs-swap-llvm 64 "$(pwd)/../some-directory-with-llvm64"

## AUTHOR

Freya Gentz &lt;zegentzy@protonmail.com&gt;

## SEE ALSO

gabs-build(1)
