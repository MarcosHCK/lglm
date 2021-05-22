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

static
void _ctor_typeerror(lua_State* L, int idx) {
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  luaL_addstring(&b, "number or ");
  int mtype;

  for(mtype = 0;mtype < _LGLM_TYPES;mtype++)
  {
    luaL_addstring(&b, lglm_typenames[mtype]);
    if(mtype != _LGLM_TYPES - 1)
    {
      luaL_addstring(&b, " or ");
    }
  }

  luaL_pushresult(&b);
  const char* types = lua_tostring(L, -1);
  _typeerror(L, idx, types);
}

#define _n (*__n)

void _get_floats_from(lua_State* L, int idx_, int* __n, const int floats_needed, float* floats) {
  int i, mtype, type = 
  lua_type(L, idx_);

  switch(type)
  {
    case LUA_TTABLE:
      for(i = 1;(floats_needed > _n);i++)
      {
        lua_pushinteger(L, i);
        lua_gettable(L, idx_);

        _get_floats_from(L, -1, __n, floats_needed, floats);
        lua_pop(L, 1);
      }
      break;
    case LUA_TUSERDATA:
      mtype = _checklglmobject(L, idx_, LUA_TGLMANY, FALSE);
      if(mtype == LUA_TGLMANY)
        _ctor_typeerror(L, idx_);
      else
      {
        lglm_union_t* union_ = lua_tolglmobject(L, idx_, mtype);
        float* mstore = (float*)(union_);
        for(i = 0;i < _floats_count(mtype) && (floats_needed > _n);i++)
        {
          floats[_n++] = mstore[i];
        }
      }
      break;
    case LUA_TNUMBER:
      floats[_n++] = lua_tonumber(L, idx_);
      break;
    case LUA_TNONE:
      floats[_n++] = 0.0f;
      break;
    default:
      _ctor_typeerror(L, idx_);
      break;
  }
}

#undef _n

void _get_floats(lua_State* L, int floats_needed, float* floats) {
  int idx = 1;
  int _n = 0;

  do
  {
    _get_floats_from(L, idx++, &_n, floats_needed, floats);
  }
  while(floats_needed > _n);
}

int _vec2(lua_State* L) {
/*
 * Needed floats to build
 * this type
 *
 */
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TVEC2);
  _get_floats(L, 2, (float*)(union_));
return 1;
}

int _vec3(lua_State* L) {
/*
 * Needed floats to build
 * this type
 *
 */
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TVEC3);
  _get_floats(L, 3, (float*)(union_));
return 1;
}

int _vec4(lua_State* L) {
/*
 * Needed floats to build
 * this type
 *
 */
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TVEC4);
  _get_floats(L, 4, (float*)(union_));
return 1;
}

int _mat2(lua_State* L) {
/*
 * Needed floats to build
 * this type
 *
 */
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT2);
  _get_floats(L, 4, (float*)(union_));
return 1;
}

int _mat3(lua_State* L) {
/*
 * Needed floats to build
 * this type
 *
 */
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT3);
  _get_floats(L, 9, (float*)(union_));
return 1;
}

int _mat4(lua_State* L) {
/*
 * Needed floats to build
 * this type
 *
 */
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  _get_floats(L, 16, (float*)(union_));
return 1;
}
