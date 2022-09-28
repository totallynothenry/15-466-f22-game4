/**
 * Code designed with reference to the HalfBuzz-FreeType tutorial:
 * https://github.com/harfbuzz/harfbuzz-tutorial/blob/master/hello-harfbuzz-freetype.c
 * this OpenGL-FreeType tutorial:
 * https://learnopengl.com/In-Practice/Text-Rendering
 * and to PPU466 from 15-466-f22-base1
 *
 * Text Program adapted from PPU466 Program and shaders in the OpenGL-FreeType tutorial
 */

#include "Text.hpp"

#include "GL.hpp"
#include "gl_compile_program.hpp"
#include "gl_errors.hpp"
#include "Load.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <stdexcept>
#include <vector>


// #define DEBUG

// Debug logging
#ifdef DEBUG
#include <iostream>
#define LOG(ARGS) std::cout << ARGS << std::endl;
#else
#define LOG(ARGS)
#endif


#define X_MAX 1280.0f

// Text Program --------------------------------------------------

struct TextProgram {
	TextProgram();
	~TextProgram();

	GLuint program = 0;

	GLuint PROJECTION_mat4 = -1U;
	GLuint TEXT_COLOR_vec3 = -1U;
};

TextProgram::TextProgram() {
	program = gl_compile_program(
		//vertex shader:
		"#version 330\n"
		"in vec4 Vertex;\n"
		"out vec2 texCoords;\n"
		"uniform mat4 PROJECTION;\n"
		"void main() {\n"
		"	gl_Position = PROJECTION * vec4(Vertex.xy, 0.0, 1.0);\n"
		"	texCoords = Vertex.zw;\n"
		"}\n"
	,
		//fragment shader:
		"#version 330\n"
		"in vec2 texCoords;\n"
		"out vec4 color;\n"
		"uniform sampler2D TEXT;\n"
		"uniform vec3 TEXT_COLOR;\n"
		"void main() {\n"
		"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(TEXT, texCoords).r);\n"
		"	color = vec4(TEXT_COLOR, 1.0) * sampled;\n"
		"}\n"
	);

	PROJECTION_mat4 = glGetUniformLocation(program, "PROJECTION");
	TEXT_COLOR_vec3 = glGetUniformLocation(program, "TEXT_COLOR");

	GL_ERRORS();
}

TextProgram::~TextProgram() {
	if (program != 0) {
		glDeleteProgram(program);
		program = 0;
	}
}

static Load< TextProgram > text_program(LoadTagEarly);


// Text --------------------------------------------------

// VBO and VAO for text
static GLuint VBO = 0;
static GLuint VAO = 0;

// Setting up buffers early like with DrawLines
static Load< void > setup_buffers(LoadTagDefault, [](){
	// From OpenGL-FreeType tutorial (see comment at beginning of file)

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Enable blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	// Bind
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Configure
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, //attribute
		4, //size
		GL_FLOAT, //type
		GL_FALSE, //normalized
		4 * sizeof(float), //stride
		0  //offset
	);

	// Done, clear binds
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GL_ERRORS(); //PARANOIA: make sure nothing strange happened during setup
});


Text::Text(std::string font_file_path, unsigned int size) {
	font_size = size;

	// From HalfBuzz-FreeType tutorial (see comment at beginning of file)
	if (FT_Init_FreeType(&ft_library)) {
		throw std::runtime_error("Failed to init ft_library");
	}
	if (FT_New_Face(ft_library, font_file_path.c_str(), 0, &ft_face)) {
		throw std::runtime_error("Failed to create ft_face from font_file_path " + font_file_path);
	}
	if (FT_Set_Char_Size(ft_face, font_size*64, font_size*64, 0, 0)) {
		throw std::runtime_error("Failed to set char size, font_size = " + std::to_string(font_size));
	}

	hb_font = hb_ft_font_create(ft_face, NULL);
	hb_buffer = hb_buffer_create();
}

Text::~Text() {
	// From HalfBuzz-FreeType tutorial (see comment at beginning of file)
	hb_buffer_destroy (hb_buffer);
	hb_font_destroy (hb_font);

	FT_Done_Face(ft_face);
	FT_Done_FreeType(ft_library);
}

Text::Glyph &Text::get_glyph(hb_codepoint_t glyph_idx) {
	// From OpenGL-FreeType tutorial (see comment at beginning of file)

	// If rendered glyph already exists, skip
	auto it = glyphs.find(glyph_idx);
	if (it != glyphs.end()) {
		return it->second;
	}

	// Load the glyph with FreeType
	if (FT_Load_Glyph(ft_face, glyph_idx, FT_LOAD_RENDER)) { // Loading glyph directly here, different from tutorial
		throw std::runtime_error("Failed to load glyph, index " + std::to_string(glyph_idx));
	}
	auto glyph_slot = ft_face->glyph; // compiler would do this anyway and this makes the code cleaner

	// Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        glyph_slot->bitmap.width,
        glyph_slot->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        glyph_slot->bitmap.buffer
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	GL_ERRORS();

	// Construct Glyph
	Glyph glyph = {
        texture,
        glm::ivec2(glyph_slot->bitmap.width, glyph_slot->bitmap.rows),
        glm::ivec2(glyph_slot->bitmap_left, glyph_slot->bitmap_top),
        static_cast< unsigned int >(glyph_slot->advance.x)
    };

	// Cache in glyphs
	glyphs.insert(std::pair< hb_codepoint_t, Glyph >(glyph_idx, glyph));

	return glyphs[glyph_idx];
}

void Text::display(std::string const &text, glm::uvec2 const &drawable_size, float x, float y,
		float const &scale, glm::vec3 const &color) {
	// From HalfBuzz-FreeType tutorial (see comment at beginning of file)

	// Load into buffer and then guess
	hb_buffer_add_utf8(hb_buffer, text.c_str(), -1, 0, -1);
	hb_buffer_guess_segment_properties(hb_buffer);

	// Shape (easier than I thought...)
	hb_shape(hb_font, hb_buffer, NULL, 0);

	/**
	 * Get glyph info and position for rendering
	 *
	 * For reference:
	 * ---
	 * typedef struct {
	 *     hb_codepoint_t codepoint; <-- is glyph index after shaping
	 *     uint32_t       cluster;
	 * } hb_glyph_info_t;
	 * ---
	 */

	hb_glyph_info_t *info = hb_buffer_get_glyph_infos(hb_buffer, NULL);
	unsigned int len = hb_buffer_get_length(hb_buffer);


	// From OpenGL-FreeType tutorial (see comment at beginning of file)

	// Load text shaders and set uniforms
	glm::mat4 projection =
		glm::ortho(0.0f, static_cast<float>(drawable_size.x), 0.0f, static_cast<float>(drawable_size.y));
	glUseProgram(text_program->program);
	glUniform3f(text_program->TEXT_COLOR_vec3, color.x, color.y, color.z);
	glUniformMatrix4fv(text_program->PROJECTION_mat4, 1, GL_FALSE, glm::value_ptr(projection));

	// Render glyphs
	glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
	GL_ERRORS();

	for (unsigned int i = 0; i < len; i++) {
		Glyph &glyph = get_glyph(info[i].codepoint);

		float xpos = x + glyph.bearing.x * scale;
		float ypos = y - (glyph.size.y - glyph.bearing.y) * scale;

		float w = glyph.size.x * scale;
		float h = glyph.size.y * scale;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, glyph.textureId);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (glyph.advance >> 6) * scale;

		GL_ERRORS();
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GL_ERRORS();

	// Reset buffer for next display
	hb_buffer_reset(hb_buffer);
}

void Text::display_wrapped(std::string const &text, glm::uvec2 const &drawable_size, float x, float y,
		float const &scale, glm::vec3 const &color) {
	std::vector< std::string > lines;
	lines.emplace_back(text);
	int idx = 0;

	// From HalfBuzz-FreeType tutorial (see comment at beginning of file)
	hb_glyph_position_t *pos;
	unsigned int len;
	float xpos_max = static_cast< float >(drawable_size.x) - x;
	while (idx < (int)lines.size()) {
		std::string line = lines[idx];

		// From HalfBuzz-FreeType tutorial (see comment at beginning of file)

		// Load into buffer and then guess
		hb_buffer_add_utf8(hb_buffer, line.c_str(), -1, 0, -1);
		hb_buffer_guess_segment_properties(hb_buffer);

		// Shape
		hb_shape(hb_font, hb_buffer, NULL, 0);

		/**
		 * Get glyph info and position for rendering
		 *
		 * For reference:
		 * ---
		 * typedef struct {
		 *     hb_codepoint_t codepoint; <-- is glyph index after shaping
		 *     uint32_t       cluster;
		 * } hb_glyph_info_t;
		 * ---
		 * typedef struct {
		 *     hb_position_t  x_advance;
		 *     hb_position_t  y_advance;
		 *     hb_position_t  x_offset;
		 *     hb_position_t  y_offset;
		 * } hb_glyph_position_t;
		 */
		pos = hb_buffer_get_glyph_positions (hb_buffer, NULL);
		len = hb_buffer_get_length(hb_buffer);

		// Hacky text wrap. Uses provided x to calculate right-margin as well
		float xpos = x;
		unsigned int last_whitepsace = 0;
		for (unsigned int i = 0; i < len; i++) {
			if (line[i] == ' ') {
				last_whitepsace = i;
			}

			xpos += (pos[i].x_advance >> 6) * scale;

			if (xpos > xpos_max) {
				// Line too long, split and add a new line, then retry wrap
				if (last_whitepsace == 0) {
					lines[idx] = line.substr(0, i-2) + "-";
					lines.emplace_back(line.substr(i-2));
				} else {
					lines[idx] = line.substr(0, last_whitepsace);
					lines.emplace_back(line.substr(last_whitepsace + 1));
				}
				break;
			}
		}

		hb_buffer_reset(hb_buffer);
		idx++;
	}

	float ypos = y;
	for (auto &line : lines) {
		display(line, drawable_size, x, ypos, scale, color);
		ypos -= static_cast< float >(font_size) * scale * 1.5f;
	}
}