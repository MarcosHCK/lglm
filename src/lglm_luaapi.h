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
#ifndef LGLM_LUAAPI_H_INCLUDED
#define LGLM_LUAAPI_H_INCLUDED
#include <macros.h>
#include <string.h>
#include <stdlib.h>
#include <lglm.h>

/* Configuration */
#include <../config.h>

/*
 * This function here is really weird,
 * because to calculate a type floats
 * count (how many number contains),
 * you just need to add two to its 'mtype'
 * ex: LUA_TVEC3 (1) plus two makes tree,
 * and in case of a matrix, subtract matrix
 * first type (LUA_TMAT2) and multiply it by
 * itself.
 * But using a switch is faster.
 *
 */
static inline
int _floats_count(int mtype) {
  switch(mtype) {
  case LUA_TVEC2:
    return 2;
    break;
  case LUA_TVEC3:
    return 3;
    break;
  case LUA_TVEC4:
    return 4;
    break;
  case LUA_TMAT2:
    return 4;
    break;
  case LUA_TMAT3:
    return 9;
    break;
  case LUA_TMAT4:
    return 16;
    break;
  case LUA_TBBOX:
    return 6;
    break;
  case LUA_TQUAT:
    return 4;
    break;
  }
return 0;
}

#if __cplusplus
extern "C" {
#endif // __cplusplus

/*
 * Internal API
 * at lglm.c
 *
 */
int _typeerror(lua_State *L, int arg, const char *tname);
int _checklglmobject(lua_State* L, int idx, int mtype, int throw_error);
int _checklglmobject_ex(lua_State* L, int idx, int mtype, int throw_error);
lglm_union_t* _tolglmobject(lua_State* L, int idx, int mtype);

/*
 * Constructors
 * at constructors.c
 *
 */
int _vec2(lua_State* L);
int _vec3(lua_State* L);
int _vec4(lua_State* L);
int _mat2(lua_State* L);
int _mat3(lua_State* L);
int _mat4(lua_State* L);

/*
 * Operators
 * at operators.c
 *
 */
int _add(lua_State* L);
int _adds(lua_State* L);
int _subs(lua_State* L);
int _sub(lua_State* L);
int _dot(lua_State* L);
int _cross(lua_State* L);
int _div(lua_State* L);
int _divs(lua_State* L);

/*
 * Operations
 * at operations.c
 *
 */
int _norm2(lua_State* L);
int _norm(lua_State* L);
int _norm_one(lua_State* L);
int _scale(lua_State* L);
int _scale_as(lua_State* L);
int _negate(lua_State* L);
int _normalize(lua_State* L);
int _angle(lua_State* L);
int _rotate(lua_State* L);
int _proj(lua_State* L);
int _center(lua_State* L);
int _distance2(lua_State* L);
int _distance(lua_State* L);
int _max(lua_State* L);
int _min(lua_State* L);
int _maxv(lua_State* L);
int _minv(lua_State* L);
int _ortho(lua_State* L);
int _clamp(lua_State* L);
int _lerp(lua_State* L);
int _lerpc(lua_State* L);
int _mix(lua_State* L);
int _mixc(lua_State* L);
int _step_uni(lua_State* L);
int _step(lua_State* L);
int _smoothstep_uni(lua_State* L);
int _smoothstep(lua_State* L);
int _smoothinterp(lua_State* L);
int _smoothinterpc(lua_State* L);
int _invert(lua_State* L);

/*
 * Bounding box
 * at bbox.c
 *
 */
int _aabb(lua_State* L);
int _aabb_transform(lua_State* L);
int _aabb_merge(lua_State* L);
int _aabb_crop(lua_State* L);
int _aabb_crop_until(lua_State* L);
int _aabb_frustum(lua_State* L);
int _aabb_invalidate(lua_State* L);
int _aabb_isvalid(lua_State* L);
int _aabb_size(lua_State* L);
int _aabb_radius(lua_State* L);
int _aabb_center(lua_State* L);
int _aabb_intercepts(lua_State* L);
int _aabb_sphere(lua_State* L);
int _aabb_point(lua_State* L);
int _aabb_contains(lua_State* L);

/*
 * Quaternion
 * at versor.c
 *
 */
int _quat(lua_State* L);

/*
 * Sphere
 * at sphere.c
 *
 */
int _sphere(lua_State* L);

/*
 * Camera transforms
 * at camera.c
 *
 */
int _frustum(lua_State* L);
int _ortho_persp(lua_State* L);
int _ortho_aabb(lua_State* L);
int _ortho_aabb_p(lua_State* L);
int _ortho_aabb_pz(lua_State* L);
int _ortho_default(lua_State* L);
int _perspective(lua_State* L);
int _persp_move_far(lua_State* L);
int _perspective_default(lua_State* L);
int _perspective_resize(lua_State* L);
int _lookat(lua_State* L);
int _look(lua_State* L);
int _look_anyup(lua_State* L);
int _persp_decomp(lua_State* L);
int _persp_fovy(lua_State* L);
int _persp_aspect(lua_State* L);
int _persp_sizes(lua_State* L);

/*
 * Affine operations
 * at affine.c
 *
 */
int _translate(lua_State* L);
int _translate_x(lua_State* L);
int _translate_y(lua_State* L);
int _translate_z(lua_State* L);

#if __cplusplus
}
#endif // __cplusplus

#endif // LGLM_LUAAPI_H_INCLUDED
