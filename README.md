# ltaglib 

Lua bindings for the [TagLib](http://taglib.github.com/) audio metadata library.

TagLib provides both an abstract API which works on all supported file formats, as well as more advanced format-specific APIs.  Currently, ltaglib only wraps the abstract API.

## Usage

```lua
local taglib = require "ltaglib"

-- open a file
local file = taglib.open("09 The King of Speed.flac")

-- view all available tags
print(f.artist, f.album, f.title, f.comment, f.genre, f.track, f.year)

-- view all available properties
print(f.length, f.bitrate, f.samplerate, f.channels)

-- update some tags
f.artist  = "Takenobu Mitsuyoshi"
f.comment = "there's a crash coming out of turn 3"
f.track   = 9

-- save our changes
f:save()

-- close the file (or just wait for it to be GC'd)
f:close()
```

## License

TagLib is released under the LGPL; as such, so is ltaglib.