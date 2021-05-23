/*  Copyright 2021-2022 MarcosHCK
 *  This file is part of lglm.
 *
 *  lglm is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  lglm is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with lglm.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <lglm_luaapi.h>

/* from constructors.c */
void _get_floats(lua_State* L, int floats_needed, float* floats);

/*
 * You may ask: Why are you doing this, if is
 * simpler (and faster, although not much faster)
 * to add this at '_create_meta'?
 * See explanation at bbox.c
 *
 */
static
int __mt_tweaked = 0;

static
int sphere_mt__F_radius(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TSPHERE);
  lua_pushnumber(L, union_->vec4_[3]);
return 1;
}

static
int sphere_mt__F_transform(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TSPHERE);
  lglm_union_t* union1 = lua_checklglmobject(L, 2, LUA_TMAT4);

  CGLM_ALIGN(GLM_ALIGNMENT)
  vec4 vec4_;

  glm_vec4_copy(union_->vec4_, vec4_);
  glm_sphere_transform(vec4_, union_->mat4_, union_->vec4_);
  lua_pushvalue(L, 1);
return 1;
}

static
const luaL_Reg sphere_mt__index[] = {
  {"radius", sphere_mt__F_radius},
  {"transform", sphere_mt__F_transform},
  {NULL, NULL},
};

static
int sphere_mt__unpack(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TSPHERE);

  lua_pushnumber(L, union_->vec4_[0]);
  lua_pushnumber(L, union_->vec4_[1]);
  lua_pushnumber(L, union_->vec4_[2]);
  lua_pushnumber(L, union_->vec4_[3]);
return 4;
}

int _sphere(lua_State* L) {
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TSPHERE);
  _get_floats(L, 4, (float*)(union_));

  if(!__mt_tweaked)
  {
    __mt_tweaked = TRUE;

    luaL_getmetatable(L, LUA_SPHERE);

    lua_pushstring(L, "__unpack");
    lua_pushcfunction(L, sphere_mt__unpack);
    lua_settable(L, -3);

    lua_pushstring(L, "__index");
    lua_createtable(L, 0, _N_ELEMENTS(sphere_mt__index));
    luaL_setfuncs(L, sphere_mt__index, 0);
    lua_settable(L, -3);

    lua_pop(L, 1);
  }
return 1;
}

int _sphere_transform(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TSPHERE);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TMAT4);
  lglm_union_t* union_ = lua_clonelglmobject(L, 1, LUA_TSPHERE);
  glm_sphere_transform(union1->vec4_, union2->mat4_, union_->vec4_);
return 1;
}

int _sphere_merge(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TSPHERE);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TSPHERE);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TSPHERE);
  glm_sphere_merge(union1->vec4_, union2->vec4_, union_->vec4_);
return 1;
}

int _sphere_intersects(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TSPHERE);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TSPHERE);

  int intersects =
  glm_sphere_sphere(union1->vec4_, union2->vec4_);
  lua_pushboolean(L, intersects);
return 1;
}

int _sphere_intersects_point(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TSPHERE);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TVEC3);

  int intersects =
  glm_sphere_point(union1->vec4_, union2->vec4_);
  lua_pushboolean(L, intersects);
return 1;
}
