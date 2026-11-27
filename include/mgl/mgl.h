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

#ifndef MGL_HDR
#define MGL_HDR

#include <stddef.h>

#define MGL_VERSION 1
#define MGL_API extern

typedef enum {
	/* Polygon fill mode values. MGL_POLYGON_FILL by default. */
	MGL_POLYGON_POINTS, MGL_POLYGON_LINES, MGL_POLYGON_FILL,
	/* Depth feature values. Disabled by default - recommended and default enabled value is MGL_DEPTH_LESS. */
	MGL_DEPTH_OFF, MGL_DEPTH_ON, MGL_DEPTH_NEVER, MGL_DEPTH_LESS,
	MGL_DEPTH_EQUAL, MGL_DEPTH_LEQUAL, MGL_DEPTH_GREATER,
	MGL_DEPTH_NEQUAL, MGL_DEPTH_GEQUAL, MGL_DEPTH_ALWAYS,
	/* Depth write toggles. This determines whether the depth buffer is updated by any draws, or only used to discard fragments.
	   Default is MGL_DEPTH_WRITEABLE, i.e. draw calls do update the depth buffer. */
	MGL_DEPTH_READONLY, MGL_DEPTH_WRITEABLE,
	/* Blend feature values. Disabled by default - recommended enabled value is MGL_BLEND_1MSRCA.
	   SRC = fragment output,
	   DST = current color buffer value,
	   *A  = alpha value of corresponding value,
	   CST = constant color set via mglColor with MGL_COLOR_BLEND,
	   1M* = result is (1 - value),
	   SAT = alpha saturate (min of SRCA and 1-DSTA) */
	MGL_BLEND_OFF, MGL_BLEND_ZERO, MGL_BLEND_ONE,
	MGL_BLEND_SRCCOL, MGL_BLEND_1MSRCCOL, MGL_BLEND_DSTCOL, MGL_BLEND_1MDSTCOL,
	MGL_BLEND_SRCA, MGL_BLEND_1MSRCA, MGL_BLEND_DSTA, MGL_BLEND_1MDSTA,
	MGL_BLEND_CST, MGL_BLEND_1MCST, MGL_BLEND_CSTA, MGL_BLEND_1MCSTA, MGL_BLEND_SAT,
	/* Culling and winding order values. BACK, FRONT and FRONTBACK are what faces can be culled (NONE for no culling); CW and CCW determine which vertex winding order specifies the front face.
	   By default, BACK faces are culled and CCW winding order is used (MGL_CULL_BACK_CCW). */
	MGL_CULL_NONE_CCW, MGL_CULL_BACK_CCW, MGL_CULL_FRONT_CCW, MGL_CULL_FRONTBACK_CCW,
	MGL_CULL_NONE_CW, MGL_CULL_BACK_CW, MGL_CULL_FRONT_CW, MGL_CULL_FRONTBACK_CW,
	/* Whether a drawn point's size can be modified in shaders via gl_PointSize. Disabled by default. */
	MGL_POINTSIZE_OFF, MGL_POINTSIZE_ON,
	/* Whether to downgrade usage of unsupported features to one that is close enough.
	   Currently, SSBOs can be downgraded to UBOs if not supported.
	   By default, no downgrading is done and an error is generated for usage if not supported. */
	MGL_DOWNGRADE_SSBO_OFF, MGL_DOWNGRADE_SSBO_ON
} mgl_state;

typedef enum {
	/* Byte/char vectors (in shader attribute = int) */
	MGL_BYTE, MGL_BYTE2, MGL_BYTE3, MGL_BYTE4,
	/* Unsigned byte/char vectors (in shader attribute = uint) */
	MGL_UBYTE, MGL_UBYTE2, MGL_UBYTE3, MGL_UBYTE4,
	/* Short vectors (in shader attribute = int) */
	MGL_SHORT, MGL_SHORT2, MGL_SHORT3, MGL_SHORT4,
	/* Unsigned short vectors (in shader attribute = uint) */
	MGL_USHORT, MGL_USHORT2, MGL_USHORT3, MGL_USHORT4,
	/* Integer vectors */
	MGL_INT, MGL_INT2, MGL_INT3, MGL_INT4,
	/* Unsigned integer vectors */
	MGL_UINT, MGL_UINT2, MGL_UINT3, MGL_UINT4,
	/* Float vectors */
	MGL_FLOAT, MGL_FLOAT2, MGL_FLOAT3, MGL_FLOAT4,
	/* Double vectors */
	MGL_DOUBLE, MGL_DOUBLE2, MGL_DOUBLE3, MGL_DOUBLE4,
	/* 2xY float matrices */
	MGL_MAT2X2, MGL_MAT2X3, MGL_MAT2X4,
	/* 3xY float matrices */
	MGL_MAT3X2, MGL_MAT3X3, MGL_MAT3X4,
	/* 4xY float matrices */
	MGL_MAT4X2, MGL_MAT4X3, MGL_MAT4X4,
	/* 2xY double matrices */
	MGL_DMAT2X2, MGL_DMAT2X3, MGL_DMAT2X4,
	/* 3xY double matrices */
	MGL_DMAT3X2, MGL_DMAT3X3, MGL_DMAT3X4,
	/* 4xY double matrices */
	MGL_DMAT4X2, MGL_DMAT4X3, MGL_DMAT4X4
} mgl_datatype;

typedef enum {
	/* Red/single component image formats of varying underlying types. */
	MGL_RED_U8, MGL_RED_I8, MGL_RED_U16, MGL_RED_I16, MGL_RED_U32, MGL_RED_I32, MGL_RED_F32,
	/* Red-green/two-component image formats of varying underlying types. */
	MGL_RG_U8, MGL_RG_I8, MGL_RG_U16, MGL_RG_I16, MGL_RG_U32, MGL_RG_I32, MGL_RG_F32,
	/* RGB image formats of varying underlying types. */
	MGL_RGB_U8, MGL_RGB_I8, MGL_RGB_U16, MGL_RGB_I16, MGL_RGB_U32, MGL_RGB_I32, MGL_RGB_F32,
	/* BGR image formats of varying underlying types. */
	MGL_BGR_U8, MGL_BGR_I8, MGL_BGR_U16, MGL_BGR_I16, MGL_BGR_U32, MGL_BGR_I32, MGL_BGR_F32,
	/* RGBA image formats of varying underlying types. */
	MGL_RGBA_U8, MGL_RGBA_I8, MGL_RGBA_U16, MGL_RGBA_I16, MGL_RGBA_U32, MGL_RGBA_I32, MGL_RGBA_F32,
	/* BGRA image formats of varying underlying types. */
	MGL_BGRA_U8, MGL_BGRA_I8, MGL_BGRA_U16, MGL_BGRA_I16, MGL_BGRA_U32, MGL_BGRA_I32, MGL_BGRA_F32,
	/* Use previous texture property for mglBufferTexture. */
	MGL_TEXTURE_PROPERTY_AUTO = 0x7FFF
} mgl_color;

typedef enum {
	/* Buffer data is not expected to change. */
	MGL_USAGE_CONSTANT,
	/* Buffer data is expected to change, likely every frame. */
	MGL_USAGE_DYNAMIC
} mgl_usage;

typedef enum {
	/* Manufacturer of GPU. */
	MGL_STRING_VENDOR,
	/* Name of GPU. */
	MGL_STRING_RENDERER,
	/* Version string in the form of MAJOR.MINOR[.RELEASE] [VENDOR_SPECIFIC] where anything enclosed in [...] is optional. */
	MGL_STRING_VERSION,
	/* GLSL version string (same format as normal version string). */
	MGL_STRING_GLSLVERSION,
	/* Name of first supported extension.
	   For other extensions, query MGL_STRING_EXTENSION0 + i. */
	MGL_STRING_EXTENSION0
} mgl_stringid;

typedef enum {
	/* Major version number of context. */
	MGL_VALUE_VERSION_MAJOR,
	/* Minor version number of context. */
	MGL_VALUE_VERSION_MINOR,
	/* Number of extensions supported. */
	MGL_VALUE_NUMEXTS,
	/* Maximum number of vertex shader attribute locations. */
	MGL_VALUE_MAX_ATTRIBS,
	/* Maximum number of uniform locations. */
	MGL_VALUE_MAX_UNIFORMS,
	/* Maximum number of UBOs able to be bound to any shader at once. */
	MGL_VALUE_MAX_UBOS,
	/* Maximum number of SSBOs able to be bound to any shader at once. */
	MGL_VALUE_MAX_SSBOS,
	/* Maximum shader size of a UBO. */
	MGL_VALUE_MAX_UBO_SIZE,
	/* Maximum shader size of an SSBO. */
	MGL_VALUE_MAX_SSBO_SIZE,
	/* Maximum number of texture units able to be bound at once. */
	MGL_VALUE_MAX_TEXUNITS
} mgl_valueid;

typedef enum {
	/* Buffer to store vertex data.
	   Attributes must be set in a data store so the GL knows how to interpret this data.
	   They should typically match with the attributes in the shader program. */
	MGL_BUFFER_VERTICES,
	/* Buffer to store indices to vertex data.
	   Can specify index data as being unsigned bytes, unsigned shorts or unsigned integers. */
	MGL_BUFFER_INDICES_UBYTE, MGL_BUFFER_INDICES_USHORT, MGL_BUFFER_INDICES_UINT,
	/* Uniform buffer data (UBO).
	   Must be bound to a program, corresponding to a named uniform in it. */
	MGL_BUFFER_UNIFORM,
	/* Indirect buffer data.
	   Specifies multiple instanced draw commands to be executed at once. */
	MGL_BUFFER_DRAW_INDIRECT,
	/* Shader storage buffer data (SSBO).
	   Similar to uniform buffer data, but typically has a larger capacity.
	   If not supported, a UBO is created instead. */
	MGL_BUFFER_SHADER_STORAGE
} mgl_buffertype;

typedef enum {
	/* Set the color to clear the framebuffer with, applied using mglClear with the MGL_CLEAR_COLOR bit. */
	MGL_COLOR_CLEAR,
	/* Set blend constant value (CST). */
	MGL_COLOR_BLEND,
	/* Set texture wrap border color for MGL_TEXTURE_WRAP_BORDER. */
	MGL_COLOR_TEXBORDER
} mgl_coltype;

typedef enum {
	/* Texture wrap mode bit where OOB texture coordinates are clamped to the edges. */
	MGL_TEXTURE_WRAP_CLAMP = 0x1,
	/* Texture wrap mode bit where OOB texture coordinates are given a border color,
	   specified via mglColor with MGL_COLOR_TEXBORDER per call. */
	MGL_TEXTURE_WRAP_BORDER = 0x2,
	/* Texture wrap mode bit where OOB texture coordinates will wrap around to repeat the texture.
	   This is the default wrap mode. */
	MGL_TEXTURE_WRAP_REPEAT = 0x4,

	/* Texture magnification and minification filter bit where the final color is
	   the weighted average of nearby pixels. Slower than nearest and produces a blurred output.
	   This is the default scaling mode. */
	MGL_TEXTURE_SCALING_LINEAR = 0x100,
	/* Texture magnification and minification filter bit where the final color is
	   the closest pixel. Faster than linear, but produces a sharp/blocky output. */
	MGL_TEXTURE_SCALING_NEAREST = 0x200,

	/* Whether mipmaps should be created for this image (number of levels depends on size). */
	MGL_TEXTURE_MIPMAPS = 0x1000,
	/* Whether the texture's format and size is immutable.
	   This makes uploading new pixel data faster, but the size and format cannot change. */
	MGL_TEXTURE_IMMUTABLE = 0x2000
} mgl_texfilter;

typedef enum {
	/* Map the buffer for reading only. */
	MGL_ACCESS_READ,
	/* Map the buffer for writing only. */
	MGL_ACCESS_WRITE,
	/* Map the buffer for both reading and writing. */
	MGL_ACCESS_READWRITE,
	/* Unmap the buffer (NULL is returned). */
	MGL_ACCESS_UNMAP
} mgl_access;

typedef enum {
	/* Vertex shader type. */
	MGL_SHADER_VERTEX_FRAGILE = 1, MGL_SHADER_VERTEX,
	/* Fragment shader type. */
	MGL_SHADER_FRAGMENT_FRAGILE, MGL_SHADER_FRAGMENT,
	/* Compute shader type. */
	MGL_SHADER_COMPUTE_FRAGILE, MGL_SHADER_COMPUTE
} mgl_shadertype;

typedef enum {
	/* Bit to clear the depth buffer. */
	MGL_CLEAR_DEPTH = 0x1,
	/* Bit to clear the color set via mglColor with MGL_COLOR_CLEAR to the framebuffer. */
	MGL_CLEAR_COLOR = 0x2
} mgl_clearbits;

typedef enum {
	/* Every three vertices/indices is treated as an individual triangle. */
	MGL_DRAW_TRIANGLES,
	/* After the first triangle, every vertex/index creates a triangle built off the last two vertices/indices. */
	MGL_DRAW_STRIPS,
	/* Connect each vertex/index pair with a line. */
	MGL_DRAW_LINES,
	/* Create a point for each vertex/index. */
	MGL_DRAW_POINTS
} mgl_drawtype;

struct mgl_internal_attribute
{
	unsigned int type, divisor, vbo, components : 3, func : 2, normalized : 1, pad : 26;
	int astride, offset, stride;
};

typedef struct { unsigned int id, indirect : 31, instanced : 1, ebo : 30, indexsz : 2; struct mgl_internal_attribute attribs[32]; } mgl_datastore;
typedef struct { unsigned int id, uniforms : 30, vertexid : 1, compute : 1; } mgl_program;
typedef struct { unsigned int id, type : 30, vertexid : 1, fragile : 1; } mgl_shader;
typedef struct { unsigned int id, fmt : 16, filters : 16; } mgl_texture;
typedef struct { unsigned int id, type : 29, indexsz : 3; } mgl_buffer;
typedef struct { unsigned int id, tex, rbuf; } mgl_framebuffer;
typedef struct { unsigned int location, type; } mgl_uniform;

/* An indirect draw command typically stored as a list in a
   MGL_BUFFER_DRAW_INDIRECT buffer for drawing via mglDrawIndirect.
   It stores the arguments for a call to mglDrawInstanced. */
typedef struct {
	unsigned int count;
	unsigned int instance_count;
	unsigned int offset;
	unsigned int instance_offset;
} mgl_indirect_cmd;

/* An indexed indirect draw command typically stored as a list in a
   MGL_BUFFER_DRAW_INDIRECT buffer for drawing via mglDrawIndirect.
   It stores the arguments for a call to mglDrawInstanced. */
typedef struct {
	unsigned int count;
	unsigned int instance_count;
	unsigned int offset;
	int element_constant;
	unsigned int instance_offset;
} mgl_indexed_indirect_cmd;

/* On failure to create an object, the ID/location (first member) will be this value. */
#define MGL_INVALID_ID 0

/* Error values. */

/* Given enum is invalid. More details in error description. */
#define MGL_ERROR_INVALID_ENUM -1
/* Attempted to modify property of immutable texture. */
#define MGL_ERROR_IMMUTABLE -2
/* The data store cannot store multiple buffers of the given type, or you gave a non-vertex buffer when modifying attributes.
   If you are trying to replace an existing index or indirect buffer, this is not supported.
   You can instead create a different data store with that buffer, or delete the existing ones. */
#define MGL_ERROR_BUFFER_TYPE -3
/* Program or shader creation failed. This likely means a compilation error or an invalid shader combination. */
#define MGL_ERROR_GLSL -4
/* The given buffer cannot be bound to a program - only a uniform or shader storage buffer. */
#define MGL_ERROR_PROGRAM_UNBUFFERABLE -5
/* The given uniform name does not match any uniform variable defined in the given shader.
   Make sure the name matches byte-for-byte. If you are sure the name is correct, this could
   be due to the uniform being unused or due to having too many uniforms. */
#define MGL_ERROR_INVALID_UNIFORM -6
/* The data store's current configuration does not support this type of drawing. */
#define MGL_ERROR_NOT_DRAWABLE -7
/* The given program does not have a compute shader. */
#define MGL_ERROR_NOT_COMPUTABLE -8
/* Given value(s) is/are invalid. More details in error description.
   This refers to numerical values, not enums or objects.
   Invalid enums instead produce MGL_ERROR_INVALID_ENUM, and invalid objects have no effect.
   Furthermore, if you request to do an action 0 times or with 0 bytes, it also has no effect.
   This means a numerical value simply does not make sense. */
#define MGL_ERROR_INVALID_NUMBER -9

/* The operation is not supported by the context. */
#define MGL_ERROR_UNSUPPORTED -255

/* Initialize library internals and GL function pointers,
   given the version used to determine functions to load, in same format as return value,
   or -1 to load the maximum version the context supports.
   Returns API version loaded in the format: major * 10 + minor (i.e. version 3.3 = 33). */
MGL_API int mglInit(void (*loader(const char *))(void), int apiver);
/* Set the debug handler for errors. */
MGL_API void mglDebug(void (*debug_callback)(int id, const char *name));

/* Returns a null-terminated OpenGL string. */
MGL_API const char *mglString(unsigned int id);
/* Retrieve a value relating to the context's capabilities. */
MGL_API int mglValue(mgl_valueid id);
/* Sets a color value for a specific purpose. */
MGL_API void mglColor(mgl_coltype coltype, float r, float g, float b, float a);
/* Get the current GL timestamp in nanoseconds as a split 64-bit number.
   Either pointer may be NULL. */
MGL_API void mglTimestamp(unsigned int *lower_bits, unsigned int *higher_bits);

/* Clears specific buffers indicated by a bitfield of mgl_clearbits values. */
MGL_API void mglClear(unsigned int clearbits);
/* Sets the state of an OpenGL feature. */
MGL_API void mglFeature(mgl_state state);
/* Set viewport dimensions and position. */
MGL_API void mglViewport(int x, int y, int width, int height);

/* Create a buffer of the specified type. */
MGL_API mgl_buffer mglCreateBuffer(mgl_buffertype type);
/* Set the data of the created buffer.
   Data may be NULL - this will have the effect of reserving the memory.
   Index buffers must be 4 bytes each. */
MGL_API void mglBufferData(const mgl_buffer *buffer, mgl_usage usage, const void *data, size_t bytes);
/* Set a specific part of the buffer data. */
MGL_API void mglBufferDataPartial(const mgl_buffer *buffer, const void *data, size_t buffer_offset, size_t bytes);
/* Get a specific part of the buffer data. */
MGL_API void mglGetBufferData(const mgl_buffer *buffer, size_t buffer_offset, size_t bytes, void *results);
/* Copy data between buffers.
   source and destination may be the same buffer, as long as the copy and paste ranges do not overlap. */
MGL_API void mglCopyBufferData(const mgl_buffer *source, size_t source_offset, const mgl_buffer *destination, size_t destination_offset, size_t copybytes);
/* Map or unmap the buffer contents into memory. */
MGL_API void *mglMapBuffer(const mgl_buffer *buffer, mgl_access access);

/* Create a texture with the specified information.
   Internally, the texture will be stored as an equivalent float type (i.e. MGL_*_F32),
   so only the 'sampler2D' uniform type can be used in shaders to access them.
   The 'filters' parameter is a bitfield made up MGL_TEXTURE_* enum bits.
   Pixels may be NULL - this will have the effect of reserving the memory.
   Defaults are used for each 'category' of texture filter bits. */
MGL_API mgl_texture mglCreateTexture(const void *pixels, mgl_color format, int width, int height, unsigned int filters);
/* Buffer pixel data to the given texture.
   If the texture is immutable, the format and dimensions must be the same.
   You can specify MGL_TEXTURE_PROPERTY_AUTO for format, width and height to use the current value. */
MGL_API void mglBufferTexture(mgl_texture *texture, const void *pixels, mgl_color format, int width, int height);
/* Bind the given texture to a specific texture unit.
   To use this in shader code, use mglSetUniform with a sampler variable with a signed integer set to the same texture binding/unit. */
MGL_API void mglBindTexture(const mgl_texture *texture, unsigned int binding);

/* Create a framebuffer of the specified size.
   The framebuffer has a depth, stencil and color attachment. */
MGL_API mgl_framebuffer mglCreateFramebuffer(int width, int height);
/* Bind the given framebuffer. This will become the target of mglDraw* and mglReadFrame operations.
   Pass in a NULL pointer to use the default framebuffer, i.e. operating on the screen as a whole. */
MGL_API void mglUseFramebuffer(const mgl_framebuffer *fb);
/* Get the texture of the given framebuffer. You can then use it in texture-related options, such as mglBindTexture.
   You should not delete this texture for obvious reasons. */
MGL_API mgl_texture mglFramebufferTexture(const mgl_framebuffer *fb);
/* Resize the framebuffer. */
MGL_API void mglResizeFramebuffer(mgl_framebuffer *fb, int width, int height);

/* Create a buffer data store. You could use this to store mesh and index data. */
MGL_API mgl_datastore mglCreateDataStore(void);
/* Add a buffer to the data store.
   Vertex buffers should be added via the mglDataStoreAttribute function, any attempts here will be ignored. */
MGL_API void mglAddDataStoreBuffer(mgl_datastore *ds, const mgl_buffer *buffer);
/* Set or add an attribute to the data store, pointing to data of the given vertex buffer.
   If a matrix type is given, it will enable all adjacent indices required to hold each row of the matrix.
   For example, adding an MGL_MAT3X4 attribute at index 0 will enable indices 0, 1 and 2 - each of which will store a vec4 (stride/offset set accordingly).
   Stride and offset parameters are interpreted as byte offsets. */
MGL_API void mglDataStoreAttribute(mgl_datastore *ds, const mgl_buffer *buffer, int index, mgl_datatype type, size_t stride, size_t offset, unsigned int divisor);

/* Create a shader of a specific type by combining multiple code sources. */
MGL_API mgl_shader mglCreateShader(mgl_shadertype type, const char **sources, int nsources);
/* Create a program from 1 or more shaders. Any 'fragile' shader is destroyed after this. */
MGL_API mgl_program mglCreateProgram(const mgl_shader *shaders, int nshaders);
/* Link a created uniform or shader storage buffer to a corresponding named buffer in the program code. */
MGL_API void mglProgramBindBuffer(const mgl_program *program, const mgl_buffer *buffer, unsigned int binding_point, const char *name);

/* Locate an attribute by name. -1 if not found. */
MGL_API int mglLocateAttribute(const mgl_program *program, const char *name);
/* Locate a uniform by name. */
MGL_API mgl_uniform mglLocateUniform(const mgl_program *program, const char *name);
/* Set a uniform's value. If it is not an array, set arraycount to 1. */
MGL_API void mglSetUniform(const mgl_program *program, const mgl_uniform *uniform, unsigned int arraycount, const void *data);

/* Draw using the given data store, program, primitive type and vertex/index offset and count. */
MGL_API void mglDraw(const mgl_datastore *ds, const mgl_program *program, mgl_drawtype primitive, unsigned int offset, unsigned int count);
/* Draw using the given data store, program, primitive type and the offset and count values for both the number of instances and number of vertices/indices.
   element_constant is added to each index if an index buffer has been added to the data store. */
MGL_API void mglDrawInstanced(const mgl_datastore *ds, const mgl_program *program, mgl_drawtype primitive, unsigned int offset, unsigned int count, unsigned int instance_offset, unsigned int instance_count, int element_constant);
/* Draw using the given data store, program, primitive type and indirect command offset and count. */
MGL_API void mglDrawIndirect(const mgl_datastore *ds, const mgl_program *program, mgl_drawtype primitive, unsigned int cmd_offset, unsigned int cmd_count);

/* Draw part of the 'read' framebuffer to part of the 'draw' framebuffer, where either framebuffer may be NULL to specify the default framebuffer.
   The 'clearbits' is the same as that of mglClear - i.e. a bitfield of mgl_clearbits.
   The 'scalemode' specifies the interpolation method when stretching an image (MGL_TEXTURE_SCALING_LINEAR or MGL_TEXTURE_SCALING_NEAREST). */
MGL_API void mglDrawFramebuffer(
	const mgl_framebuffer *read, int rx, int ry, int rwidth, int rheight,
	const mgl_framebuffer *draw, int dx, int dy, int dwidth, int dheight,
	unsigned int clearbits, mgl_texfilter scalemode
);

/* Retrieve specific dimensions of the given framebuffer's pixels in a given format.
   The framebuffer may be NULL to specify the default framebuffer. */
MGL_API void mglReadFrame(const mgl_framebuffer *fb, void *pixels, mgl_color format, int x, int y, int width, int height);

/* Execute a compute shader program with the given work group dimensions. */
MGL_API void mglCompute(const mgl_program *program, unsigned int num_x_group, unsigned int num_y_group, unsigned int num_z_group);

/* Destroy the program object. */
MGL_API void mglDestroyProgram(mgl_program *program);
/* Destroy the shader object. */
MGL_API void mglDestroyShader(mgl_shader *shader);
/* Destroy the texture object. */
MGL_API void mglDestroyTexture(mgl_texture *texture);
/* Destroy the data store object.
   Any buffers used by this data store are not affected. */
MGL_API void mglDestroyDataStore(mgl_datastore *ds);
/* Destroy the buffer object and unbind from the data store.
   This wil disable any attributes that were enabled using the given buffer. */
MGL_API void mglDestroyBuffer(mgl_buffer *buffer, mgl_datastore *ds);
/* Destroy the framebuffer object and unbind it if in use.
   This will also destroy the underlying texture. */
MGL_API void mglDestroyFramebuffer(mgl_framebuffer *fb);

#endif
