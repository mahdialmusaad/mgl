/*
   mgl - A simple yet powerful OpenGL abstractor.
   Copyright (C) 2026 Mahdi Almusaad

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef _MGL_SRC_H_
#define _MGL_SRC_H_

#include <stddef.h>

#define MGL_VERSION 1
#define MGL_API

typedef enum {
	/* Polygon fill mode values. MGL_POLYGON_FILL by default. */
	MGL_POLYGON_POINTS, MGL_POLYGON_LINES, MGL_POLYGON_FILL,
	/* Depth feature values. Disabled by default - recommended enabled value is MGL_DEPTH_LESS. */
	MGL_DEPTH_OFF, MGL_DEPTH_NEVER, MGL_DEPTH_LESS,
	MGL_DEPTH_EQUAL, MGL_DEPTH_LEQUAL, MGL_DEPTH_GREATER,
	MGL_DEPTH_NEQUAL, MGL_DEPTH_GEQUAL, MGL_DEPTH_ALWAYS,
	/* Blend feature values. Disabled by default - recommended enabled value is MGL_BLEND_1MSRCA.
	   SRC = fragment output,
	   DST = current color buffer value,
	   *A  = alpha value of corresponding value,
	   CST = constant color set by user (TODO),
	   1M* = result is (1 - value),
	   SAT = alpha saturate (min of SRCA and 1-DSTA)
	*/
	MGL_BLEND_OFF, MGL_BLEND_ZERO, MGL_BLEND_ONE,
	MGL_BLEND_SRCCOL, MGL_BLEND_1MSRCCOL, MGL_BLEND_DSTCOL, MGL_BLEND_1MDSTCOL,
	MGL_BLEND_SRCA, MGL_BLEND_1MSRCA, MGL_BLEND_DSTA, MGL_BLEND_1MDSTA,
	MGL_BLEND_CST, MGL_BLEND_1MCST, MGL_BLEND_CSTA, MGL_BLEND_1MCSTA, MGL_BLEND_SAT,
	/* Culling and winding order values. BACK, FRONT and FRONTBACK are what faces can be culled (NONE for no culling); CW and CCW determine which vertex winding order specifies the front face.
	   By default, BACK faces are culled and CCW winding order is used (MGL_CULL_BACK_CCW). */
	MGL_CULL_NONE_CCW, MGL_CULL_BACK_CCW, MGL_CULL_FRONT_CCW, MGL_CULL_FRONTBACK_CCW,
	MGL_CULL_NONE_CW, MGL_CULL_BACK_CW, MGL_CULL_FRONT_CW, MGL_CULL_FRONTBACK_CW
} mgl_state;

typedef enum {
	/* Byte/char vectors. */
	MGL_BYTE, MGL_BYTE2, MGL_BYTE3, MGL_BYTE4,
	/* Unsigned byte/char vectors. */
	MGL_UBYTE, MGL_UBYTE2, MGL_UBYTE3, MGL_UBYTE4,
	/* Short vectors. */
	MGL_SHORT, MGL_SHORT2, MGL_SHORT3, MGL_SHORT4,
	/* Unsigned short vectors. */
	MGL_USHORT, MGL_USHORT2, MGL_USHORT3, MGL_USHORT4,
	/* Integer vectors. */
	MGL_INT, MGL_INT2, MGL_INT3, MGL_INT4,
	/* Unsigned integer vectors. */
	MGL_UINT, MGL_UINT2, MGL_UINT3, MGL_UINT4,
	/* Float vectors. */
	MGL_FLOAT, MGL_FLOAT2, MGL_FLOAT3, MGL_FLOAT4,
	/* Double vectors (4.1+) */
	MGL_DOUBLE, MGL_DOUBLE2, MGL_DOUBLE3, MGL_DOUBLE4,
	/* 2xY float matrices. */
	MGL_MAT2X2, MGL_MAT2X3, MGL_MAT2X4,
	/* 3xY float matrices. */
	MGL_MAT3X2, MGL_MAT3X3, MGL_MAT3X4,
	/* 4xY float matrices. */
	MGL_MAT4X2, MGL_MAT4X3, MGL_MAT4X4,
	/* 2xY double matrices. */
	MGL_DMAT2X2, MGL_DMAT2X3, MGL_DMAT2X4,
	/* 3xY double matrices. */
	MGL_DMAT3X2, MGL_DMAT3X3, MGL_DMAT3X4,
	/* 4xY double matrices. */
	MGL_DMAT4X2, MGL_DMAT4X3, MGL_DMAT4X4,

	/* Texture type. */
	MGL_SAMPLER = MGL_INT
} mgl_datatype;

/* TODO: non-uchar color components */
typedef enum {
	MGL_RED,
	MGL_RG,
	MGL_RGB, MGL_BGR,
	MGL_RGBA, MGL_BGRA
} mgl_color;

typedef enum {
	MGL_USAGE_CONSTANT,
	MGL_USAGE_DYNAMIC
} mgl_usage;

typedef enum {
	MGL_STRING_VENDOR,
	MGL_STRING_RENDERER,
	MGL_STRING_VERSION,
	MGL_STRING_GLSLVERSION,
	/* For other extensions, query MGL_STRING_EXTENSION0 + index. */
	MGL_STRING_EXTENSION0

} mgl_stringid;

typedef enum {
	MGL_VALUE_PLACEHOLDER /* TODO */
} mgl_valueid;

typedef enum {
	/* Buffer to store vertex data.
	   Attributes must be set in a data store so the GL knows how to interpret this data.
	   They should typically match with the attributes in the shader program. */
	MGL_BUFFER_VERTICES,
	/* Buffer to store indices to vertex data.
	   Can specify index data as being unsigned bytes, unsigned shorts or unsigned integers. */
	MGL_BUFFER_INDICES_UBYTE, MGL_BUFFER_INDICES_USHORT, MGL_BUFFER_INDICES_UINT,
	/* Uniform buffer data.
	   Must be bound to a program, corresponding to a named uniform in it. */
	MGL_BUFFER_UNIFORM,
	/* Indirect buffer data.
	   Specifies multiple instanced draw commands to be executed at once.
	   4.0+ context required. */
	MGL_BUFFER_DRAW_INDIRECT,
	/* Shader storage buffer data.
	   Similar to uniform buffer data, but typically has a larger capacity.
	   4.3+ context required. */
	MGL_BUFFER_SHADER_STORAGE
} mgl_buffertype;

typedef enum { MGL_ACCESS_READ, MGL_ACCESS_WRITE, MGL_ACCESS_READWRITE, MGL_ACCESS_UNMAP } mgl_access;
typedef enum { MGL_SHADER_VERTEX, MGL_SHADER_FRAGMENT, MGL_SHADER_COMPUTE } mgl_shadertype;
typedef enum { MGL_CLEAR_DEPTH = 0x1, MGL_CLEAR_STENCIL = 0x2, MGL_CLEAR_COLOR = 0x4 } mgl_clearbits;
typedef enum { MGL_DRAW_TRIANGLES, MGL_DRAW_STRIPS, MGL_DRAW_LINES, MGL_DRAW_POINTS } mgl_drawtype;

typedef struct { unsigned int id, type : 30, meta : 2; } mgl_buffer;
typedef struct { unsigned int id, indirect : 29, indexed : 2, instanced : 1; } mgl_datastore;
typedef struct { unsigned int id, meta; } mgl_program;
typedef struct { unsigned int id, meta; } mgl_shader;
typedef struct { unsigned int id, meta; } mgl_texture;

typedef struct {
	unsigned int count;
	unsigned int instance_count;
	unsigned int first;
	unsigned int base_instance;
} mgl_indirect_cmd;

typedef struct {
	unsigned int count;
	unsigned int instance_count;
	unsigned int first_index;
	int base_vertex;
	unsigned int base_instance;
} mgl_indexed_indirect_cmd;

/* On failure to create a buffer, texture, data store, shader or program, the ID (first member) will be this value. */
#define MGL_INVALID_ID 0

/* Error values. */

/* Given enum is invalid. More details in error description. */
#define MGL_ERROR_INVALID_ENUM -1
/* Feature enum is invalid. It must be one of the values in the mgl_state enum. */
#define MGL_ERROR_INVALID_FEATURE -2
/* The data store cannot store multiple buffers of the given type, or you gave a non-vertex buffer when modifying attributes.
   If you are trying to replace an existing index or indirect buffer, this is not supported.
   You should instead create a different data store with that different buffer. */
#define MGL_ERROR_BUFFER_TYPE -3
/* Program or shader creation failed. This likely means a compilation error. */
#define MGL_ERROR_GLSL -4
/* The given buffer cannot be bound to a program - only a uniform or shader storage buffer. */
#define MGL_ERROR_PROGRAM_UNBUFFERABLE -5
/* The given type does not correspond to any uniform type.
   If you gave a short or byte type, these are not supported as GLSL does not
   natively support such type. Try sending a normal integer type instead. */
#define MGL_ERROR_INVALID_UNIFORM -6
/* The data store's current configuration does not support this type of drawing. */
#define MGL_ERROR_NOT_DRAWABLE -7

/* The operation is valid, but likely not what was intended. */
#define MGL_ERROR_MAYBE -254
/* The operation is not supported by the context. */
#define MGL_ERROR_UNSUPPORTED -255

/* Initialize OpenGL library. Returns OpenGL version loaded in the format: major * 10 + minor. */
MGL_API int mglInit(void (*loader(const char *))(void));
/* Set debug handler for errors. */
MGL_API void mglDebug(void (*debug_callback)(int id, const char *name));

/* Returns null-terminated OpenGL string. */
MGL_API const char *mglString(mgl_stringid id);
/* Get OpenGL value. */
MGL_API int mglValue(mgl_valueid id);

/* Clears specific buffers indicated by a bitfield of mgl_clearbits values. */
MGL_API void mglClear(unsigned int clearbits);
/* Clears color buffer with specified color. */
MGL_API void mglClearColor(float r, float g, float b, float a);
/* Clears stencil buffer with specified value. */
MGL_API void mglClearStencil(int stencil);
/* Sets state of OpenGL feature. */
MGL_API void mglFeature(mgl_state state);
/* Set viewport dimensions and position. */
MGL_API void mglViewport(int x, int y, int width, int height);

/* Create a buffer of the specified type. */
MGL_API mgl_buffer mglCreateBuffer(mgl_buffertype type);
/* Set the data of the created buffer.
   Data may be NULL - this will have the effect of reserving the memory.
   Index buffers must be 4 bytes each. TODO: byte and short indices */
MGL_API void mglBufferData(mgl_buffer buffer, mgl_usage usage, const void *data, size_t bytes);
/* Set a specific part of the buffer data. */
MGL_API void mglBufferDataPartial(mgl_buffer buffer, const void *data, size_t offset, size_t bytes);
/* Map or unmap the buffer contents into memory. */
MGL_API void *mglMapBuffer(mgl_buffer buffer, mgl_access access);

/* Create a texture with the specified information. TODO: wrap and mag parameters */
MGL_API mgl_texture mglCreateTexture(int width, int height, const void *pixels, mgl_color format);
/* Bind the given texture to a specific texture unit.
   You can set a sampler2D uniform to this unit using mglSetUniform with the MGL_SAMPLER2D type. */
MGL_API void mglBindTexture(mgl_texture texture, unsigned int binding);

/* Create a buffer data store. You could use this to store mesh and index data. */
MGL_API mgl_datastore mglCreateDataStore(void);
/* Add a buffer to the data store.
   Vertex buffers should be added via the mglDataStoreAttribute function, any attempts here will be ignored.
   This accepts a pointer on purpose and may update the underlying value.*/
MGL_API void mglAddDataStoreBuffer(mgl_datastore *ds, mgl_buffer buffer);
/* Set or add an attribute to the data store, pointing to data of the given vertex buffer.
   If a matrix type is given, it will enable all adjacent indices required to hold each row of the matrix.
   For example, adding an MGL_M34 attribute at index 0 will enable indices 0, 1 and 2 - each of which will store a vec4 (stride/offset set accordingly).
   Stride and offset parameters are interpreted as byte offsets.
   This accepts a pointer on purpose and may update the underlying value. */
MGL_API void mglDataStoreAttribute(mgl_datastore *ds, mgl_buffer buffer, unsigned int index, mgl_datatype type, size_t stride, size_t offset, unsigned int divisor);

/* Create a shader of a specific type by combining multiple code sources. */
MGL_API mgl_shader mglCreateShader(mgl_shadertype type, const char **sources, int nsources);
/* Create a program from 1 or more shaders. */
MGL_API mgl_program mglCreateProgram(const mgl_shader *shaders, int nshaders);
/* Link a created uniform or shader storage buffer to a corresponding named buffer in the program code. */
MGL_API void mglProgramBindBuffer(mgl_program program, mgl_buffer buffer, unsigned int binding_point, const char *name);

/* Locate a uniform by name. */
MGL_API int mglLocateUniform(mgl_program program, const char *name);
/* Set a uniform's value. */
MGL_API void mglSetUniform(mgl_program program, int location, mgl_datatype type, unsigned int count, const void *data);

/* Draw using the given data store, program, primitive type and vertex/index offset and count. */
MGL_API void mglDraw(mgl_datastore ds, mgl_program program, mgl_drawtype primitive, size_t offset, size_t count);
/* Draw using the given data store, program, primitive type and the offset and count values for both the number of instances and number of vertices/indices. */
MGL_API void mglDrawInstanced(mgl_datastore ds, mgl_program program, mgl_drawtype primitive, size_t offset, size_t count, size_t instance_offset, size_t instance_count);
/* Draw using the given data store, program, primitive type and indirect command offset and count. */
MGL_API void mglDrawIndirect(mgl_datastore ds, mgl_program program, mgl_drawtype primitive, size_t cmd_offset, size_t cmd_count);

/* Retrieve specific dimensions of the current frame's pixels as unsigned bytes in the given number of channels (type components). */
MGL_API void mglReadFrame(int x, int y, int width, int height, void *pixels, mgl_color format);

/* Destroy the program object. */
MGL_API void mglDestroyProgram(mgl_program program);
/* Destroy the shader object. */
MGL_API void mglDestroyShader(mgl_shader shader);
/* Destroy the texture object. */
MGL_API void mglDestroyTexture(mgl_texture texture);
/* Destroy the buffer object. */
MGL_API void mglDestroyBuffer(mgl_buffer buffer);
/* Destroy the data store object. */
MGL_API void mglDestroyDataStore(mgl_datastore ds);

#endif
