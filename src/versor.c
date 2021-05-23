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
int quat_mt__F_vec4(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TVEC4);
  glm_vec4_copy(union1->vec4_, union_->vec4_);
return 1;
}

static
int quat_mt__F_mat4(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_quat_mat4(union1->vec4_, union_->mat4_);
return 1;
}

static
int quat_mt__F_mat4t(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_quat_mat4t(union1->vec4_, union_->mat4_);
return 1;
}

static
int quat_mt__F_mat3(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT3);
  glm_quat_mat3(union1->vec4_, union_->mat3_);
return 1;
}

static
int quat_mt__F_mat3t(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT3);
  glm_quat_mat3t(union1->vec4_, union_->mat3_);
return 1;
}

static
const luaL_Reg quat_mt__index[] = {
  {"vec4", quat_mt__F_vec4},
  {"mat4", quat_mt__F_mat4},
  {"mat4t", quat_mt__F_mat4t},
  {"mat3", quat_mt__F_mat3},
  {"mat3t", quat_mt__F_mat3t},
  {NULL, NULL},
};

static
int quat_mt__unpack(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TQUAT);

  lua_pushnumber(L, union_->vec4_[0]);
  lua_pushnumber(L, union_->vec4_[1]);
  lua_pushnumber(L, union_->vec4_[2]);
  lua_pushnumber(L, union_->vec4_[3]);
return 4;
}

static
void _tweak_mt(lua_State* L, int idx) {
  if(!__mt_tweaked)
  {
    __mt_tweaked = TRUE;

    luaL_getmetatable(L, LUA_QUAT);

    lua_pushstring(L, "__unpack");
    lua_pushcfunction(L, quat_mt__unpack);
    lua_settable(L, -3);

    lua_pushstring(L, "__index");
    lua_createtable(L, 0, _N_ELEMENTS(quat_mt__index));
    luaL_setfuncs(L, quat_mt__index, 0);
    lua_settable(L, -3);

    lua_pop(L, 1);
  }
}

int _quat(lua_State* L) {
  int mtype = _checklglmobject(L, 2, LUA_TGLMANY, FALSE);
  if(mtype == LUA_TGLMANY)
  {
/*
 * Initialize with
 * raw values 
 *
 */
    float x = luaL_checknumber(L, 1),
          y = luaL_checknumber(L, 2),
          z = luaL_checknumber(L, 3),
          w = luaL_checknumber(L, 4);

    lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
    glm_quat_init(union_->vec4_, x, y, z, w);
    return 1;
  }
  else
  {
    float angle = luaL_checknumber(L, 1);
    lglm_union_t* union1 = _tolglmobject(L, 2, mtype);
    lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
    glm_quatv(union_->vec4_, angle, union1->vec3_);
    return 1;
  }
return 0;
}

int _quat_norm(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TQUAT);
  lua_pushnumber(L, glm_quat_norm(union_->vec4_));
return 1;
}

int _quat_normalize(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
  glm_quat_normalize_to(union1->vec4_, union_->vec4_);
return 1;
}

int _quat_dot(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TQUAT);
  lua_pushnumber(L, glm_quat_dot(union1->vec4_, union2->vec4_));
return 1;
}

int _quat_conjugate(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
  glm_quat_conjugate(union1->vec4_, union_->vec4_);
return 1;
}

int _quat_inv(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
  glm_quat_inv(union1->vec4_, union_->vec4_);
return 1;
}

int _quat_add(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
  glm_quat_add(union1->vec4_, union2->vec4_, union_->vec4_);
return 1;
}

int _quat_sub(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
  glm_quat_sub(union1->vec4_, union2->vec4_, union_->vec4_);
return 1;
}

int _quat_real(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lua_pushnumber(L, union1->vec4_[3]);
return 1;
}

int _quat_imag(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);

  lua_pushnumber(L, union1->vec4_[0]);
  lua_pushnumber(L, union1->vec4_[1]);
  lua_pushnumber(L, union1->vec4_[2]);
return 3;
}

int _quat_imagn(lua_State* L) {
  _quat_normalize(L);
  lglm_union_t* union1 = lua_checklglmobject(L, 2, LUA_TQUAT);

  lua_pushnumber(L, union1->vec4_[0]);
  lua_pushnumber(L, union1->vec4_[1]);
  lua_pushnumber(L, union1->vec4_[2]);
return 3;
}

int _quat_imaglen(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);

  CGLM_ALIGN(GLM_ALIGNMENT)
  vec3 vec3_;

  glm_vec3_copy(union1->vec3_, vec3_);
  glm_vec3_norm(vec3_);

  lua_pushnumber(L, vec3_[0]);
  lua_pushnumber(L, vec3_[1]);
  lua_pushnumber(L, vec3_[2]);
return 3;
}

int _quat_angle(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lua_pushnumber(L, glm_quat_angle(union1->vec4_));
}

int _quat_axis(lua_State* L) {
return _quat_imagn(L);
}

int _quat_mul(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
  glm_quat_mul(union1->vec4_, union2->vec4_, union_->vec4_);
return 1;
}

int _quat_mat4(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_quat_mat4(union1->vec4_, union_->mat4_);
return 1;
}

int _quat_mat4t(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_quat_mat4t(union1->vec4_, union_->mat4_);
return 1;
}

int _quat_mat3(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT3);
  glm_quat_mat3(union1->vec4_, union_->mat3_);
return 1;
}

int _quat_mat3t(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT3);
  glm_quat_mat3t(union1->vec4_, union_->mat3_);
return 1;
}

int _quat_lerp(lua_State* L) {
  lglm_union_t* from = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* to = lua_checklglmobject(L, 2, LUA_TQUAT);
  float t = luaL_checknumber(L, 3);

  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
  glm_quat_lerp(from->vec4_, to->vec4_, t, union_->vec4_);
return 1;
}

int _quat_lerpc(lua_State* L) {
  lglm_union_t* from = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* to = lua_checklglmobject(L, 2, LUA_TQUAT);
  float t = luaL_checknumber(L, 3);

  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
  glm_quat_lerpc(from->vec4_, to->vec4_, t, union_->vec4_);
return 1;
}

int _quat_slerp(lua_State* L) {
  lglm_union_t* from = lua_checklglmobject(L, 1, LUA_TQUAT);
  lglm_union_t* to = lua_checklglmobject(L, 2, LUA_TQUAT);
  float t = luaL_checknumber(L, 3);

  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
  glm_quat_slerp(from->vec4_, to->vec4_, t, union_->vec4_);
return 1;
}

int _quat_look(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TVEC3);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TQUAT);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_quat_look(union1->vec3_, union2->vec4_, union_->mat4_);
return 1;
}

int _quat_for(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TVEC3);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TVEC3);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
  glm_quat_for(union1->vec3_, union2->vec3_, union_->vec4_);
return 1;
}

int _quat_forp(lua_State* L) {
  lglm_union_t* from = lua_checklglmobject(L, 1, LUA_TVEC3);
  lglm_union_t* to = lua_checklglmobject(L, 2, LUA_TVEC3);
  lglm_union_t* up = lua_checklglmobject(L, 2, LUA_TVEC3);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TQUAT);
  glm_quat_forp(from->vec3_, to->vec3_, up->vec3_, union_->vec4_);
return 1;
}

int _qaut_rotate(lua_State* L) {
  lglm_union_t *union_, *union3, *union2, *union1 =
  lua_checklglmobject(L, 1, LUA_TQUAT);
  int mtype = _checklglmobject(L, 2, LUA_TGLMANY, FALSE);
  int mtype2;

  switch(mtype)
  {
    case LUA_TVEC3:
      union2 = _tolglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_quat_rotatev(union1->vec4_, union2->vec3_, union_->vec3_);
      return 1;
      break;
    case LUA_TMAT4:
      mtype2 = _checklglmobject(L, 3, LUA_TVEC3, FALSE);
      if(mtype2 != LUA_TGLMANY)
      {
        if(mtype2 != LUA_TVEC3)
        {
          _typeerror(L, 3, LUA_VEC3);
        }
        else
        {
          union3 = _tolglmobject(L, 3, mtype2);
          union_ = lua_clonelglmobject(L, 2, mtype);
          glm_quat_rotate_at(union_->mat4_, union1->vec4_, union3->vec3_);
          return 1;
        }
      }
      else
      {
        union2 = _tolglmobject(L, 2, mtype);
        union_ = lua_newlglmobject(L, mtype);
        glm_quat_rotate(union2->mat4_, union1->vec4_, union_->mat4_);
        return 1;
      }
      break;
    default:
      _typeerror(L, 2, LUA_VEC3 " or " LUA_MAT4);
      return 0;
      break;
  }
return 0;
}
