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
void _get_floats_from(lua_State* L, int idx_, int* __n, const int floats_needed, float* floats);

/* from operators.c */
void _only_vectors_error(lua_State* L, int idx, int base, int top);

const
char* lglm_typenames[] = {
    LUA_VEC2,
    LUA_VEC3,
    LUA_VEC4,
    LUA_MAT2,
    LUA_MAT3,
    LUA_MAT4,
    LUA_BBOX,
    LUA_QUAT,
    LUA_SPHERE,
};

/*
 * Check our custom type fits with their respective metatable names,
 * and more important, do not overflow 'mtype' field (which is 4 bits wide)
 * of 'lglm_object_t' structure.
 *
 */
_STATIC_ASSERT(_N_ELEMENTS(lglm_typenames) == _LGLM_TYPES_EX);
_STATIC_ASSERT(_N_ELEMENTS(lglm_typenames) < 15);

static
int _vector_mt__tostring(lua_State* L) {
  int mtype = lua_lglmtype(L, 1);
  if _LIKELY(mtype != LUA_TNONE)
  {
    lua_pushfstring(L,
                    "%s (%p)",
                    lglm_typenames[mtype],
                    lua_touserdata(L, 1)
                    );
    return 1;
  }
return 0;
}

static
const char vecindex_chars[] = {
    'x', 'y', 'z', 'w'
};

static
int _is_vecindex(int c) {
  int i;
  for(i = 0;i < sizeof(vecindex_chars);i++)
  {
    if(c == vecindex_chars[i])
    {
      return i;
    }
  }
return -1;
}

static
int _vector_mt__newindex(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union2, *union_ = _tolglmobject(L, 1, mtype);

  int index = 0;
  if(lua_isnumber(L, 2))
  {
    index = lua_tointeger(L, 2);
  } else
  if(lua_isstring(L, 2))
  {
    const char* index_ = lua_tostring(L, 2);
    index = _is_vecindex(index_[0]) + 1;
  } else
  return 0;

  if(index >= 1 && index <= _floats_count(mtype))
  switch(mtype)
  {
    case LUA_TVEC2:
      union_->vec2_[index - 1] = luaL_checknumber(L, 3);
      break;
    case LUA_TVEC3:
      union_->vec3_[index - 1] = luaL_checknumber(L, 3);
      break;
    case LUA_TVEC4:
      union_->vec4_[index - 1] = luaL_checknumber(L, 3);
      break;
    case LUA_TMAT2:
      union2 = lua_checklglmobject(L, 3, mtype - LUA_TMAT2);
      glm_vec2_copy(union2->vec2_, union_->mat2_[index - 1]);
      break;
    case LUA_TMAT3:
      union2 = lua_checklglmobject(L, 3, mtype - LUA_TMAT2);
      glm_vec3_copy(union2->vec3_, union_->mat3_[index - 1]);
      break;
    case LUA_TMAT4:
      union2 = lua_checklglmobject(L, 3, mtype - LUA_TMAT2);
      glm_vec4_copy(union2->vec4_, union_->mat4_[index - 1]);
      break;
  }
return 0;
}

static
int __vector_mt__F_identity(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t* union_ = _tolglmobject(L, 1, mtype);

  switch(mtype)
  {
    case LUA_TVEC4:
      glm_quat_identity(union_->vec4_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TMAT2:
      glm_mat2_identity(union_->mat2_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TMAT3:
      glm_mat3_identity(union_->mat3_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TMAT4:
      glm_mat4_identity(union_->mat4_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, LUA_TVEC4, LUA_TMAT4);
      break;
  }
return 0;
}

static
int __vector_mt__F_zero(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t* union_ = _tolglmobject(L, 1, mtype);

  switch(mtype)
  {
    case LUA_TVEC2:
      glm_vec2_zero(union_->vec2_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TVEC3:
      glm_vec3_zero(union_->vec3_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TVEC4:
      glm_vec4_zero(union_->vec4_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TMAT2:
      glm_mat2_zero(union_->mat2_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TMAT3:
      glm_mat3_zero(union_->mat3_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TMAT4:
      glm_mat4_zero(union_->mat4_);
      lua_pushvalue(L, 1);
      return 1;
      break;
  }
return 0;
}

static
int __vector_mt__F_one(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t* union_ = _tolglmobject(L, 1, mtype);

  switch(mtype)
  {
    case LUA_TVEC2:
      glm_vec2_one(union_->vec2_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TVEC3:
      glm_vec3_one(union_->vec3_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TVEC4:
      glm_vec4_one(union_->vec4_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

static
int __vector_mt__F_invert(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t* union_ = _tolglmobject(L, 1, mtype);

  CGLM_ALIGN(GLM_ALIGNMENT)
  union {
    mat2 mat2_;
    mat3 mat3_;
    mat4 mat4_;
  } tmp;

  switch(mtype)
  {
    case LUA_TMAT2:
      glm_mat2_copy(union_->mat2_, tmp.mat2_);
      glm_mat2_inv(tmp.mat2_, union_->mat2_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TMAT3:
      glm_mat3_copy(union_->mat3_, tmp.mat3_);
      glm_mat3_inv(tmp.mat3_, union_->mat3_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    case LUA_TMAT4:
      glm_mat4_copy(union_->mat4_, tmp.mat4_);
      glm_mat4_inv(tmp.mat4_, union_->mat4_);
      lua_pushvalue(L, 1);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, LUA_TMAT2, LUA_TMAT4);
      break;
  }
return 0;
}

static
int _vector_mt__index(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t* union_ = _tolglmobject(L, 1, mtype);

  if(lua_isnumber(L, 2))
  {
/*
 * Simple array-like access (ex: vec4[3] or mat2[1])
 * note: index starts from 1 (as usual in Lua)
 *
 */
    int index = lua_tointeger(L, 2);

    switch(mtype)
    {
      case LUA_TVEC2:
        if(index >= 1 && index <= 2)
          lua_pushnumber(L, union_->vec2_[index - 1]);
        else
          lua_pushnil(L);
        return 1;
        break;
      case LUA_TVEC3:
        if(index >= 1 && index <= 3)
          lua_pushnumber(L, union_->vec3_[index - 1]);
        else
          lua_pushnil(L);
        return 1;
        break;
      case LUA_TVEC4:
        if(index >= 1 && index <= 4)
          lua_pushnumber(L, union_->vec4_[index - 1]);
        else
          lua_pushnil(L);
        return 1;
        break;
/*
 * TODO:
 * I'd like matrices could be manipulated in a way like 'mat4[1][1] = 5',
 * but I have no idea how to propagate generated vector __newindex call
 * to its parent matrix in C (maybe an upvalue?).
 *
 */
      case LUA_TMAT2:
        if(index >= 1 && index <= 2)
        {
          lglm_union_t* union2 = lua_newlglmobject(L, LUA_TVEC2);
          glm_vec2_copy(union_->mat2_[index - 1], union2->vec2_);
        }
        else
          lua_pushnil(L);
        return 1;
        break;
      case LUA_TMAT3:
        if(index >= 1 && index <= 3)
        {
          lglm_union_t* union2 = lua_newlglmobject(L, LUA_TVEC3);
          glm_vec3_copy(union_->mat3_[index - 1], union2->vec3_);
        }
        else
          lua_pushnil(L);
        return 1;
        break;
      case LUA_TMAT4:
        if(index >= 1 && index <= 4)
        {
          lglm_union_t* union2 = lua_newlglmobject(L, LUA_TVEC4);
          glm_vec4_copy(union_->mat4_[index - 1], union2->vec4_);
        }
        else
          lua_pushnil(L);
        return 1;
        break;
    }
  } else
  if(lua_isstring(L, 2))
  {
/*
 * Methods and point-like access
 * (ex: mat4:identity() or vec3.xz)
 *
 */
    size_t i, sz;
    const char* index =
    lua_tolstring(L, 2, &sz);
    int point_like = TRUE;

    /* check if index is not a point-like access,         */
    /* is faster then check for individual methods first  */
    for(i = 0;i < sz;i++)
    if(_is_vecindex(index[i]) == (-1))
    {
      point_like = FALSE;
      break;
    }

    if(point_like)
    {
      if(mtype >= LUA_TMAT2)
      {
        /* this kind of access are only available to vectors */
        return 0;
      }

      int c, r = 0;
      for(i = 0;i < sz;i++)
      {
        c = _is_vecindex(index[i]);
        switch(mtype)
        {
          case LUA_TVEC2:
            if(c >= 0 && c <= 1)
            {
              lua_pushnumber(L, union_->vec2_[c]);
              ++r;
            }
            break;
          case LUA_TVEC3:
            if(c >= 0 && c <= 2)
            {
              lua_pushnumber(L, union_->vec3_[c]);
              ++r;
            }
            break;
          case LUA_TVEC4:
            if(c >= 0 && c <= 3)
            {
              lua_pushnumber(L, union_->vec4_[c]);
              ++r;
            }
            break;
        }
      }
      return r;
    } 
    else
    {
      if(!strcmp(index, "identity"))
      {
        lua_pushcfunction(L, __vector_mt__F_identity);
        return 1;
      } else
      if(!strcmp(index, "zero"))
      {
        lua_pushcfunction(L, __vector_mt__F_zero);
        return 1;
      } else
      if(!strcmp(index, "one"))
      {
        lua_pushcfunction(L, __vector_mt__F_one);
        return 1;
      } else
      if(!strcmp(index, "invert"))
      {
        lua_pushcfunction(L, __vector_mt__F_invert);
        return 1;
      }
    }
  }
return 0;
}

static
const luaL_Reg vector_mt[] = {
  {"__tostring", _vector_mt__tostring},
  {"__newindex", _vector_mt__newindex},
  {"__index", _vector_mt__index},
  {NULL, NULL},
};

void _create_meta(lua_State * L) {
  int mtype;
  const char* typename;

  for(mtype = LUA_TGLMANY + 1;mtype < _N_ELEMENTS(lglm_typenames);mtype++)
  {
    typename = lglm_typenames[mtype];

    luaL_newmetatable(L, typename);
    luaL_setfuncs(L, vector_mt, 0);
    lua_pushstring(L, "__name");
    lua_pushstring(L, typename);
    lua_settable(L, -3);
    lua_pop(L, 1);
  }
}
