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

int _quat(lua_State* L) {
  float angle = luaL_checknumber(L, 1);
  lglm_union_t *union_, *union1 =
  lua_tolglmobject(L, 2, LUA_TVEC3);
  if(union1 == NULL)
  {
    float x = luaL_checknumber(L, 2),
          y = luaL_checknumber(L, 3),
          z = luaL_checknumber(L, 4);

    union_ = lua_newlglmobject(L, LUA_TVEC4);
    glm_quat(union_->vec4_, angle, x, y, z);
    return 1;
  }
  else
  {
    union_ = lua_newlglmobject(L, LUA_TVEC4);
    glm_quatv(union_->vec4_, angle, union1->vec3_);
    return 1;
  }
return 0;
}
