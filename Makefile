LUA_I = $(shell pkg-config --cflags-only-I lua5.1)
TAG_I = $(shell pkg-config --cflags-only-I taglib_c)
TAG_L = $(shell pkg-config --libs taglib_c)

CFLAGS  = -fPIC -Wall $(LUA_I) $(TAG_I)
LDFLAGS = $(TAG_L)

ltaglib.so: ltaglib.c
	gcc -shared $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	-rm ltaglib.so
