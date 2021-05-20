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

int _translate(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TMAT4, NULL);
  lglm_union_t* union2 = lua_checklglmobject(L, 2, LUA_TVEC3, NULL);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_translate_to(union1->mat4_, union2->vec3_, union_->mat4_);
return 1;
}

int _translate_x(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TMAT4, NULL);
  float number = luaL_checknumber(L, 2);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_mat4_copy(union1->mat4_, union_->mat4_);
  glm_translate_x(union_->mat4_, number);
return 1;
}

int _translate_y(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TMAT4, NULL);
  float number = luaL_checknumber(L, 2);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_mat4_copy(union1->mat4_, union_->mat4_);
  glm_translate_y(union_->mat4_, number);
return 1;
}

int _translate_z(lua_State* L) {
  lglm_union_t* union1 = lua_checklglmobject(L, 1, LUA_TMAT4, NULL);
  float number = luaL_checknumber(L, 2);
  lglm_union_t* union_ = lua_newlglmobject(L, LUA_TMAT4);
  glm_mat4_copy(union1->mat4_, union_->mat4_);
  glm_translate_z(union_->mat4_, number);
return 1;
}
