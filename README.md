# png2lp

PNG to line printer converter

## Overview

The purpose of this program is to convert two-color PNG images to line printers.
In principle it should be possible to implement any terminal or dot-matrix
printer as program output.

## Usage

Basic usage is as simple as providing file name of PNG image to be converted:
```
png2lp tux.png
```
As a result tux.png would be printed on standard output using UTF-8 encoding, as
vt100 is default sink.

It is possible to choose between few printers and page layouts and more could be
possibly added in future.

* to list available printers, you can invoke: `png2lp -l`
* to list available pages for certain printer, do: `png2lp -L vt100`
* to set non-default printer as a sink: `png2lp -p oki3321`
* to set non-default page layout on a sink: `png2lp -p oki3321 -P a4-p`

## Sinks

Currently only following sinks and pages are available:
* `vt100` - standard UTF-8 terminal
  * `80x25` - 80 columns, 25 lines
  * `80xINF` - 80 columns, no line limit
* `oki3321` - Oki 3321 printer (generally other Oki machines should work here)
  * `a4-p` - A4 paper in portrait orientation
