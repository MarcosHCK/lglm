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

/* from operators.c */
void _only_vectors_error(lua_State* L, int idx, int base, int top);

int _norm2(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t* union_ = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC2:
      lua_pushnumber(L, glm_vec2_norm2(union_->vec2_));
      return 1;
      break;
    case LUA_TVEC3:
      lua_pushnumber(L, glm_vec3_norm2(union_->vec3_));
      return 1;
      break;
    case LUA_TVEC4:
      lua_pushnumber(L, glm_vec4_norm2(union_->vec4_));
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _norm(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t* union_ = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC2:
      lua_pushnumber(L, glm_vec2_norm(union_->vec2_));
      return 1;
      break;
    case LUA_TVEC3:
      lua_pushnumber(L, glm_vec3_norm(union_->vec3_));
      return 1;
      break;
    case LUA_TVEC4:
      lua_pushnumber(L, glm_vec4_norm(union_->vec4_));
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _norm_one(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t* union_ = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC3:
      lua_pushnumber(L, glm_vec3_norm_one(union_->vec3_));
      return 1;
      break;
    case LUA_TVEC4:
      lua_pushnumber(L, glm_vec4_norm_one(union_->vec4_));
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, LUA_TVEC3, LUA_TVEC4);
      break;
  }
return 0;
}

int _norm_inf(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t* union_ = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC3:
      lua_pushnumber(L, glm_vec3_norm_inf(union_->vec3_));
      return 1;
      break;
    case LUA_TVEC4:
      lua_pushnumber(L, glm_vec4_norm_inf(union_->vec4_));
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, LUA_TVEC3, LUA_TVEC4);
      break;
  }
return 0;
}

int _scale(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union1 = _tolglmobject(L, 1, mtype);
  if(lua_isnumber(L, 2))
  {
    float scale = lua_tonumber(L, 2);
    switch(mtype) 
    {
      case LUA_TVEC2:
        union_ = lua_newlglmobject(L, mtype);
        glm_vec2_scale(union1->vec2_, scale, union_->vec2_);
        return 1;
        break;
      case LUA_TVEC3:
        union_ = lua_newlglmobject(L, mtype);
        glm_vec3_scale(union1->vec3_, scale, union_->vec3_);
        return 1;
        break;
      case LUA_TVEC4:
        union_ = lua_newlglmobject(L, mtype);
        glm_vec4_scale(union1->vec4_, scale, union_->vec4_);
        return 1;
        break;
      case LUA_TMAT2:
        union_ = lua_newlglmobject(L, mtype);
        glm_mat2_copy(union1->mat2_, union_->mat2_);
        glm_mat2_scale(union_->mat2_, scale);
        break;
      case LUA_TMAT3:
        union_ = lua_newlglmobject(L, mtype);
        glm_mat3_copy(union1->mat3_, union_->mat3_);
        glm_mat3_scale(union_->mat3_, scale);
        break;
      case LUA_TMAT4:
        union_ = lua_newlglmobject(L, mtype);
        glm_mat4_copy(union1->mat4_, union_->mat4_);
        glm_mat4_scale(union_->mat4_, scale);
        break;
    }
  }
  else
  {
    int othertype = _checklglmobject(L, 2, LUA_TGLMANY, TRUE);
    lglm_union_t *union2 = _tolglmobject(L, 2, othertype);

    switch(othertype)
    {
      case LUA_TVEC3:
        if(mtype != LUA_TMAT4)
          _typeerror(L, 2, LUA_MAT4);
        union_ = lua_newlglmobject(L, mtype);
        glm_scale_to(union1->mat4_, union2->vec3_, union_->mat4_);
        return 1;
        break;
      case LUA_TVEC2:
        if(mtype != LUA_TMAT3)
          _typeerror(L, 2, LUA_MAT3);
        union_ = lua_newlglmobject(L, mtype);
        glm_mat3_copy(union1->mat3_, union_->mat3_);
        glm_scale2d(union_->mat3_, union2->vec2_);
        return 1;
        break;
      default:
        _only_vectors_error(L, 2, LUA_TVEC2, LUA_TVEC3);
        break;
    }
  }
return 0;
}

int _scale_as(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union1 = _tolglmobject(L, 1, mtype);
  float scale = luaL_checknumber(L, 2);
  switch(mtype) 
  {
    case LUA_TVEC2:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec2_scale_as(union1->vec2_, scale, union_->vec2_);
      return 1;
      break;
    case LUA_TVEC3:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_scale_as(union1->vec3_, scale, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_scale_as(union1->vec4_, scale, union_->vec4_);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _negate(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union1 = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC2:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec2_negate_to(union1->vec2_, union_->vec2_);
      return 1;
      break;
    case LUA_TVEC3:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_negate_to(union1->vec3_, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_negate_to(union1->vec4_, union_->vec4_);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _normalize(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union1 = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC2:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec2_normalize_to(union1->vec2_, union_->vec2_);
      return 1;
      break;
    case LUA_TVEC3:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_normalize_to(union1->vec3_, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_normalize_to(union1->vec4_, union_->vec4_);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _angle(lua_State* L) {
  lglm_union_t *union1 = lua_checklglmobject(L, 1, LUA_TVEC3),
               *union2 = lua_checklglmobject(L, 2, LUA_TVEC3);
  lua_pushnumber(L, glm_vec3_angle(union1->vec3_, union2->vec3_));
return 1;
}

int _rotate(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TVEC3, TRUE);
  lglm_union_t *union_, *union2, *union1 = _tolglmobject(L, 1, mtype);
  float axis;

  switch(mtype)
  {
    case LUA_TMAT4:
      axis = lua_tonumber(L, 2);
      union2 = lua_checklglmobject(L, 3, LUA_TVEC3);
      union_ = lua_newlglmobject(L, LUA_TMAT4);
      glm_mat4_copy(union1->mat4_, union_->mat4_);
      glm_rotate(union_->mat4_, axis, union2->vec3_);
      return 1;
      break;
    case LUA_TVEC3:
      if(lua_isnumber(L, 2))
      {
        float angle = lua_tonumber(L, 2);
        union_ = lua_newlglmobject(L, LUA_TVEC3);
        glm_vec3_rotate(union1->vec3_, angle, union_->vec3_);
        return 1;
      }
      else
      {
        mtype = _checklglmobject(L, 2, LUA_TGLMANY, TRUE);
        union2 = _tolglmobject(L, 2, mtype);
        switch(mtype)
        {
          case LUA_TMAT3:
            glm_vec3_rotate_m3(union2->mat3_, union1->vec3_, union_->vec3_);
            break;
          case LUA_TMAT4:
            glm_vec3_rotate_m4(union2->mat4_, union1->vec3_, union_->vec3_);
            break;
          default:
            _typeerror(L, 2, LUA_MAT3 " or " LUA_MAT4);
            break;
        }
      }
      break;
    default:
      _typeerror(L, 1, LUA_MAT4 " or " LUA_VEC3);
      break;
  }
return 0;
}

int _proj(lua_State* L) {
  lglm_union_t *union1 = lua_checklglmobject(L, 1, LUA_TVEC3),
               *union2 = lua_checklglmobject(L, 2, LUA_TVEC3),
               *union_ = lua_newlglmobject(L, LUA_TVEC3);
  glm_vec3_proj(union1->vec3_, union2->vec3_, union_->vec3_);
return 1;
}

int _center(lua_State* L) {
  lglm_union_t *union1 = lua_checklglmobject(L, 1, LUA_TVEC3),
               *union2 = lua_checklglmobject(L, 2, LUA_TVEC3),
               *union_ = lua_newlglmobject(L, LUA_TVEC3);
  glm_vec3_center(union1->vec3_, union2->vec3_, union_->vec3_);
return 1;
}

int _distance2(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union2, *union1 = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC2:
      union2 = lua_checklglmobject(L, 2, mtype);
      lua_pushnumber(L, glm_vec2_distance2(union1->vec2_, union2->vec2_));
      return 1;
      break;
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 2, mtype);
      lua_pushnumber(L, glm_vec3_distance2(union1->vec3_, union2->vec3_));
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 2, mtype);
      lua_pushnumber(L, glm_vec4_distance2(union1->vec4_, union2->vec4_));
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _distance(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union2, *union1 = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC2:
      union2 = lua_checklglmobject(L, 2, mtype);
      lua_pushnumber(L, glm_vec2_distance(union1->vec2_, union2->vec2_));
      return 1;
      break;
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 2, mtype);
      lua_pushnumber(L, glm_vec3_distance(union1->vec3_, union2->vec3_));
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 2, mtype);
      lua_pushnumber(L, glm_vec4_distance(union1->vec4_, union2->vec4_));
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _max(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_ = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC2:
      lua_pushnumber(L, glm_vec2_max(union_->vec2_));
      return 1;
      break;
    case LUA_TVEC3:
      lua_pushnumber(L, glm_vec3_max(union_->vec3_));
      return 1;
      break;
    case LUA_TVEC4:
      lua_pushnumber(L, glm_vec4_max(union_->vec4_));
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _min(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_ = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC2:
      lua_pushnumber(L, glm_vec2_min(union_->vec2_));
      return 1;
      break;
    case LUA_TVEC3:
      lua_pushnumber(L, glm_vec3_min(union_->vec3_));
      return 1;
      break;
    case LUA_TVEC4:
      lua_pushnumber(L, glm_vec4_min(union_->vec4_));
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _maxv(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union2, *union1 = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC2:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec2_maxv(union1->vec2_, union2->vec2_, union_->vec2_);
      return 1;
      break;
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_maxv(union1->vec3_, union2->vec3_, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_maxv(union1->vec4_, union2->vec4_, union_->vec4_);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _minv(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union2, *union1 = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC2:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec2_minv(union1->vec2_, union2->vec2_, union_->vec2_);
      return 1;
      break;
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_minv(union1->vec3_, union2->vec3_, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_minv(union1->vec4_, union2->vec4_, union_->vec4_);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, 0, LUA_TVEC4);
      break;
  }
return 0;
}

int _ortho(lua_State* L) {
  lglm_union_t *union1 = lua_checklglmobject(L, 1, LUA_TVEC3),
               *union_ = lua_newlglmobject(L, LUA_TVEC3);
  glm_vec3_ortho(union1->vec3_, union_->vec3_);
return 1;
}

static
int _clamp_vector(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union1 = _tolglmobject(L, 1, mtype);
  float min_v = luaL_checknumber(L, 2),
        max_v = luaL_checknumber(L, 3);

  switch(mtype) 
  {
    case LUA_TVEC2:
      union_ = lua_clonelglmobject(L, 1, mtype);
      glm_vec2_clamp(union_->vec2_, min_v, max_v);
      return 1;
      break;
    case LUA_TVEC3:
      union_ = lua_clonelglmobject(L, 1, mtype);
      glm_vec3_clamp(union_->vec3_, min_v, max_v);
      return 1;
      break;
    case LUA_TVEC4:
      union_ = lua_clonelglmobject(L, 1, mtype);
      glm_vec4_clamp(union_->vec4_, min_v, max_v);
      return 1;
      break;
  }
return 0;
}

int _clamp(lua_State* L) {
  if(lua_isnumber(L, 1))
  {
    float val = luaL_checknumber(L, 1),
          min_v = luaL_checknumber(L, 2),
          max_v = luaL_checknumber(L, 3);

    lua_pushnumber(L, glm_clamp(val, min_v, max_v));
    return 1;
  }
  else
  {
    int return_ = _clamp_vector(L);
    if(return_ == 0)
    {
      _typeerror(L, 1, "number or " LUA_VEC2 " or " LUA_VEC3 " or " LUA_VEC4);
    }

    return return_;
  }
return 0;
}

static
int _lerp_vector(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union2, *union1 = _tolglmobject(L, 1, mtype);
  float t = luaL_checknumber(L, 2);
  switch(mtype) 
  {
    case LUA_TVEC2:
      union2 = lua_checklglmobject(L, 3, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec2_lerp(union1->vec2_, union2->vec2_, t, union_->vec2_);
      return 1;
      break;
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 3, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_lerp(union1->vec3_, union2->vec3_, t, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 3, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_lerp(union1->vec4_, union2->vec4_, t, union_->vec4_);
      return 1;
      break;
  }
return 0;
}

int _lerp(lua_State* L) {
  if(lua_isnumber(L, 1))
  {
    float from = luaL_checknumber(L, 1),
          to = luaL_checknumber(L, 2),
          t = luaL_checknumber(L, 3);

    lua_pushnumber(L, glm_lerp(from, to, t));
    return 1;
  }
  else
  {
    int return_ = _lerp_vector(L);
    if(return_ == 0)
    {
      _typeerror(L, 1, "number or " LUA_VEC2 " or " LUA_VEC3 " or " LUA_VEC4);
    }

    return return_;
  }
return 0;
}

static
int _lerpc_vector(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union2, *union1 = _tolglmobject(L, 1, mtype);
  float t = luaL_checknumber(L, 2);
  switch(mtype) 
  {
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 3, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_lerp(union1->vec3_, union2->vec3_, t, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 3, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_lerp(union1->vec4_, union2->vec4_, t, union_->vec4_);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, LUA_TVEC3, LUA_TVEC4);
      break;
  }
return 0;
}

int _lerpc(lua_State* L) {
  if(lua_isnumber(L, 1))
  {
    float from = luaL_checknumber(L, 1),
          to = luaL_checknumber(L, 2),
          t = luaL_checknumber(L, 3);

    lua_pushnumber(L, glm_lerpc(from, to, t));
    return 1;
  }
  else
  {
    int return_ = _lerpc_vector(L);
    if(return_ == 0)
    {
      _typeerror(L, 1, "number or " LUA_VEC3 " or " LUA_VEC4);
    }

    return return_;
  }
return 0;
}

int _mix(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union2, *union1 = _tolglmobject(L, 1, mtype);
  float t = luaL_checknumber(L, 2);
  switch(mtype) 
  {
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 3, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_mix(union1->vec3_, union2->vec3_, t, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 3, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_mix(union1->vec4_, union2->vec4_, t, union_->vec4_);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, LUA_TVEC3, LUA_TVEC4);
      break;
  }
return 0;
}

int _mixc(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union2, *union1 = _tolglmobject(L, 1, mtype);
  float t = luaL_checknumber(L, 2);
  switch(mtype) 
  {
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 3, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_mixc(union1->vec3_, union2->vec3_, t, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 3, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_mixc(union1->vec4_, union2->vec4_, t, union_->vec4_);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, LUA_TVEC3, LUA_TVEC4);
      break;
  }
return 0;
}

int _step_uni(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union1 = _tolglmobject(L, 1, mtype);
  float step = luaL_checknumber(L, 2);

  switch(mtype) 
  {
    case LUA_TVEC3:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_step_uni(step, union1->vec3_, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_step_uni(step, union1->vec4_, union_->vec4_);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, LUA_TVEC3, LUA_TVEC4);
      break;
  }
return 0;
}

static
int _step_vector(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union2, *union1 = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_step(union1->vec3_, union2->vec3_, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_step(union1->vec4_, union2->vec4_, union_->vec4_);
      return 1;
      break;
  }
return 0;
}

int _step(lua_State* L) {
  if(lua_isnumber(L, 1))
  {
    float edge = luaL_checknumber(L, 1),
          x = luaL_checknumber(L, 2);

    lua_pushnumber(L, glm_step(edge, x));
    return 1;
  }
  else
  {
    int return_ = _step_vector(L);
    if(return_ == 0)
    {
      _typeerror(L, 1, "number or " LUA_VEC3 " or " LUA_VEC4);
    }

    return return_;
  }
return 0;
}

int _smoothstep_uni(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union1 = _tolglmobject(L, 1, mtype);
  float edge1 = luaL_checknumber(L, 2),
        edge2 = luaL_checknumber(L, 3);

  switch(mtype) 
  {
    case LUA_TVEC3:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_smoothstep_uni(edge1, edge2, union1->vec3_, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_smoothstep_uni(edge1, edge2, union1->vec4_, union_->vec4_);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, LUA_TVEC3, LUA_TVEC4);
      break;
  }
return 0;
}

static
int _smoothstep_vector(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union3, *union2, *union1 = _tolglmobject(L, 1, mtype);
  switch(mtype) 
  {
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 2, mtype);
      union3 = lua_checklglmobject(L, 3, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_smoothstep(union1->vec3_, union2->vec3_, union3->vec3_, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 2, mtype);
      union3 = lua_checklglmobject(L, 3, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_smoothstep(union1->vec4_, union2->vec4_, union3->vec4_, union_->vec4_);
      return 1;
      break;
  }
return 0;
}

int _smoothstep(lua_State* L) {
  if(lua_isnumber(L, 1))
  {
    float edge1 = luaL_checknumber(L, 1),
          edge2 = luaL_checknumber(L, 2),
          x = luaL_checknumber(L, 3);

    lua_pushnumber(L, glm_smoothstep(edge1, edge2, x));
    return 1;
  }
  else
  {
    int return_ = _smoothstep_vector(L);
    if(return_ == 0)
    {
      _typeerror(L, 1, "number or " LUA_VEC3 " or " LUA_VEC4);
    }

    return return_;
  }
return 0;
}

static
int _smoothinterp_vector(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union2, *union1 = _tolglmobject(L, 1, mtype);
  float t = luaL_checknumber(L, 3);

  switch(mtype) 
  {
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_smoothinterp(union1->vec3_, union2->vec3_, t, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_smoothinterp(union1->vec4_, union2->vec4_, t, union_->vec4_);
      return 1;
      break;
  }
return 0;
}

int _smoothinterp(lua_State* L) {
  if(lua_isnumber(L, 1))
  {
    float from = luaL_checknumber(L, 1),
          to = luaL_checknumber(L, 2),
          x = luaL_checknumber(L, 3);

    lua_pushnumber(L, glm_smoothinterp(from, to, x));
    return 1;
  }
  else
  {
    int return_ = _smoothinterp_vector(L);
    if(return_ == 0)
    {
      _typeerror(L, 1, "number or " LUA_VEC3 " or " LUA_VEC4);
    }

    return return_;
  }
return 0;
}

static
int _smoothinterpc_vector(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union_, *union2, *union1 = _tolglmobject(L, 1, mtype);
  float t = luaL_checknumber(L, 3);

  switch(mtype) 
  {
    case LUA_TVEC3:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec3_smoothinterpc(union1->vec3_, union2->vec3_, t, union_->vec3_);
      return 1;
      break;
    case LUA_TVEC4:
      union2 = lua_checklglmobject(L, 2, mtype);
      union_ = lua_newlglmobject(L, mtype);
      glm_vec4_smoothinterpc(union1->vec4_, union2->vec4_, t, union_->vec4_);
      return 1;
      break;
  }
return 0;
}

int _smoothinterpc(lua_State* L) {
  if(lua_isnumber(L, 1))
  {
    float from = luaL_checknumber(L, 1),
          to = luaL_checknumber(L, 2),
          x = luaL_checknumber(L, 3);

    lua_pushnumber(L, glm_smoothinterpc(from, to, x));
    return 1;
  }
  else
  {
    int return_ = _smoothinterpc_vector(L);
    if(return_ == 0)
    {
      _typeerror(L, 1, "number or " LUA_VEC3 " or " LUA_VEC4);
    }

    return return_;
  }
return 0;
}

int _invert(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t *union2, *union_ = _tolglmobject(L, 1, mtype);

  switch(mtype)
  {
    case LUA_TMAT2:
      union2 = lua_newlglmobject(L, mtype);
      glm_mat2_inv(union_->mat2_, union2->mat2_);
      return 1;
      break;
    case LUA_TMAT3:
      union2 = lua_newlglmobject(L, mtype);
      glm_mat3_inv(union_->mat3_, union2->mat3_);
      return 1;
      break;
    case LUA_TMAT4:
      union2 = lua_newlglmobject(L, mtype);
      glm_mat4_inv(union_->mat4_, union2->mat4_);
      return 1;
      break;
    default:
      _only_vectors_error(L, 1, LUA_TMAT2, LUA_TMAT4);
      break;
  }
return 0;
}
