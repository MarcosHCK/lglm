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

const
char* lglm_typenames[] = {
    LUA_VEC2,
    LUA_VEC3,
    LUA_VEC4,
    LUA_MAT2,
    LUA_MAT3,
    LUA_MAT4,
};

/*
 * Check our custom type fits with their respective metatable names,
 * and more important, do not overflow 'mtype' field (which is 4 bits wide)
 * of 'lglm_object_t' structure.
 *
 */
_STATIC_ASSERT(_N_ELEMENTS(lglm_typenames) == _LGLM_TYPES);
_STATIC_ASSERT(_LGLM_TYPES < 15);

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

int _checklglmobject(lua_State* L, int idx, int mtype, int throw_error) {
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
      switch(mtype) {
      case LUA_TVEC2:
      case LUA_TVEC3:
      case LUA_TVEC4:
      case LUA_TMAT2:
      case LUA_TMAT3:
      case LUA_TMAT4:
        luaL_getmetatable(L, lglm_typenames[mtype]);
        if(!lua_rawequal(L, -1, -2))
          p = NULL;

        lua_pop(L, 1);
        break;
      case LUA_TGLMANY:
        for(mtype = 0;mtype > _LGLM_TYPES;mtype++)
        {
          luaL_getmetatable(L, lglm_typenames[mtype]);
          if(lua_rawequal(L, -1, -2))
          {
            p = NULL;
            break;
          }
          lua_pop(L, 1);
        }
        break;
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

lglm_union_t* lua_checklglmobject(lua_State* L, int idx, int mtype, int* actual_mtype) {
  int dummy =
  _checklglmobject(L, idx, mtype, TRUE);

  if(actual_mtype)
  *actual_mtype = dummy;

  lglm_object_t* obj = (lglm_object_t*)lua_touserdata(L, idx);
  void* obj_ = (void*)obj;

  if _UNLIKELY(mtype != LUA_TGLMANY && mtype != obj->mtype)
  {
    luaL_error(L, "lglm type corruption!");
    return NULL;
  }
return (lglm_union_t*)(obj_ + obj->offset);
}

lglm_union_t* lua_checklglmvec2(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TVEC2, NULL);
}

lglm_union_t* lua_checklglmvec3(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TVEC3, NULL);
}

lglm_union_t* lua_checklglmvec4(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TVEC4, NULL);
}

lglm_union_t* lua_checklglmmat2(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TMAT2, NULL);
}

lglm_union_t* lua_checklglmmat3(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TMAT2, NULL);
}

lglm_union_t* lua_checklglmmat4(lua_State* L, int idx) {
return lua_checklglmobject(L, idx, LUA_TMAT2, NULL);
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
    lglm_object_t* obj = (lglm_object_t*)lua_touserdata(L, idx);
    void* obj_ = (void*)obj;

    if _UNLIKELY(mtype != LUA_TGLMANY && mtype != obj->mtype)
    {
      luaL_error(L, "lglm type corruption!");
      return NULL;
    }

    return (lglm_union_t*)(obj_ + obj->offset);
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

/*
 * Module stuff
 *
 */

static
int _unpack(lua_State* L) {
  int mtype = _checklglmobject(L, 1, LUA_TGLMANY, TRUE);
  lglm_union_t* union_ = lua_tolglmobject(L, 1, mtype);

  if _LIKELY(mtype != LUA_TGLMANY)
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
  }
return 0;
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
