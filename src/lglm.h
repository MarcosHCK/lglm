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
#ifndef LGLM_H_INCLUDED
#define LGLM_H_INCLUDED
#include <stdint.h>

/* GLM C-binding main header */
#include <cglm/cglm.h>

/*
 * Metatable names
 *
 */
#define LUA_VEC2 "vec2"
#define LUA_VEC3 "vec3"
#define LUA_VEC4 "vec4"
#define LUA_MAT2 "mat2"
#define LUA_MAT3 "mat3"
#define LUA_MAT4 "mat4"

/*
 * Mathematical entities
 * to type identifiers
 * (usually stored under 'mtype' name)
 *
 */
enum {
  LUA_TGLMANY = -1,
  LUA_TVEC2 = 0,
  LUA_TVEC3,
  LUA_TVEC4,
  LUA_TMAT2,
  LUA_TMAT3,
  LUA_TMAT4,
  _LGLM_TYPES,
};

#define GLM_ALIGNMENT 16

#pragma push(pack, 1)

typedef struct _lglm_object lglm_object_t;
typedef union _lglm_union lglm_union_t;

struct _lglm_object {
  struct {
    char alignment_[GLM_ALIGNMENT];
    union _lglm_union {
      vec2 vec2_;
      vec3 vec3_;
      vec4 vec4_;
      mat2 mat2_;
      mat3 mat3_;
      mat4 mat4_;
    } _union;
  };

  uint8_t mtype : 4;
  uint8_t offset : 4;
} __attribute__((packed, aligned(1)));

#pragma pop(pack)

#if __cplusplus
extern "C" {
#endif // __cplusplus

/*
 * Lua main headers
 *
 */
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

lglm_union_t* lua_newlglmobject(lua_State* L, int mtype);
lglm_union_t* lua_checklglmobject(lua_State* L, int idx, int mtype, int* actual_mtype);
lglm_union_t* lua_clonelglmobject(lua_State* L, int idx, int mtype);
lglm_union_t* lua_tolglmobject(lua_State* L, int idx, int mtype);

extern const char* lglm_typenames[];

#if __cplusplus
}
#endif // __cplusplus

#endif // LGLM_H_INCLUDED
