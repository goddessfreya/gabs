gabs-build-config(5) -- `gabs-build` configuration script
===========================================================

## DESCRIPTION

`gabs-build-config` is a small script that should set the `${BUILTINS_DIR}` env
variable and the list of builtin packages. This script is sourced in by
`gabs-build-handler` and `gabs-build` and can be overridden by their `-bcs`
option, see gabs-build(1) and gabs-build-handler(1).

## AUTHOR

Freya Gentz &lt;zegentzy@protonmail.com&gt;

## SEE ALSO

gabs-build(1), gabs-build-handler(1)
