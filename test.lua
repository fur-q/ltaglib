local taglib = require "ltaglib"

assert(taglib.open "nothing.ogg" == nil)

local f = taglib.open "test.mp3"

assert(f.artist  == "test artist")
assert(f.album   == "test album")
assert(f.title   == "test title")
assert(f.track   == 64)
assert(f.bitrate == 128)
assert(f.nope    == nil)

f.artist = "best artist"
assert(f.artist  == "best artist")

taglib.save(f)
taglib.close(f)

local f = taglib.open "test.mp3" 

assert(f.artist  == "best artist")

f.artist = "test artist"

taglib.save(f)
taglib.close(f)

print("OK")