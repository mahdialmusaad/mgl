#include "mgl/mgl.h"
#include "GLFW/glfw3.h"

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

static mgl_framebuffer fb;
static GLFWwindow *window;
static int wwidth = 1280, wheight = 720;
static void err(int id, const char *desc) { printf("ERROR %d: %s\n", id, desc); }
static void framebuffer(GLFWwindow *fwindow, int width, int height) { (void)fwindow; mglViewport(0, 0, wwidth = width, wheight = height); mglResizeFramebuffer(&fb, width, height); }

static float nrand(void)
{
	const int res = rand();
	return (float)((double)res * (1.0 / (double)RAND_MAX));
}

static uint64_t timestamp(void)
{
	unsigned int lo, hi;
	mglTimestamp(&lo, &hi);
	return ((uint64_t)hi << 32) | lo;
}

int main(int argc, char **argv)
{
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
	"void main(){ gl_Position = vec4(position * 2.0 + vec2(-0.1, 0.1), 0.0, 1.0); coords = tcoords * 1.1 + vec2(-0.05, -0.05); }";
	const char *fragment_source2 = "#version 330\n"
	"out vec4 frag;"
	"uniform sampler2D tex;"
	"in vec2 coords;"
	"void main(){ frag = texture(tex, coords); }";

	const char *vertex_source3 = "#version 330\n"
	"layout(location = 0) in vec2 position;"
	"layout(std140) uniform named_ubo { vec2 ubo_position; };"
	"void main(){ gl_Position = vec4(position + ubo_position, 0.0, 1.0); }";
	const char *fragment_source3 = "#version 330\n"
	"out vec4 frag;"
	"void main(){ frag = vec4(1.0, 0.0, 0.0, 0.5); }";

	const char *vertex_source4 = "#version 330\n"
	"layout(location = 0) in vec2 position;"
	"layout(location = 1) in vec2 tcoords;"
	"out vec2 coords;"
	"void main(){ gl_Position = vec4(position, 0.0, 1.0); coords = tcoords; }";
	const char *fragment_source4 = "#version 330\n"
	"out vec4 frag;"
	"uniform sampler2D tex;"
	"uniform bool invert;"
	"in vec2 coords;"
	"void main(){ if (invert) frag = vec4(vec3(1.0 - texture(tex, coords)), 1.0); else frag = texture(tex, coords); }";

	const float verts[] = {
		 0.0f,  0.0f,
		 0.1f,  0.0f,
		 0.0f, -0.1f,
		 0.1f, -0.1f
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
		 0.4f,  -0.4f
	};
	const mgl_indirect_cmd indirect[] = {
		{ 4, 4, 0, 0 },
		{ 4, 4, 0, 4 }
	};

	const float verts2[] = {
		 0.0f,  0.0f,  0.0f, 0.0f,
		 0.1f,  0.0f,  1.0f, 0.0f,
		 0.0f, -0.1f,  0.0f, 1.0f,
		 0.1f, -0.1f,  1.0f, 1.0f
	};

	const unsigned int inds[] = {
		0, 1, 2,
		2, 3, 0
	};

	const float vertsfb[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f
	};

	const unsigned char texpixels[] = {
		255, 0,   0,
		0,   255, 0,
		0,   0,   255,
		255, 255, 255,

		255, 255, 0,
		0,   255, 255,
		128, 128, 128,
		255, 0,   255
	};

	mgl_program program, program2, program3, program4;
	mgl_shader finalshaders[2];
	mgl_buffer ubo_buffer, vbo, ivbo, dvbo, vbo2, vbo3, ebo3, vbo4, ivbo4, vbofb;
	mgl_datastore ds, ds2, ds3, ds4, dsfb;
	mgl_uniform location, samplerloc, fbtexloc, invertloc;
	mgl_texture tex, fbtex;

	struct { float ubo_position[2]; } ubo = { { -1.0f, -0.9f } };
	const float p1col[] = { 1.0f, 1.0f, 1.0f };
	int wire = 0, cd = 0, cdf2 = 0, cdf3 = 0, cdf4 = 0, invert = 0, apiver = 0;
	const int ubo_binding_point = 0, texbinding = 0, fbtexbinding = 1;
	size_t fid = 0, y;
	float indcol[3];
	double start = 0.0;

	char a1 = argc > 1 ? argv[1][0] : '\0';
	char a2 = argc > 2 ? argv[2][0] : '\0';

	if (a1 == 'x' || a1 == 'X' || a2 == 'x' || a2 == 'X') glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
	glfwSetErrorCallback(err);
	if (!glfwInit()) { fprintf(stderr, "Failed to initialize GLFW\n"); exit(EXIT_FAILURE); }
	if (a1 == 'e' || a1 == 'E' || a2 == 'e' || a2 == 'E') {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		apiver = 31;
	} else {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		apiver = 33;
	}
	window = glfwCreateWindow(wwidth, wheight, "mgl", NULL, NULL);
	if (!window) { fprintf(stderr, "Failed to create window\n"); exit(EXIT_FAILURE); }
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer);
	glfwSwapInterval(1);
	mglDebug(err);
	if (!mglInit(glfwGetProcAddress, apiver)) { fprintf(stderr, "Failed to load OpenGL functions\n"); exit(EXIT_FAILURE); }

	printf("GL %s | GLSL %s | %s (%s)\n", mglString(MGL_STRING_VERSION), mglString(MGL_STRING_GLSLVERSION), mglString(MGL_STRING_RENDERER), mglString(MGL_STRING_VENDOR));
	printf("Extensions: %d (%s, ..., %s)\n", (int)mglValue(MGL_VALUE_NUMEXTS), mglString(MGL_STRING_EXTENSION0), mglString(MGL_STRING_EXTENSION0 + (unsigned)mglValue(MGL_VALUE_NUMEXTS) - 1));
	printf(
		"Maximum counts: %d tex units, %d vertex attribs, %d uniforms, %d UBOs (any shader), %d SSBOs (any shader) | Maximum sizes: %dB UBO, %dB SSBO\n",
		mglValue(MGL_VALUE_MAX_TEXUNITS), mglValue(MGL_VALUE_MAX_ATTRIBS),
		mglValue(MGL_VALUE_MAX_UNIFORMS), mglValue(MGL_VALUE_MAX_UBOS), mglValue(MGL_VALUE_MAX_SSBOS),
		mglValue(MGL_VALUE_MAX_UBO_SIZE), mglValue(MGL_VALUE_MAX_SSBO_SIZE)
	);
	printf("Current timestamp: %" PRIu64 "\n", timestamp());

	mglColor(MGL_COLOR_BLEND, 1.0f, 1.0f, 0.0f, 1.0f);
	mglColor(MGL_COLOR_CLEAR, 0.2f, 0.2f, 0.2f, 1.0f);
	mglColor(MGL_COLOR_TEXBORDER, 1.0f, 0.5f, 0.0f, 1.0f);
	mglFeature(MGL_DOWNGRADE_SSBO_ON);
	mglFeature(MGL_BLEND_1MSRCA);
	mglFeature(MGL_CULL_BACK_CW);

	fb = mglCreateFramebuffer(wwidth, wheight);
	fbtex = mglFramebufferTexture(&fb);

	finalshaders[0] = mglCreateShader(MGL_SHADER_VERTEX_FRAGILE, &vertex_source, 1);
	finalshaders[1] =  mglCreateShader(MGL_SHADER_FRAGMENT_FRAGILE, &fragment_source, 1);
	program = mglCreateProgram(finalshaders, 2);

	finalshaders[0] = mglCreateShader(MGL_SHADER_VERTEX_FRAGILE, &vertex_source2, 1);
	finalshaders[1] = mglCreateShader(MGL_SHADER_FRAGMENT_FRAGILE, &fragment_source2, 1);
	program2 = mglCreateProgram(finalshaders, 2);

	finalshaders[0] = mglCreateShader(MGL_SHADER_VERTEX_FRAGILE, &vertex_source3, 1);
	finalshaders[1] = mglCreateShader(MGL_SHADER_FRAGMENT_FRAGILE, &fragment_source3, 1);
	program3 = mglCreateProgram(finalshaders, 2);

	finalshaders[0] = mglCreateShader(MGL_SHADER_VERTEX_FRAGILE, &vertex_source4, 1);
	finalshaders[1] = mglCreateShader(MGL_SHADER_FRAGMENT_FRAGILE, &fragment_source4, 1);
	program4 = mglCreateProgram(finalshaders, 2);

	ubo_buffer = mglCreateBuffer(MGL_BUFFER_UNIFORM);
	mglBufferData(&ubo_buffer, MGL_USAGE_DYNAMIC, &ubo, sizeof ubo);
	mglProgramBindBuffer(&program3, &ubo_buffer, ubo_binding_point, "named_ubo");

	ds = mglCreateDataStore();
	vbo = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(&vbo, MGL_USAGE_CONSTANT, verts, sizeof verts);
	mglDataStoreAttribute(&ds, &vbo, 0, MGL_FLOAT2, 0, 0, 0);
	ivbo = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(&ivbo, MGL_USAGE_CONSTANT, instanced, sizeof instanced);
	mglDataStoreAttribute(&ds, &ivbo, 1, MGL_FLOAT2, 0, 0, 1);
	dvbo = mglCreateBuffer(MGL_BUFFER_DRAW_INDIRECT);
	mglBufferData(&dvbo, MGL_USAGE_CONSTANT, indirect, sizeof indirect);
	mglAddDataStoreBuffer(&ds, &dvbo);

	ds2 = mglCreateDataStore();
	vbo2 = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(&vbo2, MGL_USAGE_CONSTANT, verts2, sizeof verts2);
	mglDataStoreAttribute(&ds2, &vbo2, 0, MGL_FLOAT2, 4 * sizeof(float), 0 * sizeof(float), 0);
	mglDataStoreAttribute(&ds2, &vbo2, 1, MGL_FLOAT2, 4 * sizeof(float), 2 * sizeof(float), 0);

	ds3 = mglCreateDataStore();
	vbo3 = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(&vbo3, MGL_USAGE_DYNAMIC, NULL, sizeof verts);
	mglCopyBufferData(&vbo, 0, &vbo3, 0, sizeof verts);
	mglDataStoreAttribute(&ds3, &vbo3, 0, MGL_FLOAT2, 0, 0, 0);
	ebo3 = mglCreateBuffer(MGL_BUFFER_INDICES_UINT);
	mglBufferData(&ebo3, MGL_USAGE_CONSTANT, inds, sizeof inds);

	ds4 = mglCreateDataStore();
	vbo4 = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(&vbo4, MGL_USAGE_CONSTANT, verts, sizeof verts);
	mglDataStoreAttribute(&ds4, &vbo4, 0, MGL_FLOAT2, 0, 0, 0);
	ivbo4 = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(&ivbo4, MGL_USAGE_CONSTANT, instanced2, sizeof instanced2);
	mglDataStoreAttribute(&ds4, &ivbo4, 1, MGL_FLOAT2, 0, 0, 1);

	dsfb = mglCreateDataStore();
	vbofb = mglCreateBuffer(MGL_BUFFER_VERTICES);
	mglBufferData(&vbofb, MGL_USAGE_CONSTANT, vertsfb, sizeof vertsfb);
	mglDataStoreAttribute(&dsfb, &vbofb, 0, MGL_FLOAT2, 4 * sizeof(float), 0 * sizeof(float), 0);
	mglDataStoreAttribute(&dsfb, &vbofb, 1, MGL_FLOAT2, 4 * sizeof(float), 2 * sizeof(float), 0);

	location = mglLocateUniform(&program, "col");
	samplerloc = mglLocateUniform(&program2, "tex");
	fbtexloc = mglLocateUniform(&program4, "tex");
	invertloc = mglLocateUniform(&program4, "invert");

	tex = mglCreateTexture(texpixels, MGL_RGB_U8, 4, 2, MGL_TEXTURE_SCALING_NEAREST | MGL_TEXTURE_WRAP_BORDER | MGL_TEXTURE_IMMUTABLE);
	mglBufferTexture(&tex, texpixels, MGL_RGB_U8, 4, 2);
	printf("Error example: ");
	mglBufferTexture(&tex, texpixels, MGL_RGB_U8, 3, 2);
	mglSetUniform(&program2, &samplerloc, 1, &texbinding);
	mglSetUniform(&program4, &fbtexloc, 1, &fbtexbinding);

	srand((unsigned int)time(NULL));

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		if (fid > 2 && fid  % 60 == 0) start = glfwGetTime();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !cd) { mglFeature((wire = 1 - wire) ? MGL_POLYGON_LINES : MGL_POLYGON_FILL); cd = 1; }
		else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE) cd = 0;
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !cdf3) { invert = 1 - invert; mglSetUniform(&program4, &invertloc, 1, &invert); cdf3 = 1; }
		else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE) cdf3 = 0;
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !cdf4) { float bufpos[2]; mglGetBufferData(&ubo_buffer, 0, sizeof bufpos, bufpos); printf("Position from buffer: %f, %f\n", (double)bufpos[0], (double)bufpos[1]); cdf4 = 1; }
		else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) cdf4 = 0;
		mglBindTexture(&tex, texbinding);
		mglUseFramebuffer(&fb);
		mglFeature(MGL_DEPTH_LEQUAL);
		mglClear(MGL_CLEAR_DEPTH | MGL_CLEAR_COLOR);
		mglDraw(&ds2, &program2, MGL_DRAW_STRIPS, 0, 4);
		mglSetUniform(&program, &location, 1, p1col);
		mglDrawInstanced(&ds4, &program, MGL_DRAW_STRIPS, 0, 4, (fid % 120 <= 60), 3, 0);
		if (fid++ % 60 == 0) { indcol[0] = nrand(); indcol[1] = nrand(); indcol[2] = nrand(); }
		mglSetUniform(&program, &location, 1, indcol);
		mglDrawIndirect(&ds, &program, MGL_DRAW_STRIPS, (fid / 60) % 2, 1);
		mglDraw(&ds3, &program3, MGL_DRAW_STRIPS, 0, 6);
		mglUseFramebuffer(NULL);
		mglClear(MGL_CLEAR_DEPTH | MGL_CLEAR_COLOR);
		mglFeature(MGL_DEPTH_OFF);
		mglBindTexture(&fbtex, fbtexbinding);
		mglDraw(&dsfb, &program4, MGL_DRAW_STRIPS, 0, 4);
		mglDrawFramebuffer(&fb, 0, 0, wwidth, wheight, NULL, wwidth - wwidth / 6 - 50, wheight - wheight / 6 - 50, wwidth / 6, wheight / 6, MGL_CLEAR_COLOR, MGL_TEXTURE_SCALING_LINEAR);
		ubo.ubo_position[0] = ubo.ubo_position[0] > 0.9f ? ((void)((ubo.ubo_position[1] += 0.1f)), -1.0f) : ubo.ubo_position[0] + 0.016f;
		ubo.ubo_position[1] = ubo.ubo_position[1] > 1.0f ? -0.9f : ubo.ubo_position[1];
		mglBufferDataPartial(&ubo_buffer, &ubo, 0, sizeof ubo);
		glfwSwapBuffers(window);
		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && !cdf2) {
			const size_t w = (size_t)wwidth, h = (size_t)wheight;
			FILE *ppm = fopen("screenshot.ppm", "w");
			unsigned char *pixels = (unsigned char *)malloc(w * (h + 1) * 3);
			unsigned char *trow = pixels + w * h * 3;
			size_t rowsz = w * 3;
			cdf2 = 1;
			if (!ppm || !pixels) goto end;
			fprintf(ppm, "P6 %d %d 255 ", (int)w, (int)h);
			mglReadFrame(NULL, pixels, MGL_RGB_U8, 0, 0, (int)w, (int)h);
			for (y = 0; y < h / 2; ++y) {
				unsigned char *top = pixels + y * rowsz;
				unsigned char *bottom = pixels + (h - y - 1) * rowsz;
				memcpy(trow, top, rowsz);
				memcpy(top, bottom, rowsz);
				memcpy(bottom, trow, rowsz);
			}
			fwrite(pixels, w * h * 3, 1, ppm);
		end:
			if (pixels) free(pixels);
			if (ppm) fclose(ppm);
		} else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE) cdf2 = 0;
		if (fid > 2 && fid % 60 == 1) printf("FPS: %1f\n", 1.0 / (glfwGetTime() - start));
	}

	mglDestroyBuffer(&vbo, &ds);
	mglDestroyBuffer(&ivbo, &ds);
	mglDestroyBuffer(&dvbo, &ds);
	mglDestroyDataStore(&ds);
	mglDestroyProgram(&program);

	mglDestroyBuffer(&vbo2, &ds2);
	mglDestroyDataStore(&ds2);
	mglDestroyProgram(&program2);

	mglDestroyBuffer(&ubo_buffer, &ds3);
	mglDestroyBuffer(&vbo3, &ds3);
	mglDestroyBuffer(&ebo3, &ds3);
	mglDestroyDataStore(&ds3);
	mglDestroyProgram(&program3);

	mglDestroyBuffer(&vbo4, &ds4);
	mglDestroyBuffer(&ivbo4, &ds4);
	mglDestroyDataStore(&ds4);

	mglDestroyBuffer(&vbofb, &dsfb);
	mglDestroyDataStore(&dsfb);
	mglDestroyFramebuffer(&fb);

	mglDestroyTexture(&tex);
	mglDestroyProgram(&program4);

	glfwTerminate();

	return 0;
}
