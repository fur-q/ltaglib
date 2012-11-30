package = 'ltaglib'
version = 'git-1'

source = {
  url = 'git://github.com/fur-q/ltaglib.git',
}

description = {
  summary  = 'Lua bindings to TagLib',
  homepage = 'https://github.com/fur-q/ltaglib',
  license  = 'MIT/X11',
}

dependencies = {
  'lua >= 5.1'
}

external_dependencies = {
  TAGLIB = {
    header  = "tag_c.h",
    library = "tag_c"
  }
}

build = {
  type    = 'builtin',
  modules = {
    ltaglib = {
      sources   = { 'ltaglib.c' },
      libraries = { 'tag', 'tag_c'},
      incdirs   = { "$(TAGLIB_INCDIR)" },
      libdirs   = { "$(TAGLIB_LIBDIR)" },
    },
  },
}