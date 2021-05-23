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

#if LUA_VERSION_NUM >= 502
#define new_lib(L, l) (luaL_newlib(L, l))
#else
#define new_lib(L, l) (lua_newtable(L), luaL_register(L, NULL, l))
#endif

/*
 * Taken from Lua source
 *
 */
int _typeerror(lua_State *L, int arg, const char *tname) {
  const char *msg;
  const char *typearg;  /* name for the type of the actual argument */
  if (luaL_getmetafield(L, arg, "__name") == LUA_TSTRING)
    typearg = lua_tostring(L, -1);  /* use the given type name */
  else if (lua_type(L, arg) == LUA_TLIGHTUSERDATA)
    typearg = "light userdata";  /* special name for messages */
  else
    typearg = luaL_typename(L, arg);  /* standard name */
  msg = lua_pushfstring(L, "%s expected, got %s", tname, typearg);
  return luaL_argerror(L, arg, msg);
}

/*
 * Public API
 *
 */
lglm_union_t* lua_newlglmobject(lua_State* L, int mtype) {
  lglm_object_t* obj = (lglm_object_t*)lua_newuserdata(L, sizeof(lglm_object_t));
  luaL_setmetatable(L, lglm_typenames[mtype]);

  memset(obj, 0, sizeof(lglm_object_t));
  void* obj_ = (void*)obj;
  uintptr_t ptr_ = (uintptr_t)obj;

  obj->offset = GLM_ALIGNMENT - (ptr_ & (GLM_ALIGNMENT-1));
  obj->mtype = mtype;
return (lglm_union_t*)(obj_ + obj->offset);
}

static
int _trowtypeerror(lua_State* L, int idx, int mtype) {
  if(mtype == LUA_TGLMANY)
  {
    luaL_Buffer b;
    luaL_buffinit(L, &b);

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
  else
  {
    _typeerror(L, idx, lglm_typenames[mtype]);
  }
}

int _checklglmobject_full(lua_State* L, int idx, int mtype, int throw_error, int any_top) {
  void* p = lua_touserdata(L, idx);
  if(p)
  {
/*
 * To avoid confusions:
 * at the end of function, if 'p'
 * variable (declared above),
 * is NULL, type needs are not
 * met.
 *
 */
    if(lua_getmetatable(L, idx))
    {
      if(mtype == LUA_TGLMANY)
      {
        int matches = FALSE;
        for(mtype = 0;mtype < any_top;mtype++)
        {
          luaL_getmetatable(L, lglm_typenames[mtype]);
          if(lua_rawequal(L, -1, -2))
          {
            matches = TRUE;
            lua_pop(L, 1);
            break;
          }

          lua_pop(L, 1);
        }

        if(!matches)
          p = NULL;
      }
      else
      {
        luaL_getmetatable(L, lglm_typenames[mtype]);
        if(!lua_rawequal(L, -1, -2))
          p = NULL;
        lua_pop(L, 1);
      }

      lua_pop(L, 1);
    }
    else
    p = NULL;
  }

  if(p == NULL)
  {
    if(throw_error)
      _trowtypeerror(L, idx, mtype);
    return LUA_TGLMANY;
  }

  lglm_object_t* obj = (lglm_object_t*)(p);
return obj->mtype;
}

lglm_union_t* _tolglmobject(lua_State* L, int idx, int mtype) {
  lglm_object_t* obj = (lglm_object_t*)lua_touserdata(L, idx);
  void* obj_ = (void*)obj;

  if _UNLIKELY(mtype != LUA_TGLMANY && mtype != obj->mtype)
  {
    luaL_error(L, "lglm type corruption!");
    return NULL;
  }
return (lglm_union_t*)(obj_ + obj->offset);
}

int _checklglmobject(lua_State* L, int idx, int mtype, int throw_error) {
  return _checklglmobject_full(L, idx, mtype, throw_error, _LGLM_TYPES);
}

int _checklglmobject_ex(lua_State* L, int idx, int mtype, int throw_error) {
  return _checklglmobject_full(L, idx, mtype, throw_error, _LGLM_TYPES_EX);
}

int lua_lglmtype(lua_State* L, int idx) {
return _checklglmobject_full(L, idx, LUA_TGLMANY, FALSE, _LGLM_TYPES_EX);
}

lglm_union_t* lua_checklglmobject(lua_State* L, int idx, int mtype) {
  int got =
  _checklglmobject(L, idx, mtype, TRUE);
  if(got == LUA_TGLMANY)
    return NULL;

return _tolglmobject(L, idx, mtype);
}

lglm_union_t* lua_checklglmvec2(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TVEC2);
}

lglm_union_t* lua_checklglmvec3(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TVEC3);
}

lglm_union_t* lua_checklglmvec4(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TVEC4);
}

lglm_union_t* lua_checklglmmat2(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TMAT2);
}

lglm_union_t* lua_checklglmmat3(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TMAT2);
}

lglm_union_t* lua_checklglmmat4(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TMAT2);
}

lglm_union_t* lua_checklglmbbox(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TBBOX);
}

lglm_union_t* lua_checklglmsphere(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TSPHERE);
}

lglm_union_t* lua_clonelglmobject(lua_State* L, int idx, int mtype) {
  lglm_union_t* from = lua_tolglmobject(L, idx, mtype);
  lglm_union_t* to = lua_newlglmobject(L, mtype);

  switch(mtype)
  {
    case LUA_TVEC2:
      glm_vec2_copy(from->vec2_, to->vec2_);
      break;
    case LUA_TVEC3:
      glm_vec3_copy(from->vec3_, to->vec3_);
      break;
    case LUA_TVEC4:
      glm_vec4_copy(from->vec4_, to->vec4_);
      break;
    case LUA_TMAT2:
      glm_mat2_copy(from->mat2_, to->mat2_);
      break;
    case LUA_TMAT3:
      glm_mat3_copy(from->mat3_, to->mat3_);
      break;
    case LUA_TMAT4:
      glm_mat4_copy(from->mat4_, to->mat4_);
      break;
  }
return to;
}

lglm_union_t* lua_tolglmobject(lua_State* L, int idx, int mtype) {
  if(_checklglmobject(L, idx, mtype, FALSE) != LUA_TGLMANY)
  {
    return _tolglmobject(L, idx, mtype);
  }
return NULL;
}

lglm_union_t* lua_tolglmvec2(lua_State* L, int idx) {
return lua_tolglmobject(L, idx, LUA_TVEC2);
}

lglm_union_t* lua_tolglmvec3(lua_State* L, int idx) {
return lua_tolglmobject(L, idx, LUA_TVEC3);
}

lglm_union_t* lua_tolglmvec4(lua_State* L, int idx) {
return lua_tolglmobject(L, idx, LUA_TVEC4);
}

lglm_union_t* lua_tolglmmat2(lua_State* L, int idx) {
return lua_tolglmobject(L, idx, LUA_TMAT2);
}

lglm_union_t* lua_tolglmmat3(lua_State* L, int idx) {
return lua_tolglmobject(L, idx, LUA_TMAT3);
}

lglm_union_t* lua_tolglmmat4(lua_State* L, int idx) {
return lua_tolglmobject(L, idx, LUA_TMAT4);
}

lglm_union_t* lua_tolglmbbox(lua_State* L, int idx) {
return lua_tolglmobject(L, idx, LUA_TBBOX);
}

lglm_union_t* lua_tolglmsphere(lua_State* L, int idx) {
return lua_tolglmobject(L, idx, LUA_TSPHERE);
}

/*
 * Module stuff
 *
 */

static
int _unpack_idx(lua_State* L, int idx) {
  int mtype = _checklglmobject_ex(L, idx, LUA_TGLMANY, TRUE);
  lglm_union_t* union_ = lua_tolglmobject(L, idx, mtype);

  if _UNLIKELY(mtype == LUA_TGLMANY)
    return 0;

  switch(mtype)
  {
    case LUA_TVEC2:
        lua_pushnumber(L, union_->vec2_[0]);
        lua_pushnumber(L, union_->vec2_[1]);
        return 2;
        break;
    case LUA_TVEC3:
        lua_pushnumber(L, union_->vec3_[0]);
        lua_pushnumber(L, union_->vec3_[1]);
        lua_pushnumber(L, union_->vec3_[2]);
        return 3;
        break;
    case LUA_TVEC4:
        lua_pushnumber(L, union_->vec4_[0]);
        lua_pushnumber(L, union_->vec4_[1]);
        lua_pushnumber(L, union_->vec4_[2]);
        lua_pushnumber(L, union_->vec4_[3]);
        return 4;
        break;
    case LUA_TMAT2:
        lua_pushnumber(L, union_->mat2_[0][0]);
        lua_pushnumber(L, union_->mat2_[0][1]);
        lua_pushnumber(L, union_->mat2_[1][0]);
        lua_pushnumber(L, union_->mat2_[1][1]);
        return 4;
        break;
    case LUA_TMAT3:
        lua_pushnumber(L, union_->mat3_[0][0]);
        lua_pushnumber(L, union_->mat3_[0][1]);
        lua_pushnumber(L, union_->mat3_[0][2]);
        lua_pushnumber(L, union_->mat3_[1][0]);
        lua_pushnumber(L, union_->mat3_[1][1]);
        lua_pushnumber(L, union_->mat3_[1][2]);
        lua_pushnumber(L, union_->mat3_[2][0]);
        lua_pushnumber(L, union_->mat3_[2][1]);
        lua_pushnumber(L, union_->mat3_[2][2]);
        return 9;
        break;
    case LUA_TMAT4:
        lua_pushnumber(L, union_->mat4_[0][0]);
        lua_pushnumber(L, union_->mat4_[0][1]);
        lua_pushnumber(L, union_->mat4_[0][2]);
        lua_pushnumber(L, union_->mat4_[0][3]);
        lua_pushnumber(L, union_->mat4_[1][0]);
        lua_pushnumber(L, union_->mat4_[1][1]);
        lua_pushnumber(L, union_->mat4_[1][2]);
        lua_pushnumber(L, union_->mat4_[1][3]);
        lua_pushnumber(L, union_->mat4_[2][0]);
        lua_pushnumber(L, union_->mat4_[2][1]);
        lua_pushnumber(L, union_->mat4_[2][2]);
        lua_pushnumber(L, union_->mat4_[2][3]);
        lua_pushnumber(L, union_->mat4_[3][0]);
        lua_pushnumber(L, union_->mat4_[3][1]);
        lua_pushnumber(L, union_->mat4_[3][2]);
        lua_pushnumber(L, union_->mat4_[3][3]);
        return 16;
        break;
      default:
        if(lua_getmetatable(L, idx))
        {
          int top = lua_gettop(L) - 1;

          lua_pushstring(L, "__unpack");
          lua_gettable(L, -2);
          lua_remove(L, -2);

          if(lua_isnil(L, -1))
          {
            lua_pop(L, 1);
            return 0;
          }

          lua_pushvalue(L, idx);
          lua_call(L, 1, LUA_MULTRET);
          return lua_gettop(L) - top;
        }
        return 0;
        break;
  }
return 0;
}

static
int _unpack(lua_State* L) {
  int i, top = lua_gettop(L);
  int r = 0;

  for(i = 1;i <= top;i++)
  r += _unpack_idx(L, i);
return r;
}

static
int _clone(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lua_clonelglmobject(L, 1, mtype);
return 1;
}

static
int _rad(lua_State* L) {
  lua_pushnumber(L,
      glm_rad(luaL_checknumber(L, 1))
    );
return 1;
}

static
int _deg(lua_State* L) {
  lua_pushnumber(L,
      glm_deg(luaL_checknumber(L, 1))
    );
return 1;
}

#define LGLM_SYMBOL(sym) { #sym, _##sym },

static
const struct luaL_Reg lglm_lib[] = {
/*
 * Basic operations
 *
 */
  LGLM_SYMBOL(unpack) LGLM_SYMBOL(clone)
  LGLM_SYMBOL(rad) LGLM_SYMBOL(deg)

/*
 * Constructors
 *
 */
  LGLM_SYMBOL(vec2) LGLM_SYMBOL(vec3) LGLM_SYMBOL(vec4)
  LGLM_SYMBOL(mat2) LGLM_SYMBOL(mat3) LGLM_SYMBOL(mat4)

/*
 * Operators
 *
 */
  LGLM_SYMBOL(add) LGLM_SYMBOL(adds)
  LGLM_SYMBOL(sub) LGLM_SYMBOL(subs)
  LGLM_SYMBOL(dot) LGLM_SYMBOL(cross)
  LGLM_SYMBOL(div) LGLM_SYMBOL(divs)

/*
 * Operations
 *
 */
  LGLM_SYMBOL(norm2) LGLM_SYMBOL(norm)
  LGLM_SYMBOL(scale) LGLM_SYMBOL(scale_as)
  LGLM_SYMBOL(negate)
  LGLM_SYMBOL(normalize)
  LGLM_SYMBOL(angle)
  LGLM_SYMBOL(rotate)
  LGLM_SYMBOL(proj)
  LGLM_SYMBOL(center)
  LGLM_SYMBOL(distance2) LGLM_SYMBOL(distance)
  LGLM_SYMBOL(max) LGLM_SYMBOL(min)
  LGLM_SYMBOL(maxv) LGLM_SYMBOL(minv)
  LGLM_SYMBOL(ortho)
  LGLM_SYMBOL(clamp)
  LGLM_SYMBOL(lerp) LGLM_SYMBOL(lerpc)
  LGLM_SYMBOL(mix) LGLM_SYMBOL(mixc)
  LGLM_SYMBOL(step_uni)
  LGLM_SYMBOL(step)
  LGLM_SYMBOL(smoothstep_uni)
  LGLM_SYMBOL(smoothstep)
  LGLM_SYMBOL(smoothinterp)
  LGLM_SYMBOL(smoothinterpc)
  LGLM_SYMBOL(invert)

/*
 * Bounding box
 * at bbox.c
 *
 */
  LGLM_SYMBOL(aabb)
  LGLM_SYMBOL(aabb_transform)
  LGLM_SYMBOL(aabb_merge)
  LGLM_SYMBOL(aabb_crop)
  LGLM_SYMBOL(aabb_crop_until)
  LGLM_SYMBOL(aabb_frustum)
  LGLM_SYMBOL(aabb_invalidate)
  LGLM_SYMBOL(aabb_isvalid)
  LGLM_SYMBOL(aabb_size)
  LGLM_SYMBOL(aabb_radius)
  LGLM_SYMBOL(aabb_center)
  {"aabb_aabb", _aabb_intercepts},
  LGLM_SYMBOL(aabb_intercepts)
  LGLM_SYMBOL(aabb_sphere)
  LGLM_SYMBOL(aabb_point)
  LGLM_SYMBOL(aabb_contains)

/*
 * Quaternion
 * at versor.c
 *
 */
  LGLM_SYMBOL(quat)
  LGLM_SYMBOL(quat_norm)
  LGLM_SYMBOL(quat_normalize)
  LGLM_SYMBOL(quat_dot)
  LGLM_SYMBOL(quat_conjugate)
  LGLM_SYMBOL(quat_inv)
  LGLM_SYMBOL(quat_add)
  LGLM_SYMBOL(quat_sub)
  LGLM_SYMBOL(quat_real)
  LGLM_SYMBOL(quat_imag)
  LGLM_SYMBOL(quat_imagn)
  LGLM_SYMBOL(quat_imaglen)
  LGLM_SYMBOL(quat_angle)
  LGLM_SYMBOL(quat_axis)
  LGLM_SYMBOL(quat_mul)
  LGLM_SYMBOL(quat_mat4)
  LGLM_SYMBOL(quat_mat4t)
  LGLM_SYMBOL(quat_mat3)
  LGLM_SYMBOL(quat_mat3t)
  LGLM_SYMBOL(quat_lerp)
  LGLM_SYMBOL(quat_lerpc)
  LGLM_SYMBOL(quat_slerp)
  LGLM_SYMBOL(quat_look)
  LGLM_SYMBOL(quat_for)
  LGLM_SYMBOL(quat_forp)
  LGLM_SYMBOL(qaut_rotate)

/*
 * Sphere
 *
 */
  LGLM_SYMBOL(sphere)
  LGLM_SYMBOL(sphere_transform)
  LGLM_SYMBOL(sphere_merge)
  LGLM_SYMBOL(sphere_intersects)
  {"sphere_sphere", _sphere_intersects},
  LGLM_SYMBOL(sphere_intersects_point)
  {"sphere_point", _sphere_intersects_point},

/*
 * Camera
 *
 */
  LGLM_SYMBOL(frustum)
  LGLM_SYMBOL(ortho_persp)
  LGLM_SYMBOL(ortho_aabb)
  LGLM_SYMBOL(ortho_aabb_p)
  LGLM_SYMBOL(ortho_aabb_pz)
  LGLM_SYMBOL(ortho_default)
  LGLM_SYMBOL(perspective)
  LGLM_SYMBOL(persp_move_far)
  LGLM_SYMBOL(perspective_default)
  LGLM_SYMBOL(perspective_resize)
  LGLM_SYMBOL(lookat)
  LGLM_SYMBOL(look)
  LGLM_SYMBOL(look_anyup)
  LGLM_SYMBOL(persp_decomp)
  LGLM_SYMBOL(persp_fovy)
  LGLM_SYMBOL(persp_aspect)
  LGLM_SYMBOL(persp_sizes)

/*
 * Affine
 *
 */
  LGLM_SYMBOL(translate)
  LGLM_SYMBOL(translate_x)
  LGLM_SYMBOL(translate_y)
  LGLM_SYMBOL(translate_z)

/*
 * NULL-terminated array
 *
 */
  {NULL, NULL},
};

/* from lglm_meta.c */
void _create_meta(lua_State * L);

static
void _set_info(lua_State* L) {
  lua_pushliteral(L, "Copyright 2021-2022 MarcosHCK");
  lua_setfield(L, -2, "_COPYRIGHT");
  lua_pushliteral(L, PACKAGE_NAME "-" PACKAGE_VERSION);
  lua_setfield(L, -2, "_VERSION");
}

int luaopen_lglm(lua_State * L) {
  _create_meta(L);
  new_lib(L, lglm_lib);
  lua_pushvalue(L, -1);
  lua_setglobal(L, "lglm");
  _set_info(L);
return 1;
}
