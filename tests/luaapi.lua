--[[
-- Copyright 2021-2022 MarcosHCK
-- This file is part of lglm.
--
-- lglm is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- lglm is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with lglm.  If not, see <http://www.gnu.org/licenses/>.
--
]]
package.cpath = '.libs/lib?.so;'
local glm = require('lglm');

local function test_feature(msg, func)
  io.stdout:write(msg .. '\r\n');
  func();
  collectgarbage();
  io.stdout:write('\r\n');
end

local function test_constuctors()
  local vec2 = glm.vec2(16, 15);
  local vec3 = glm.vec3(16, 15, 14);
  local vec4 = glm.vec4(16, 15, 14, 13);
  local mat2 = glm.mat2(16, 15, 14, 13);
  local mat3 = glm.mat3(16, 15, 14, 13, 12, 11, 10,  9,  8);
  local mat4 = glm.mat4(16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1);

  print(glm.unpack(vec2));
  print(glm.unpack(vec3));
  print(glm.unpack(vec4));
  print(glm.unpack(mat2));
  print(glm.unpack(mat3));
  print(glm.unpack(mat4));
end
test_feature('Testing constructors ...', test_constuctors);

local function test_table_constuctors()
  local vec4 = glm.vec4 {16, 15, 14, 13}
  local mat4 = glm.mat4 {
    glm.vec4 {16, 15, 14, 13},
    glm.vec4 {12, 11, 10,  9},
    glm.vec3 { 8,  7,  6}, 5,
    glm.vec4 { 4,  3,  2,  1},
  };

  print(glm.unpack(vec4));
  print(glm.unpack(mat4));
end
test_feature('Testing table constructors ...', test_table_constuctors);

local function test_ex_constuctors()
  local box = glm.aabb(1, 1, 1, 5, 5, 5);

  print(glm.unpack(box));
end
test_feature('Testing extended constructors ...', test_ex_constuctors);

local function test_zero_contructor()
  local vec3 = glm.vec3();
  print(glm.unpack(vec3));
end
test_feature('Testing zero construction ...', test_zero_contructor);

local function test_matrix_from_vectors()
  local vec2 = glm.vec2(16, 15);
  local vec4 = glm.vec4(16, 15, 14, 13);

  local mat4 = glm.mat4(vec4, vec4, vec4, vec4);
  print(glm.unpack(mat4));

  local mat4 = glm.mat4(vec4, vec4, vec4, 1, 2, 3, 4);
  print(glm.unpack(mat4));

  local mat2 = glm.mat2(vec2, vec2);
  print(glm.unpack(mat2));

  local mat2 = glm.mat2(vec2, 1, 2);
  print(glm.unpack(mat2));

  local mat2 = glm.mat2(1, vec2, 2);
  print(glm.unpack(mat2));
end
test_feature('Testing matrix from vectors construction ...', test_matrix_from_vectors);

local function test_misplaced_args()
  local function test(func, ...)
    local success, reason = pcall(func, ...);
    local function seri_(...)
      local ret = '';
      for _, value in ipairs({...}) do
        ret = ret .. tostring(value) .. ', ';
      end
    return ret:gsub(',%s$', '');
    end
    io.stdout:write(string.format('test: %s(%s) ... ', tostring(func), seri_(...)));
    if(success) then
      io.stdout:write('failed');
    else
      io.stdout:write('success (' .. reason .. ')');
    end
    io.stdout:write('\r\n');
  end

  test(glm.invert, glm.vec3());
  test(glm.cross, glm.vec3(), glm.vec2());
  test(glm.cross, glm.vec3(), glm.aabb(1, 1, 1, 5, 5, 5));
end
test_feature('Testing misplaced args ...', test_misplaced_args);

local function test_vector_index_getter()
  local vec2 = glm.vec2(16, 15);
  local vec3 = glm.vec3(16, 15, 14);
  local vec4 = glm.vec4(16, 15, 14, 13);
  local mat4 = glm.mat4(vec4, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1);

  print(vec4[4], vec4[3], vec4[2], vec4[1]);
  print(vec4.w, vec4.z, vec4.y, vec4.x);
  print(vec3.w, vec3.z, vec3.y, vec3.x);
  print(vec2.w, vec2.z, vec2.y, vec2.x);
  print(mat4[1]);
  print(mat4[1][1]);
end
test_feature('Testing vector indexing (getter) ...', test_vector_index_getter);

local function test_vector_index_setter()
  local vec2 = glm.vec2(16, 15);
  local vec4 = glm.vec4(16, 15, 14, 13);

  vec2[2] = 1;
  vec4[4] = 2;

  print(vec2.x, vec2.y, vec2.z, vec2.w);
  print(vec4.x, vec4.y, vec4.z, vec4.w);

  vec2.xzy = 8, 7;
  vec4.wx = 8, 7;

  print(vec2.x, vec2.y, vec2.z, vec2.w);
  print(vec4.x, vec4.y, vec4.z, vec4.w);
end
test_feature('Testing vector indexing (setter) ...', test_vector_index_setter);

local function test_vector_methods()
  local mat4 = glm.mat4();
  print(glm.unpack(mat4));
  mat4:identity()
  print(glm.unpack(mat4));

  local vec2 = glm.vec2();
  print(glm.unpack(vec2));
  vec2:one()
  print(glm.unpack(vec2));

  print(glm.unpack(glm.mat4():identity()));
  print(glm.unpack(glm.mat4():identity():invert()));
  print(glm.unpack(glm.invert(glm.mat4():identity())));
end
test_feature('Testing vector methods ...', test_vector_methods);

local function test_operators()
  local vec2 = glm.vec2(16, 15);
  local mat2 = glm.mat2(16, 15, 14, 13);

  print(glm.unpack(glm.add(vec2, vec2)));
  print(glm.unpack(glm.sub(vec2, vec2))); 
  print(glm.unpack(glm.dot(mat2, vec2))); 
  print(glm.unpack(glm.div(vec2, vec2)));
  print(glm.cross(vec2, vec2)); 
end
test_feature('Testing operators over vectors and matrices ...', test_operators);

local function test_operations()
  local mat4 = glm.mat4(16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1);

  print(glm.unpack(glm.translate(mat4, glm.vec3(1, 2, 3))));
end
test_feature('Testing operations over vectors and matrices ...', test_operations);
