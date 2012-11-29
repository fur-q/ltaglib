#include <string.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <tag_c.h>

typedef struct AudioData {
  TagLib_File *file;
  TagLib_Tag *tag;
  const TagLib_AudioProperties *props;
} AudioData;

static int ltaglib_save(lua_State *L) {
  AudioData *ad = luaL_checkudata(L, 1, "AudioData");
  lua_pushboolean(L, taglib_file_save(ad->file));
  return 1;
}

static int ltaglib_close(lua_State *L) {
  AudioData *ad = luaL_checkudata(L, 1, "AudioData");
  lua_pushnil(L);
  lua_setmetatable(L, 1);
  taglib_tag_free_strings(); // ??
  taglib_file_free(ad->file);
  return 0;
}

static int audiodata_index(lua_State *L) {
  AudioData *ad = luaL_checkudata(L, 1, "AudioData");
  const char *idx = luaL_checkstring(L, 2);

  // tags
  if (!strcmp(idx, "title"))
    lua_pushstring(L, taglib_tag_title(ad->tag));
  else if (!strcmp(idx, "artist"))
    lua_pushstring(L, taglib_tag_artist(ad->tag));
  else if (!strcmp(idx, "album"))
    lua_pushstring(L, taglib_tag_album(ad->tag));
  else if (!strcmp(idx, "comment"))
    lua_pushstring(L, taglib_tag_comment(ad->tag));
  else if (!strcmp(idx, "genre"))
    lua_pushstring(L, taglib_tag_genre(ad->tag));
  else if (!strcmp(idx, "year"))
    lua_pushinteger(L, taglib_tag_year(ad->tag));
  else if (!strcmp(idx, "track"))
    lua_pushinteger(L, taglib_tag_track(ad->tag));

  // properties
  else if (!strcmp(idx, "length"))
    lua_pushinteger(L, taglib_audioproperties_length(ad->props));
  else if (!strcmp(idx, "bitrate"))
    lua_pushinteger(L, taglib_audioproperties_bitrate(ad->props));
  else if (!strcmp(idx, "samplerate"))
    lua_pushinteger(L, taglib_audioproperties_samplerate(ad->props));
  else if (!strcmp(idx, "channels"))
    lua_pushinteger(L, taglib_audioproperties_channels(ad->props));

  // functions
  else if (!strcmp(idx, "save"))
    lua_pushcfunction(L, ltaglib_save);
  else if (!strcmp(idx, "close"))
    lua_pushcfunction(L, ltaglib_close);

  // nothing
  else
    lua_pushnil(L);

  return 1;
}

static int audiodata_newindex(lua_State *L) {
  AudioData *ad = luaL_checkudata(L, 1, "AudioData");
  const char *idx = luaL_checkstring(L, 2);
  const char *val = luaL_checkstring(L, 3);

  if (!strcmp(idx, "title"))
    taglib_tag_set_title(ad->tag, val);
  else if (!strcmp(idx, "artist"))
    taglib_tag_set_artist(ad->tag, val);
  else if (!strcmp(idx, "album"))
    taglib_tag_set_album(ad->tag, val);
  else if (!strcmp(idx, "comment"))
    taglib_tag_set_comment(ad->tag, val);
  else if (!strcmp(idx, "genre"))
    taglib_tag_set_genre(ad->tag, val);
  else if (!strcmp(idx, "year"))
    taglib_tag_set_year(ad->tag, luaL_checkinteger(L, 3));
  else if (!strcmp(idx, "track")) 
    taglib_tag_set_track(ad->tag, luaL_checkinteger(L, 3));
  else
    return luaL_error(L, "bad argument #3 to 'newindex'");

  return 0;
}

static int ltaglib_open(lua_State *L) {
  const char *path = luaL_checkstring(L, 1);
  TagLib_File *file = taglib_file_new(path);
  if (file == NULL) {
    lua_pushnil(L);
    lua_pushfstring(L, "Error opening %s", path);
    return 2;
  }

  TagLib_Tag *tag = taglib_file_tag(file);
  const TagLib_AudioProperties *props = taglib_file_audioproperties(file);

  if (tag == NULL) {
    lua_pushnil(L);
    lua_pushfstring(L, "Error opening %s", path);
    return 2; 
  }

  AudioData *ad = (AudioData *)lua_newuserdata(L, sizeof(AudioData));
  ad->file = file, ad->tag = tag, ad->props = props;

  luaL_newmetatable(L, "AudioData");
  lua_pushcfunction(L, audiodata_index);
  lua_setfield(L, 3, "__index");
  lua_pushcfunction(L, audiodata_newindex);
  lua_setfield(L, 3, "__newindex");
  lua_pushcfunction(L, ltaglib_close);
  lua_setfield(L, 3, "__gc");

  lua_setmetatable(L, 2);

  return 1;
}

luaL_Reg ltaglib[] = {
    { "open",  ltaglib_open  },
    { "save",  ltaglib_save  },
    { "close", ltaglib_close },
    { NULL, NULL }
};

int luaopen_ltaglib(lua_State *L) {
    lua_newtable(L);
#if LUA_VERSION_NUM > 501  // lua 5.2
    luaL_setfuncs(L,ltaglib,0);
#else                      // lua 5.1
    luaL_register(L, NULL, ltaglib);
#endif
    return 1;
}