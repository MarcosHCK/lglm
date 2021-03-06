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

#if __cplusplus
}
#endif // __cplusplus


/*
 * Metatable names
 *
 */
#define LUA_VEC2    "vec2"
#define LUA_VEC3    "vec3"
#define LUA_VEC4    "vec4"
#define LUA_MAT2    "mat2"
#define LUA_MAT3    "mat3"
#define LUA_MAT4    "mat4"
#define LUA_BBOX    "bbox"
#define LUA_QUAT    "qaut"
#define LUA_SPHERE  "sphere"

/*
 * lglm extended types
 * identifiers
 * (note this *ARE NOT* Lua types,
 * so you *CAN NOT* test them against lua_type
 * return).
 *
 */
enum {
  LUA_TGLMANY = LUA_TNONE,

/*
 * Basic types
 *
 */
  LUA_TVEC2 = 0,
  LUA_TVEC3,
  LUA_TVEC4,
  LUA_TMAT2,
  LUA_TMAT3,
  LUA_TMAT4,

/*
 * Total basic types
 *
 */
  _LGLM_TYPES,

/*
 * Extended types
 * note: this types won't be checked if you
 * pass LUA_TGLMANY to lua_checklglmobject,
 * they are only recognized by it's specific
 * type.
 *
 */
  LUA_TBBOX = _LGLM_TYPES,
  LUA_TQUAT,
  LUA_TSPHERE,

/*
 * Total extended types
 *
 */
  _LGLM_TYPES_EX,
};

#define GLM_ALIGNMENT 16

#pragma pack(push, 1)

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

#pragma pack(pop)

#if __cplusplus
extern "C" {
#endif // __cplusplus

/*
 * Gets object type at index 'idx'.
 * If object at index is not a lglm object
 * returns LUA_TNONE.
 *
 */
int lua_lglmtype(lua_State* L, int idx);

/*
 * Pushes an userdata of type 'mtype', zero initialized.
 *
 */
lglm_union_t* lua_newlglmobject(lua_State* L, int mtype);

/*
 * Checks for an specific lglm object type,
 * and throws an error if not.
 * Obviously they are intended for C closure
 * argument checking.
 *
 * lua_checklglmobject is different because 'mtype'
 * could be LUA_TGLMANY to test for any type of lglm object and
 * stores it's type on 'actual_mtype' (which is nullable).
 * This allows polymorphic function which accepts multiple lglm
 * object types as input. Of course, it throws an error if value
 * at index 'idx' is not a lglm object of type 'mtype' (or any type 
 * if you've passed LUA_TGLMANY);
 *
 */
lglm_union_t* lua_checklglmobject(lua_State* L, int idx, int mtype);
lglm_union_t* lua_checklglmvec2(lua_State* L, int idx);
lglm_union_t* lua_checklglmvec3(lua_State* L, int idx);
lglm_union_t* lua_checklglmvec4(lua_State* L, int idx);
lglm_union_t* lua_checklglmmat2(lua_State* L, int idx);
lglm_union_t* lua_checklglmmat3(lua_State* L, int idx);
lglm_union_t* lua_checklglmmat4(lua_State* L, int idx);
lglm_union_t* lua_checklglmbbox(lua_State* L, int idx);
lglm_union_t* lua_checklglmsphere(lua_State* L, int idx);

/*
 * Pushes a clone of lglm object at index 'idx'.
 *
 */
lglm_union_t* lua_clonelglmobject(lua_State* L, int idx, int mtype);

/*
 * Getter functions.
 * they behaves same as check function,
 * but don't throws error if condition are
 * not met.
 *
 */
lglm_union_t* lua_tolglmobject(lua_State* L, int idx, int mtype);
lglm_union_t* lua_tolglmvec2(lua_State* L, int idx);
lglm_union_t* lua_tolglmvec3(lua_State* L, int idx);
lglm_union_t* lua_tolglmvec4(lua_State* L, int idx);
lglm_union_t* lua_tolglmmat2(lua_State* L, int idx);
lglm_union_t* lua_tolglmmat3(lua_State* L, int idx);
lglm_union_t* lua_tolglmmat4(lua_State* L, int idx);
lglm_union_t* lua_tolglmbbox(lua_State* L, int idx);
lglm_union_t* lua_tolglmsphere(lua_State* L, int idx);

/*
 * Self-explanatory variable.
 * Mostly used internally, but
 * can be useful under certain
 * circumstances (like polymorphic functions).
 *
 */
extern const char* lglm_typenames[];

#if __cplusplus
}
#endif // __cplusplus

#endif // LGLM_H_INCLUDED
