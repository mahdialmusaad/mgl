#include "mgl/mgl.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int wwidth = 1280, wheight = 720;
static void err(int id, const char *desc) { printf("ERROR %d: %s\n", id, desc); }
static void framebuffer(GLFWwindow *window, int width, int height) { (void)window; mglViewport(0, 0, wwidth = width, wheight = height); }

int main(int argc, char **argv)
{
	if (argc == 2 && argv[1][0] == 'x') glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
	glfwSetErrorCallback(err);
	if (!glfwInit()) { printf("NO INIT\n"); return 0; }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	GLFWwindow *window = glfwCreateWindow(wwidth, wheight, "mgl", NULL, NULL);
	if (!window) { printf("NO WINDOW\n"); return 0; }
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer);
	glfwSwapInterval(1);
	mglInit(glfwGetProcAddress);

	printf("%s | %s\n", mglString(MGL_STRING_VERSION), mglString(MGL_STRING_RENDERER));

	mglDebug(err);

	mglFeature(MGL_BLEND_1MSRCA);
	mglFeature(MGL_CULL_BACK_CW);
	mglFeature(MGL_DEPTH_ALWAYS);

	const char *vertex_source = "#version 330\n"
	"layout(location = 0) in vec2 position;"
	"layout(location = 1) in vec2 instanced_position;"
	"void main(){ gl_Position = vec4(position + instanced_position, 0.0, 1.0); }";
	const char *fragment_source = "#version 330\n"
	"out vec4 frag;"
	"uniform vec3 col;"
	"void main(){ frag = vec4(col, 1.0); }";

	const char *vertex_source2 = "#version 330\n"
	"layout(location = 0) in vec2 position;"
	"layout(location = 1) in vec2 tcoords;"
	"out vec2 coords;"
	"void main(){ gl_Position = vec4(position * 2.0 + vec2(-0.1f, 0.1f), 0.0, 1.0); coords = tcoords; }";
	const char *fragment_source2 = "#version 330\n"
	"out vec4 frag;"
	"uniform sampler2D tex;"
	"in vec2 coords;"
	"void main(){ frag = texture(tex, coords); }";

	const char *vertex_source3 = "#version 330\n"
	"layout(location = 0) in vec2 position;"
	"layout(std140) uniform named_ubo { vec2 ubo_position; };"
	"void main(){ gl_Position = vec4(position + ubo_position.xy, 0.0, 1.0); }";
	const char *fragment_source3 = "#version 330\n"
	"out vec4 frag;"
	"uniform sampler2D tex;"
	"in vec2 coords;"
	"void main(){ frag = texture(tex, coords); }";

	mgl_shader vertex = mglCreateShader(MGL_SHADER_VERTEX, &vertex_source, 1);
	mgl_shader fragment = mglCreateShader(MGL_SHADER_FRAGMENT, &fragment_source, 1);
	mgl_program program = mglCreateProgram((mgl_shader[]){vertex, fragment}, 2);

	mglDestroyShader(vertex);
	mglDestroyShader(fragment);

	mgl_shader vertex2 = mglCreateShader(MGL_SHADER_VERTEX, &vertex_source2, 1);
	mgl_shader fragment2 = mglCreateShader(MGL_SHADER_FRAGMENT, &fragment_source2, 1);
	mgl_program program2 = mglCreateProgram((mgl_shader[]){vertex2, fragment2}, 2);

	mglDestroyShader(vertex2);
	mglDestroyShader(fragment2);

	mgl_shader vertex3 = mglCreateShader(MGL_SHADER_VERTEX, &vertex_source3, 1);
	mgl_shader fragment3 = mglCreateShader(MGL_SHADER_FRAGMENT, &fragment_source3, 1);
	mgl_program program3 = mglCreateProgram((mgl_shader[]){vertex2, fragment3}, 2);

	mglDestroyShader(vertex3);
	mglDestroyShader(fragment3);

	const float verts[] = {
		 0.0f,  0.0f, // TL
		 0.1f,  0.0f, // TR
		 0.0f, -0.1f, // BL
		 0.1f, -0.1f, // BR
	};
	const float instanced[] = {
		-1.0f,   1.0f,
		 0.9f,   1.0f,
		-1.0f,  -0.9f,
		 0.9f,  -0.9f,

		-0.05f,  1.0f,
		-1.0f,   0.05f,
		 0.9f,   0.05f,
		-0.05f, -0.9f
	};
	const float instanced2[] = {
		-0.5f,   0.5f,
		 0.4f,   0.5f,
		-0.5f,  -0.4f,
		 0.4f,  -0.4f,
	};
	const mgl_indirect_cmd indirect[] = {
		{ 4, 4, 0, 0 },
		{ 4, 4, 0, 4 }
	};

	const float verts2[] = {
		 0.0f,  0.0f,  0.0f, 0.0f, // TL
		 0.1f,  0.0f,  1.0f, 0.0f, // TR
		 0.0f, -0.1f,  0.0f, 1.0f, // BL
		 0.1f, -0.1f,  1.0f, 1.0f  // BR
	};

	const unsigned int inds[] = {
		0, 1, 2,
		2, 3, 0
	};

	struct {
		float ubo_position[2];
	} ubo = { { -1.0f, -0.9f } };

	const int ubo_binding_point = 0;
	mgl_buffer ubo_buffer = mglCreateBuffer(MGL_BUFFER_UNIFORM);
	mglBufferData(ubo_buffer, MGL_USAGE_DYNAMIC, &ubo, sizeof ubo);
	mglProgramBindBuffer(program3, ubo_buffer, ubo_binding_point, "named_ubo");

	mgl_datastore ds = mglCreateDataStore();
	mgl_buffer vbo = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(vbo, MGL_USAGE_CONSTANT, verts, sizeof verts);
	mglDataStoreAttribute(&ds, vbo, 0, MGL_FLOAT2, 0, 0, 0);
	mgl_buffer ivbo = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(ivbo, MGL_USAGE_CONSTANT, instanced, sizeof instanced);
	mglDataStoreAttribute(&ds, ivbo, 1, MGL_FLOAT2, 0, 0, 1);
	mgl_buffer dvbo = mglCreateBuffer(MGL_BUFFER_DRAW_INDIRECT);
	mglBufferData(dvbo, MGL_USAGE_CONSTANT, indirect, sizeof indirect);
	mglAddDataStoreBuffer(&ds, dvbo);

	mgl_datastore ds2 = mglCreateDataStore();
	mgl_buffer vbo2 = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(vbo2, MGL_USAGE_CONSTANT, verts2, sizeof verts2);
	mglDataStoreAttribute(&ds2, vbo2, 0, MGL_FLOAT2, 4 * sizeof(float), 0 * sizeof(float), 0);
	mglDataStoreAttribute(&ds2, vbo2, 1, MGL_FLOAT2, 4 * sizeof(float), 2 * sizeof(float), 0);

	mgl_datastore ds3 = mglCreateDataStore();
	mgl_buffer vbo3 = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(vbo3, MGL_USAGE_CONSTANT, verts, sizeof verts);
	mgl_buffer ebo3 = mglCreateBuffer(MGL_BUFFER_INDICES_UINT);
	mglBufferData(ebo3, MGL_USAGE_CONSTANT, inds, sizeof inds);
	mglDataStoreAttribute(&ds3, vbo3, 0, MGL_FLOAT2, 0, 0, 0);

	mgl_datastore ds4 = mglCreateDataStore();
	mgl_buffer vbo4 = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(vbo4, MGL_USAGE_CONSTANT, verts, sizeof verts);
	mglDataStoreAttribute(&ds4, vbo4, 0, MGL_FLOAT2, 0, 0, 0);
	mgl_buffer ivbo4 = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(ivbo4, MGL_USAGE_CONSTANT, instanced2, sizeof instanced2);
	mglDataStoreAttribute(&ds4, ivbo4, 1, MGL_FLOAT2, 0, 0, 1);

	int location = mglLocateUniform(program, "col");
	int samplerloc = mglLocateUniform(program2, "tex");
	int wire = 0, cd = 0, cdf2 = 0, fid = 0;
	float indcol[3];

	const int texbinding = 0;

	const unsigned char texpixels[] = {
		255, 0,   0,   // R
		0,   255, 0,   // G
		0,   0,   255, // B
		255, 255, 255  // W
	};

	mgl_texture tex = mglCreateTexture(4, 1, texpixels, MGL_RGB);
	mglBindTexture(tex, texbinding);
	mglSetUniform(program2, samplerloc, MGL_SAMPLER, 1, &texbinding);

	srand(time(NULL));

	double start;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		if (fid > 2 && fid  % 60 == 0) start = glfwGetTime();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !cd) { mglFeature((wire = 1 - wire) ? MGL_POLYGON_LINES : MGL_POLYGON_FILL); cd = 1; }
		else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE) cd = 0;
		mglClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		mglClear(MGL_CLEAR_COLOR);
		mglDraw(ds2, program2, MGL_DRAW_STRIPS, 0, 4);
		mglSetUniform(program, location, MGL_FLOAT3, 1, (float[3]){ 1.0f, 1.0f, 1.0f });
		mglDrawInstanced(ds4, program, MGL_DRAW_STRIPS, 0, 4, (fid % 120 <= 60), 3);
		if (fid++ % 60 == 0) { indcol[0] = (float)rand() / RAND_MAX; indcol[1] = (float)rand() / RAND_MAX; indcol[2] = (float)rand() / RAND_MAX; };
		mglSetUniform(program, location, MGL_FLOAT3, 1, indcol);
		mglDrawIndirect(ds, program, MGL_DRAW_STRIPS, (fid / 60) % 2, 1);
		mglDraw(ds3, program3, MGL_DRAW_STRIPS, 0, 6);
		ubo.ubo_position[0] = ubo.ubo_position[0] > 0.9f ? ((ubo.ubo_position[1] += 0.1f), -1.0f) : ubo.ubo_position[0] + 0.016f;
		ubo.ubo_position[1] = ubo.ubo_position[1] > 1.0f ? -0.9f : ubo.ubo_position[1];
		mglBufferDataPartial(ubo_buffer, &ubo, 0, sizeof ubo);
		glfwSwapBuffers(window);
		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && !cdf2) {
			cdf2 = 1;
			FILE *ppm = fopen("screenshot.ppm", "w");
			unsigned char *pixels = malloc((wwidth + 1) * wheight * 3);
			unsigned char *trow = pixels + wwidth * wheight * 3;
			size_t rowsz = wwidth * 3;
			if (!ppm || !pixels) goto end;
			fprintf(ppm, "P6 %d %d 255 ", wwidth, wheight);
			mglReadFrame(0, 0, wwidth, wheight, pixels, MGL_RGB);
			for (int y = 0; y < wheight / 2; ++y) {
				unsigned char *top = pixels + y * rowsz;
				unsigned char *bottom = pixels + (wheight - y - 1) * rowsz;
				memcpy(trow, top, rowsz);
				memcpy(top, bottom, rowsz);
				memcpy(bottom, trow, rowsz);
			}
			fwrite(pixels, wwidth * wheight * 3, 1, ppm);
		end:
			if (pixels) free(pixels);
			if (ppm) fclose(ppm);
		} else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE) cdf2 = 0;
		if (fid > 2 && fid % 60 == 1) printf("FPS: %1f\n", 1.0 / (glfwGetTime() - start));
	}

	mglDestroyBuffer(vbo, ds);
	mglDestroyBuffer(ivbo, ds);
	mglDestroyBuffer(dvbo, ds);
	mglDestroyDataStore(ds);
	mglDestroyProgram(program);

	mglDestroyBuffer(vbo2, ds2);
	mglDestroyDataStore(ds2);
	mglDestroyProgram(program2);

	mglDestroyBuffer(ubo_buffer, ds3);
	mglDestroyBuffer(vbo3, ds3);
	mglDestroyBuffer(ebo3, ds3);
	mglDestroyDataStore(ds3);
	mglDestroyProgram(program3);

	mglDestroyBuffer(vbo4, ds4);
	mglDestroyBuffer(ivbo4, ds4);
	mglDestroyDataStore(ds4);

	mglDestroyTexture(tex);

	glfwTerminate();
}
