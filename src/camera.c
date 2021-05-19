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

int _frustum(lua_State* L) {
  vec4 viewport = GLM_VEC4_BLACK_INIT;
  int idx = 1;

  if(lua_isnumber(L, 1) &&
     lua_isnumber(L, 2) &&
     lua_isnumber(L, 3) &&
     lua_isnumber(L, 4)
    )
  {
    viewport[0] = lua_tonumber(L, idx++);
    viewport[1] = lua_tonumber(L, idx++);
    viewport[2] = lua_tonumber(L, idx++);
    viewport[3] = lua_tonumber(L, idx++);
  }
  else
  {
    lglm_union_t* union_ = lua_checklglmobject(L, idx++, LUA_TVEC4, NULL);
    glm_vec4_copy(union_->vec4_, viewport);
  }

  float near_v = luaL_checknumber(L, idx++);
  float far_v = luaL_checknumber(L, idx++);

  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_frustum(viewport[0], viewport[1], viewport[2], viewport[3], near_v, far_v, union_->mat4_);
return 1;
}

int _ortho_persp(lua_State* L) {
  vec4 viewport = GLM_VEC4_BLACK_INIT;
  int idx = 1;

  if(lua_isnumber(L, 1) &&
     lua_isnumber(L, 2) &&
     lua_isnumber(L, 3) &&
     lua_isnumber(L, 4)
    )
  {
    viewport[0] = lua_tonumber(L, idx++);
    viewport[1] = lua_tonumber(L, idx++);
    viewport[2] = lua_tonumber(L, idx++);
    viewport[3] = lua_tonumber(L, idx++);
  }
  else
  {
    lglm_union_t* union_ = lua_checklglmobject(L, idx++, LUA_TVEC4, NULL);
    glm_vec4_copy(union_->vec4_, viewport);
  }

  float near_v = luaL_checknumber(L, idx++);
  float far_v = luaL_checknumber(L, idx++);

  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_ortho(viewport[0], viewport[1], viewport[2], viewport[3], near_v, far_v, union_->mat4_);
return 1;
}

int _ortho_aabb(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TVEC3, NULL);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TVEC3, NULL);

  vec3 vectors[2];
  glm_vec3_copy(union1->vec3_, vectors[0]);
  glm_vec3_copy(union2->vec3_, vectors[1]);

  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_ortho_aabb(vectors, union_->mat4_);
return 1;
}

int _ortho_aabb_p(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TVEC3, NULL);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TVEC3, NULL);
  float padding = luaL_checknumber(L, 3);

  vec3 vectors[2];
  glm_vec3_copy(union1->vec3_, vectors[0]);
  glm_vec3_copy(union2->vec3_, vectors[1]);

  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_ortho_aabb_p(vectors, padding, union_->mat4_);
return 1;
}

int _ortho_aabb_pz(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TVEC3, NULL);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TVEC3, NULL);
  float padding = luaL_checknumber(L, 3);

  vec3 vectors[2];
  glm_vec3_copy(union1->vec3_, vectors[0]);
  glm_vec3_copy(union2->vec3_, vectors[1]);

  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_ortho_aabb_pz(vectors, padding, union_->mat4_);
return 1;
}

int _ortho_default(lua_State* L) {
  float aspect = luaL_checknumber(L, 1);

  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_ortho_default(aspect, union_->mat4_);
return 1;
}

int _ortho_default_s(lua_State* L) {
  float aspect = luaL_checknumber(L, 1);
  float size = luaL_checknumber(L, 1);

  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_ortho_default_s(aspect, size, union_->mat4_);
return 1;
}

int _perspective(lua_State* L) {
  float fovy = luaL_checknumber(L, 1);
  float aspect = luaL_checknumber(L, 2);
  float near_v = luaL_checknumber(L, 3);
  float far_v = luaL_checknumber(L, 4);

  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_perspective(fovy, aspect, near_v, far_v, union_->mat4_);
return 1;
}

int _persp_move_far(lua_State* L) {
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  float delta = luaL_checknumber(L, 2);
  glm_persp_move_far(union_->mat4_, delta);
  lua_pushvalue(L, 1);
return 1;
}

int _perspective_default(lua_State* L) {
  float aspect = luaL_checknumber(L, 1);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_perspective_default(aspect, union_->mat4_);
return 1;
}

int _perspective_resize(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TMAT4, NULL);
  float aspect = luaL_checknumber(L, 2);
  glm_perspective_resize(aspect, union_->mat4_);
  lua_pushvalue(L, 1);
return 1;
}

int _lookat(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TVEC3, NULL);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TVEC3, NULL);
  lglm_union_t* union3 = lua_checklglmobject(L, 3, LUA_TVEC3, NULL);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_lookat(union1->vec3_, union2->vec3_, union3->vec3_, union_->mat4_);
return 1;
}

int _look(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TVEC3, NULL);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TVEC3, NULL);
  lglm_union_t* union3 = lua_checklglmobject(L, 3, LUA_TVEC3, NULL);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_look(union1->vec3_, union2->vec3_, union3->vec3_, union_->mat4_);
return 1;
}

int _look_anyup(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TVEC3, NULL);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TVEC3, NULL);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_look_anyup(union1->vec3_, union2->vec3_, union_->mat4_);
return 1;
}

int _persp_decomp(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TMAT4, NULL);
  union {
    float array_[6];
    struct {
      float near_v, far_v, top, bottom, left, right;
    };
  } frustum;

  glm_persp_decompv(union_->mat4_, frustum.array_);

  lua_pushnumber(L, frustum.left);
  lua_pushnumber(L, frustum.right);
  lua_pushnumber(L, frustum.bottom);
  lua_pushnumber(L, frustum.top);
  lua_pushnumber(L, frustum.near_v);
  lua_pushnumber(L, frustum.far_v);
return 6;
}

int _persp_fovy(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TMAT4, NULL);
  lua_pushnumber(L, glm_persp_fovy(union_->mat4_));
return 1;
}

int _persp_aspect(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TMAT4, NULL);
  lua_pushnumber(L, glm_persp_aspect(union_->mat4_));
return 1;
}

int _persp_sizes(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TMAT4, NULL);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TVEC4);
  glm_persp_sizes(union1->mat4_, glm_persp_fovy(union1->mat4_), union_->vec4_);
return 1;
}
