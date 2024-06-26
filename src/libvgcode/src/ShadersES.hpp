///|/ Copyright (c) Prusa Research 2023 Enrico Turri @enricoturri1966, Pavel Mikuš @Godrak
///|/
///|/ libvgcode is released under the terms of the AGPLv3 or higher
///|/
#ifndef VGCODE_SHADERSES_HPP
#define VGCODE_SHADERSES_HPP

// needed for tech VGCODE_ENABLE_COG_AND_TOOL_MARKERS
#include "../include/Types.hpp"

namespace libvgcode {

// static const char* Segments_Vertex_Shader_ES =
// "#version 300 es\n"
// "precision lowp usampler2D;\n"
// "#define POINTY_CAPS\n"
// "#define FIX_TWISTING\n"
// "const vec3  light_top_dir = vec3(-0.4574957, 0.4574957, 0.7624929);\n"
// "const float light_top_diffuse = 0.6 * 0.8;\n"
// "const float light_top_specular = 0.6 * 0.125;\n"
// "const float light_top_shininess = 20.0;\n"
// "const vec3  light_front_dir = vec3(0.6985074, 0.1397015, 0.6985074);\n"
// "const float light_front_diffuse = 0.6 * 0.3;\n"
// "const float ambient = 0.3;\n"
// "const float emission = 0.15;\n"
// "const vec3 UP = vec3(0, 0, 1);\n"
// "uniform mat4 view_matrix;\n"
// "uniform mat4 projection_matrix;\n"
// "uniform vec3 camera_position;\n"
// "uniform sampler2D position_tex;\n"
// "uniform sampler2D height_width_angle_tex;\n"
// "uniform sampler2D color_tex;\n"
// "uniform usampler2D segment_index_tex;\n"
// "in float vertex_id_float;\n"
// "out vec3 color;\n"
// "vec3 decode_color(float color) {\n"
// "  int c = int(round(color));\n"
// "  int r = (c >> 16) & 0xFF;\n"
// "  int g = (c >> 8) & 0xFF;\n"
// "  int b = (c >> 0) & 0xFF;\n"
// "  float f = 1.0 / 255.0f;\n"
// "  return f * vec3(r, g, b);\n"
// "}\n"
// "float lighting(vec3 eye_position, vec3 eye_normal) {\n"
// "  float top_diffuse = light_top_diffuse * max(dot(eye_normal, light_top_dir), 0.0);\n"
// "  float front_diffuse = light_front_diffuse * max(dot(eye_normal, light_front_dir), 0.0);\n"
// "  float top_specular = light_top_specular * pow(max(dot(-normalize(eye_position), reflect(-light_top_dir, eye_normal)), 0.0), light_top_shininess);\n"
// "  return ambient + top_diffuse + front_diffuse + top_specular + emission;\n"
// "}\n"
// "ivec2 tex_coord(sampler2D sampler, int id) {\n"
// "  ivec2 tex_size = textureSize(sampler, 0);\n"
// "  return (tex_size.y == 1) ? ivec2(id, 0) : ivec2(id % tex_size.x, id / tex_size.x);\n"
// "}\n"
// "ivec2 tex_coord_u(usampler2D sampler, int id) {\n"
// "  ivec2 tex_size = textureSize(sampler, 0);\n"
// "  return (tex_size.y == 1) ? ivec2(id, 0) : ivec2(id % tex_size.x, id / tex_size.x);\n"
// "}\n"
// "void main() {\n"
// "  int vertex_id = int(vertex_id_float);\n"
// "  int id_a = int(texelFetch(segment_index_tex, tex_coord_u(segment_index_tex, gl_InstanceID), 0).r);\n"
// "  int id_b = id_a + 1;\n"
// "  vec3 pos_a = texelFetch(position_tex, tex_coord(position_tex, id_a), 0).xyz;\n"
// "  vec3 pos_b = texelFetch(position_tex, tex_coord(position_tex, id_b), 0).xyz;\n"
// "  vec3 line = pos_b - pos_a;\n"
// "  // directions of the line box in world space\n"
// "  float line_len = length(line);\n"
// "  vec3 line_dir;\n"
// "  if (line_len < 1e-4)\n"
// "    line_dir = vec3(1.0, 0.0, 0.0);\n"
// "  else\n"
// "    line_dir = line / line_len;\n"
// "  vec3 line_right_dir;\n"
// "  if (abs(dot(line_dir, UP)) > 0.9) {\n"
// "    // For vertical lines, the width and height should be same, there is no concept of up and down.\n"
// "    // For simplicity, the code will expand width in the x axis, and height in the y axis\n"
// "    line_right_dir = normalize(cross(vec3(1, 0, 0), line_dir));\n"
// "  }\n"
// "  else\n"
// "    line_right_dir = normalize(cross(line_dir, UP));\n"
// "  vec3 line_up_dir = normalize(cross(line_right_dir, line_dir));\n"
// "  const vec2 horizontal_vertical_view_signs_array[16] = vec2[](\n"
// "    //horizontal view (from right)\n"
// "    vec2(1.0, 0.0),\n"
// "    vec2(0.0, 1.0),\n"
// "    vec2(0.0, 0.0),\n"
// "    vec2(0.0, -1.0),\n"
// "    vec2(0.0, -1.0),\n"
// "    vec2(1.0, 0.0),\n"
// "    vec2(0.0, 1.0),\n"
// "    vec2(0.0, 0.0),\n"
// "    // vertical view (from top)\n"
// "    vec2(0.0, 1.0),\n"
// "    vec2(-1.0, 0.0),\n"
// "    vec2(0.0, 0.0),\n"
// "    vec2(1.0, 0.0),\n"
// "    vec2(1.0, 0.0),\n"
// "    vec2(0.0, 1.0),\n"
// "    vec2(-1.0, 0.0),\n"
// "    vec2(0.0, 0.0)\n"
// "    );\n"
// "  int id = vertex_id < 4 ? id_a : id_b;\n"
// "  vec3 endpoint_pos = vertex_id < 4 ? pos_a : pos_b;\n"
// "  vec3 height_width_angle = texelFetch(height_width_angle_tex, tex_coord(height_width_angle_tex, id), 0).xyz;\n"
// "#ifdef FIX_TWISTING\n"
// "  int closer_id = (dot(camera_position - pos_a, camera_position - pos_a) < dot(camera_position - pos_b, camera_position - pos_b)) ? id_a : id_b;\n"
// "  vec3 closer_pos = (closer_id == id_a) ? pos_a : pos_b;\n"
// "  vec3 camera_view_dir = normalize(closer_pos - camera_position);\n"
// "  vec3 closer_height_width_angle = texelFetch(height_width_angle_tex, tex_coord(height_width_angle_tex, closer_id), 0).xyz;\n"
// "  vec3 diagonal_dir_border = normalize(closer_height_width_angle.x * line_up_dir + closer_height_width_angle.y * line_right_dir);\n"
// "#else\n"
// "  vec3 camera_view_dir = normalize(endpoint_pos - camera_position);\n"
// "  vec3 diagonal_dir_border = normalize(height_width_angle.x * line_up_dir + height_width_angle.y * line_right_dir);\n"
// "#endif\n"
// "  bool is_vertical_view = abs(dot(camera_view_dir, line_up_dir)) / abs(dot(diagonal_dir_border, line_up_dir)) >\n"
// "    abs(dot(camera_view_dir, line_right_dir)) / abs(dot(diagonal_dir_border, line_right_dir));\n"
// "  vec2 signs = horizontal_vertical_view_signs_array[vertex_id + 8 * int(is_vertical_view)];\n"
// "#ifndef POINTY_CAPS\n"
// "  if (vertex_id == 2 || vertex_id == 7) signs = -horizontal_vertical_view_signs_array[(vertex_id - 2) + 8 * int(is_vertical_view)];\n"
// "#endif\n"
// "  float view_right_sign = sign(dot(-camera_view_dir, line_right_dir));\n"
// "  float view_top_sign = sign(dot(-camera_view_dir, line_up_dir));\n"
// "  float half_height = 0.5 * height_width_angle.x;\n"
// "  float half_width = 0.5 * height_width_angle.y;\n"
// "  vec3 horizontal_dir = half_width * line_right_dir;\n"
// "  vec3 vertical_dir = half_height * line_up_dir;\n"
// "  float horizontal_sign = signs.x * view_right_sign;\n"
// "  float vertical_sign = signs.y * view_top_sign;\n"
// "  vec3 pos = endpoint_pos + horizontal_sign * horizontal_dir + vertical_sign * vertical_dir;\n"
// "  if (vertex_id == 2 || vertex_id == 7) {\n"
// "    float line_dir_sign = (vertex_id == 2) ? -1.0 : 1.0;\n"
// "    if (height_width_angle.z == 0.0) {\n"
// "#ifdef POINTY_CAPS\n"
// "      // There I add a cap to lines that do not have a following line\n"
// "      // (or they have one, but perfectly aligned, so the cap is hidden inside the next line).\n"
// "      pos += line_dir_sign * line_dir * half_width;\n"
// "#endif\n"
// "    }\n"
// "    else {\n"
// "      pos += line_dir_sign * line_dir * half_width * sin(abs(height_width_angle.z) * 0.5);\n"
// "      pos += sign(height_width_angle.z) * horizontal_dir * cos(abs(height_width_angle.z) * 0.5);\n"
// "    }\n"
// "  }\n"
// "  vec3 eye_position = (view_matrix * vec4(pos, 1.0)).xyz;\n"
// "  vec3 eye_normal = (view_matrix * vec4(normalize(pos - endpoint_pos), 0.0)).xyz;\n"
// "  vec3 color_base = decode_color(texelFetch(color_tex, tex_coord(color_tex, id), 0).r);\n"
// "  color = color_base * lighting(eye_position, eye_normal);\n"
// "  gl_Position = projection_matrix * vec4(eye_position, 1.0);\n"
// "}\n";

// static const char* Segments_Fragment_Shader_ES =
// "#version 300 es\n"
// "precision highp float;\n"
// "in vec3 color;\n"
// "out vec4 fragment_color;\n"
// "void main() {\n"
// "  fragment_color = vec4(color, 1.0);\n"
// "}\n";

// static const char* Options_Vertex_Shader_ES =
// "#version 300 es\n"
// "precision lowp usampler2D;\n"
// "const vec3  light_top_dir = vec3(-0.4574957, 0.4574957, 0.7624929);\n"
// "const float light_top_diffuse = 0.6 * 0.8;\n"
// "const float light_top_specular = 0.6 * 0.125;\n"
// "const float light_top_shininess = 20.0;\n"
// "const vec3  light_front_dir = vec3(0.6985074, 0.1397015, 0.6985074);\n"
// "const float light_front_diffuse = 0.6 * 0.3;\n"
// "const float ambient = 0.3;\n"
// "const float emission = 0.25;\n"
// "const float scaling_factor = 1.5;\n"
// "uniform mat4 view_matrix;\n"
// "uniform mat4 projection_matrix;\n"
// "uniform sampler2D position_tex;\n"
// "uniform sampler2D height_width_angle_tex;\n"
// "uniform sampler2D color_tex;\n"
// "uniform usampler2D segment_index_tex;\n"
// "in vec3 in_position;\n"
// "in vec3 in_normal;\n"
// "out vec3 color;\n"
// "vec3 decode_color(float color) {\n"
// "  int c = int(round(color));\n"
// "  int r = (c >> 16) & 0xFF;\n"
// "  int g = (c >> 8) & 0xFF;\n"
// "  int b = (c >> 0) & 0xFF;\n"
// "  float f = 1.0 / 255.0f;\n"
// "  return f * vec3(r, g, b);\n"
// "}\n"
// "float lighting(vec3 eye_position, vec3 eye_normal) {\n"
// "  float top_diffuse = light_top_diffuse * max(dot(eye_normal, light_top_dir), 0.0);\n"
// "  float front_diffuse = light_front_diffuse * max(dot(eye_normal, light_front_dir), 0.0);\n"
// "  float top_specular = light_top_specular * pow(max(dot(-normalize(eye_position), reflect(-light_top_dir, eye_normal)), 0.0), light_top_shininess);\n"
// "  return ambient + top_diffuse + front_diffuse + top_specular + emission;\n"
// "}\n"
// "ivec2 tex_coord(sampler2D sampler, int id) {\n"
// "  ivec2 tex_size = textureSize(sampler, 0);\n"
// "  return (tex_size.y == 1) ? ivec2(id, 0) : ivec2(id % tex_size.x, id / tex_size.x);\n"
// "}\n"
// "ivec2 tex_coord_u(usampler2D sampler, int id) {\n"
// "  ivec2 tex_size = textureSize(sampler, 0);\n"
// "  return (tex_size.y == 1) ? ivec2(id, 0) : ivec2(id % tex_size.x, id / tex_size.x);\n"
// "}\n"
// "void main() {\n"
// "  int id = int(texelFetch(segment_index_tex, tex_coord_u(segment_index_tex, gl_InstanceID), 0).r);\n"
// "  vec2 height_width = texelFetch(height_width_angle_tex, tex_coord(height_width_angle_tex, id), 0).xy;\n"
// "  vec3 offset = texelFetch(position_tex, tex_coord(position_tex, id), 0).xyz - vec3(0.0, 0.0, 0.5 * height_width.x);\n"
// "  height_width *= scaling_factor;\n"
// "  mat3 scale_matrix = mat3(\n"
// "    height_width.y, 0.0, 0.0,\n"
// "    0.0, height_width.y, 0.0,\n"
// "    0.0, 0.0, height_width.x);\n"
// "  vec3 eye_position = (view_matrix * vec4(scale_matrix * in_position + offset, 1.0)).xyz;\n"
// "  vec3 eye_normal = (view_matrix * vec4(in_normal, 0.0)).xyz;\n"
// "  vec3 color_base = decode_color(texelFetch(color_tex, tex_coord(color_tex, id), 0).r);\n"
// "  color = color_base * lighting(eye_position, eye_normal);\n"
// "  gl_Position = projection_matrix * vec4(eye_position, 1.0);\n"
// "}\n";

// static const char* Options_Fragment_Shader_ES =
// "#version 300 es\n"
// "precision highp float;\n"
// "in vec3 color;\n"
// "out vec4 fragment_color;\n"
// "void main() {\n"
// "  fragment_color = vec4(color, 1.0);\n"
// "}\n";

#if VGCODE_ENABLE_COG_AND_TOOL_MARKERS
static const char* Cog_Marker_Vertex_Shader_ES =
"#version 300 es\n"
"const vec3  light_top_dir = vec3(-0.4574957, 0.4574957, 0.7624929);\n"
"const float light_top_diffuse = 0.6 * 0.8;\n"
"const float light_top_specular = 0.6 * 0.125;\n"
"const float light_top_shininess = 20.0;\n"
"const vec3  light_front_dir = vec3(0.6985074, 0.1397015, 0.6985074);\n"
"const float light_front_diffuse = 0.6 * 0.3;\n"
"const float ambient = 0.3;\n"
"const float emission = 0.25;\n"
"uniform vec3 world_center_position;\n"
"uniform float scale_factor;\n"
"uniform mat4 view_matrix;\n"
"uniform mat4 projection_matrix;\n"
"in vec3 in_position;\n"
"in vec3 in_normal;\n"
"out float intensity;\n"
"out vec3 world_position;\n"
"float lighting(vec3 eye_position, vec3 eye_normal) {\n"
"  float top_diffuse = light_top_diffuse * max(dot(eye_normal, light_top_dir), 0.0);\n"
"  float front_diffuse = light_front_diffuse * max(dot(eye_normal, light_front_dir), 0.0);\n"
"  float top_specular = light_top_specular * pow(max(dot(-normalize(eye_position), reflect(-light_top_dir, eye_normal)), 0.0), light_top_shininess);\n"
"  return ambient + top_diffuse + front_diffuse + top_specular + emission;\n"
"}\n"
"void main() {\n"
"  world_position = scale_factor * in_position + world_center_position;\n"
"  vec3 eye_position = (view_matrix * vec4(world_position, 1.0)).xyz;\n"
"  vec3 eye_normal = (view_matrix * vec4(in_normal, 0.0)).xyz;\n"
"  intensity = lighting(eye_position, eye_normal);\n"
"  gl_Position = projection_matrix * vec4(eye_position, 1.0);\n"
"}\n";

static const char* Cog_Marker_Fragment_Shader_ES =
"#version 300 es\n"
"precision highp float;\n"
"const vec3 BLACK = vec3(0.05);\n"
"const vec3 WHITE = vec3(0.95);\n"
"uniform vec3 world_center_position;\n"
"in float intensity;\n"
"in vec3 world_position;\n"
"out vec4 out_color;\n"
"void main()\n"
"{\n"
"  vec3 delta = world_position - world_center_position;\n"
"  vec3 color = delta.x * delta.y * delta.z > 0.0 ? BLACK : WHITE;\n"
"  out_color = intensity * vec4(color, 1.0);\n"
"}\n";

static const char* Tool_Marker_Vertex_Shader_ES =
"#version 300 es\n"
"const vec3  light_top_dir = vec3(-0.4574957, 0.4574957, 0.7624929);\n"
"const float light_top_diffuse = 0.6 * 0.8;\n"
"const float light_top_specular = 0.6 * 0.125;\n"
"const float light_top_shininess = 20.0;\n"
"const vec3  light_front_dir = vec3(0.6985074, 0.1397015, 0.6985074);\n"
"const float light_front_diffuse = 0.6 * 0.3;\n"
"const float ambient = 0.3;\n"
"const float emission = 0.25;\n"
"uniform vec3 world_origin;\n"
"uniform float scale_factor;\n"
"uniform mat4 view_matrix;\n"
"uniform mat4 projection_matrix;\n"
"uniform vec4 color_base;\n"
"in vec3 in_position;\n"
"in vec3 in_normal;\n"
"out vec4 color;\n"
"float lighting(vec3 eye_position, vec3 eye_normal) {\n"
"  float top_diffuse = light_top_diffuse * max(dot(eye_normal, light_top_dir), 0.0);\n"
"  float front_diffuse = light_front_diffuse * max(dot(eye_normal, light_front_dir), 0.0);\n"
"  float top_specular = light_top_specular * pow(max(dot(-normalize(eye_position), reflect(-light_top_dir, eye_normal)), 0.0), light_top_shininess);\n"
"  return ambient + top_diffuse + front_diffuse + top_specular + emission;\n"
"}\n"
"void main() {\n"
"  vec3 world_position = scale_factor * in_position + world_origin;\n"
"  vec3 eye_position = (view_matrix * vec4(world_position, 1.0)).xyz;\n"
"  // no need of normal matrix as the scaling is uniform\n"
"  vec3 eye_normal = (view_matrix * vec4(in_normal, 0.0)).xyz;\n"
"  color = vec4(color_base.rgb * lighting(eye_position, eye_normal), color_base.a);\n"
"  gl_Position = projection_matrix * vec4(eye_position, 1.0);\n"
"}\n";

static const char* Tool_Marker_Fragment_Shader_ES =
"#version 300 es\n"
"precision highp float;\n"
"in vec4 color;\n"
"out vec4 fragment_color;\n"
"void main() {\n"
"  fragment_color = color;\n"
"}\n";
#endif // VGCODE_ENABLE_COG_AND_TOOL_MARKERS

} // namespace libvgcode

#endif // VGCODE_SHADERSES_HPP

