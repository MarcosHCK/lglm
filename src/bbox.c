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

typedef vec3 *bbox;

/* from constructors.c */
void _get_floats(lua_State* L, int floats_needed, float* floats);

int _aabb(lua_State* L) {
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TBBOX);
  _get_floats(L, 6, (float*)(union_));
return 1;
}

int _aabb_transform(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TBBOX);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TMAT4);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TBBOX);

  glm_aabb_transform((bbox)(union1->mat3_),
                     union2->mat4_,
                     (bbox)(union_->vec2_));
return 1;
}

int _aabb_merge(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TBBOX);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TBBOX);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TBBOX);

  glm_aabb_merge((bbox)(union1->mat3_),
                 (bbox)(union2->vec2_),
                 (bbox)(union_->vec2_));
return 1;
}

int _aabb_crop(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TBBOX);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TBBOX);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TBBOX);

  glm_aabb_crop((bbox)(union1->mat3_),
                 (bbox)(union2->vec2_),
                 (bbox)(union_->vec2_));
return 1;
}

int _aabb_crop_until(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TBBOX);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TBBOX);
  lglm_union_t* union3 = lua_checklglmobject(L, 3, LUA_TBBOX);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TBBOX);

  glm_aabb_crop((bbox)(union1->mat3_),
                 (bbox)(union2->vec2_),
                 (bbox)(union_->vec2_));

  glm_aabb_merge((bbox)(union3->vec2_),
                 (bbox)(union_->vec2_),
                 (bbox)(union_->vec2_));
return 1;
}

int _aabb_frustum(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TBBOX);
  lglm_union_t* union1;

  CGLM_ALIGN(GLM_ALIGNMENT)
  vec4 planes[6];
  int planes_n = 0;
  int i, idx = 2;
  int type, mtype;

  while(planes_n < 6)
  {
    type = lua_type(L, idx);
    switch(type)
    {
      case LUA_TTABLE:
        for(i = 1;;i++)
        {
          lua_pushinteger(L, i);
          lua_gettable(L, idx);

          union1 = lua_tolglmobject(L, -1, LUA_TVEC4);
          if(union1)
            glm_vec4_copy(union1->vec4_, planes[planes_n++]);
          else
            break;
        }
         ++idx;
        break;
      case LUA_TUSERDATA:
        union1 = lua_tolglmobject(L, idx++, LUA_TVEC4);
        glm_vec4_copy(union1->vec4_, planes[planes_n++]);
        break;
      case LUA_TNONE:
        _typeerror(L, idx, "table or " LUA_VEC4);
        return 0;
        break;
    }
  }

  if(planes_n >= 6)
  {
    luaL_error(L, "not enough vec4 object to operate");
    return 0;
  }

  int intercepts =
  glm_aabb_frustum((bbox)(union_), planes);
  lua_pushboolean(L, intercepts);
return 1;
}

int _aabb_invalidate(lua_State* L) {
  lglm_union_t* union_ = lua_clonelglmobject(L, 1, LUA_TBBOX);
  glm_aabb_invalidate((bbox)(union_->mat3_));
return 1;
}

int _aabb_isvalid(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TBBOX);
  int valid =
  glm_aabb_isvalid((bbox)(union_->mat3_));
  lua_pushboolean(L, valid);
return 1;
}

int _aabb_size(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TBBOX);
  float size =
  glm_aabb_size((bbox)(union_->mat3_));
  lua_pushnumber(L, size);
return 1;
}

int _aabb_radius(lua_State* L) {
  lglm_union_t* union_ = lua_checklglmobject(L, 1, LUA_TBBOX);
  float size =
  glm_aabb_radius((bbox)(union_->mat3_));
  lua_pushnumber(L, size);
return 1;
}

int _aabb_center(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TBBOX);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TVEC3);
  glm_aabb_center((bbox)(union1->mat3_), union_->vec3_);
return 1;
}

int _aabb_intercepts(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TBBOX);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TBBOX);
  int intercepts =
  glm_aabb_aabb((bbox)(union1->mat3_), (bbox)(union2->mat3_));
  lua_pushboolean(L, intercepts);
return 1;
}

int _aabb_sphere(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TBBOX);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TSPHERE);
  int intercepts =
  glm_aabb_sphere((bbox)(union1->mat3_), union2->vec4_);
  lua_pushboolean(L, intercepts);
return 1;
}

int _aabb_point(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TBBOX);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TVEC3);
  int intercepts =
  glm_aabb_point((bbox)(union1->mat3_), union2->vec3_);
  lua_pushboolean(L, intercepts);
return 1;
}

int _aabb_contains(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TBBOX);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TBBOX);
  int contains =
  glm_aabb_contains((bbox)(union1->mat3_), (bbox)(union2->mat3_));
  lua_pushboolean(L, contains);
return 1;
}

