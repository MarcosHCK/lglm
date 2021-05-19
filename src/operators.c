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

void _only_vectors_error(lua_State* L, int idx, int base, int top) {
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  int mtype;
  ++top;

  for(mtype = base;mtype < top;mtype++)
  {
    luaL_addstring(&b, lglm_typenames[mtype]);
    if(mtype != top - 1)
    {
      luaL_addstring(&b, " or ");
    }
  }

  luaL_pushresult(&b);
  const char* types = lua_tostring(L, -1);
  _typeerror(L, idx, types);
}

static
void _only_same_dimensions(lua_State* L, int idx, int mtype) {
  luaL_Buffer b;
  luaL_buffinit(L, &b);

  luaL_addstring(&b, lglm_typenames[mtype]);
  luaL_addstring(&b, " or ");
  luaL_addstring(&b, lglm_typenames[mtype + LUA_TMAT2]);

  luaL_pushresult(&b);
  const char* types = lua_tostring(L, -1);
  _typeerror(L, idx, types);
}

int _add(lua_State* L) {
  int mtype, othertype;
  lglm_union_t *union_, *union2, *union1 =
  lua_checklglmobject(L, 1, LUA_TGLMANY, &mtype);

  switch(mtype)
  {
    case LUA_TVEC2:
    case LUA_TVEC3:
    case LUA_TVEC4:
/*
 * Vectors can be added, subtracted, multiplied or divided
 * only by another vector of similar dimensions
 *
 */
      union2 = lua_checklglmobject(L, 2, mtype, NULL);
      union_ = lua_newlglmobject(L, mtype);
      switch(mtype)
      {
        case LUA_TVEC2:
          glm_vec2_add(union1->vec2_, union2->vec2_, union_->vec2_);
          break;
        case LUA_TVEC3:
          glm_vec3_add(union1->vec3_, union2->vec3_, union_->vec3_);
          break;
        case LUA_TVEC4:
          glm_vec4_add(union1->vec4_, union2->vec4_, union_->vec4_);
          break;
        default:
          lua_pop(L, 1);
          return 0;
          break;
      }
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _adds(lua_State* L) {
  int mtype, othertype;
  lglm_union_t *union_, *union1 =
  lua_checklglmobject(L, 1, LUA_TGLMANY, &mtype);
  float scalar = luaL_checknumber(L, 2);

  switch(mtype)
  {
    case LUA_TVEC2:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec2_adds(union1->vec2_, scalar, union_->vec2_);
      return 1;
    case LUA_TVEC3:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_adds(union1->vec3_, scalar, union_->vec3_);
      return 1;
    case LUA_TVEC4:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_adds(union1->vec4_, scalar, union_->vec4_);
      return 1;
      break;
  }
return 0;
}

int _sub(lua_State* L) {
  int mtype, othertype;
  lglm_union_t *union_, *union2, *union1 =
  lua_checklglmobject(L, 1, LUA_TGLMANY, &mtype);

  switch(mtype)
  {
    case LUA_TVEC2:
    case LUA_TVEC3:
    case LUA_TVEC4:
/*
 * Vectors can be added, subtracted, multiplied or divided
 * only by another vector of similar dimensions
 *
 */
      union2 = lua_checklglmobject(L, 2, mtype, NULL);
      union_ = lua_newlglmobject(L, mtype);
      switch(mtype)
      {
        case LUA_TVEC2:
          glm_vec2_sub(union1->vec2_, union2->vec2_, union_->vec2_);
          break;
        case LUA_TVEC3:
          glm_vec3_sub(union1->vec3_, union2->vec3_, union_->vec3_);
          break;
        case LUA_TVEC4:
          glm_vec4_sub(union1->vec4_, union2->vec4_, union_->vec4_);
          break;
        default:
          lua_pop(L, 1);
          return 0;
          break;
      }
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _subs(lua_State* L) {
  int mtype, othertype;
  lglm_union_t *union_, *union1 =
  lua_checklglmobject(L, 1, LUA_TGLMANY, &mtype);
  float scalar = luaL_checknumber(L, 2);

  switch(mtype)
  {
    case LUA_TVEC2:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec2_subs(union1->vec2_, scalar, union_->vec2_);
      return 1;
    case LUA_TVEC3:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_subs(union1->vec3_, scalar, union_->vec3_);
      return 1;
    case LUA_TVEC4:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_subs(union1->vec4_, scalar, union_->vec4_);
      return 1;
      break;
  }
return 0;
}

int _dot(lua_State* L) {
  int mtype, othertype;
  lglm_union_t *union_, *union2, *union1 =
  lua_checklglmobject(L, 1, LUA_TGLMANY, &mtype);

  switch(mtype)
  {
    case LUA_TVEC2:
    case LUA_TVEC3:
    case LUA_TVEC4:
/*
 * Vectors can be added, subtracted, multiplied or divided
 * only by another vector of similar dimensions
 *
 */
      union2 = lua_checklglmobject(L, 2, mtype, NULL);
      switch(mtype)
      {
        case LUA_TVEC2:
          lua_pushnumber(L,
          glm_vec2_dot(union1->vec2_, union2->vec2_));
          break;
        case LUA_TVEC3:
          lua_pushnumber(L,
          glm_vec3_dot(union1->vec3_, union2->vec3_));
          break;
        case LUA_TVEC4:
          lua_pushnumber(L,
          glm_vec4_dot(union1->vec4_, union2->vec4_));
          break;
        default:
          lua_pushnil(L);
          break;
      }
      return 1;
      break;
    case LUA_TMAT2:
    case LUA_TMAT3:
    case LUA_TMAT4:
      union2 = lua_checklglmobject(L, 2, LUA_TGLMANY, &othertype);
/*
 * Matrices can be multiplied by another similar matrix or
 * by a column vector
 *
 */
      switch(mtype - othertype)
      {
        case LUA_TMAT2:
        union_ = lua_newlglmobject(L, othertype);
/*
 * Second argument is a column vector
 *
 */  
          switch(othertype)
          {
            case LUA_TVEC2:
              glm_mat2_mulv(union1->mat2_, union2->vec2_, union_->vec2_);
              break;
            case LUA_TVEC3:
              glm_mat3_mulv(union1->mat3_, union2->vec3_, union_->vec3_);
              break;
            case LUA_TVEC4: 
              glm_mat4_mulv(union1->mat4_, union2->vec4_, union_->vec4_);
              break;
            default:
              lua_pop(L, 1);
              return 0;
              break;
          }

          return 1;
          break;
        case 0:
        union_ = lua_newlglmobject(L, othertype);
/*
 * Second argument another matrix
 *
 */
          switch(othertype)
          {
            case LUA_TMAT2:
              glm_mat2_mul(union1->mat2_, union2->mat2_, union_->mat2_);
              break;
            case LUA_TMAT3:
              glm_mat3_mul(union1->mat3_, union2->mat3_, union_->mat3_);
              break;
            case LUA_TMAT4:
              glm_mat4_mul(union1->mat4_, union2->mat4_, union_->mat4_);
              break;
            default:
              lua_pop(L, 1);
              return 0;
              break;
          }

          return 1;
          break;
        default:
          _only_same_dimensions(L, 1, mtype - LUA_TMAT2);
          break;
      }
      break;
  }
return 0;
}

int _cross(lua_State* L) {
  int mtype, othertype;
  lglm_union_t *union_, *union2, *union1 =
  lua_checklglmobject(L, 1, LUA_TGLMANY, &mtype);

  switch(mtype)
  {
    case LUA_TVEC2:
    case LUA_TVEC3:
    case LUA_TVEC4:
/*
 * Vectors can be added, subtracted, multiplied or divided
 * only by another vector of similar dimensions
 *
 */
      union2 = lua_checklglmobject(L, 2, mtype, NULL);
      switch(mtype)
      {
        case LUA_TVEC2:
          lua_pushnumber(L,
          glm_vec2_cross(union1->vec2_, union2->vec2_));
          break;
        case LUA_TVEC3:
          union_ = lua_newlglmobject(L, mtype);
          glm_vec3_cross(union1->vec3_, union2->vec3_, union_->vec3_);
          break;
        default:
          lua_pushnil(L);
          break;
      }
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC3);
      break;
  }
return 0;
}

int _div(lua_State* L) {
  int mtype, othertype;
  lglm_union_t *union_, *union2, *union1 =
  lua_checklglmobject(L, 1, LUA_TGLMANY, &mtype);

  switch(mtype)
  {
    case LUA_TVEC2:
    case LUA_TVEC3:
    case LUA_TVEC4:
/*
 * Vectors can be added, subtracted, multiplied or divided
 * only by another vector of similar dimensions
 *
 */
      union2 = lua_checklglmobject(L, 2, mtype, NULL);
      union_ = lua_newlglmobject(L, mtype);
      switch(mtype)
      {
        case LUA_TVEC2:
          glm_vec2_div(union1->vec2_, union2->vec2_, union_->vec2_);
          break;
        case LUA_TVEC3:
          glm_vec3_div(union1->vec3_, union2->vec3_, union_->vec3_);
          break;
        case LUA_TVEC4:
          glm_vec4_div(union1->vec4_, union2->vec4_, union_->vec4_);
          break;
      }
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _divs(lua_State* L) {
  int mtype, othertype;
  lglm_union_t *union_, *union1 =
  lua_checklglmobject(L, 1, LUA_TGLMANY, &mtype);
  float scalar = luaL_checknumber(L, 2);

  switch(mtype)
  {
    case LUA_TVEC2:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec2_divs(union1->vec2_, scalar, union_->vec2_);
      return 1;
    case LUA_TVEC3:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_divs(union1->vec3_, scalar, union_->vec3_);
      return 1;
    case LUA_TVEC4:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_divs(union1->vec4_, scalar, union_->vec4_);
      return 1;
      break;
  }
return 0;
}
