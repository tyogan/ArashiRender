#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"

#include "vao.h"
#include "shader.h"
#include "Utility/image.h"
#include "Core/mesh.h"

#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;
using std::shared_ptr;

class RENDER_API Envmap
{
public:
	Envmap();
	~Envmap();

	void init();
	void load(string path);

	void drawBackground(glm::mat4 view, glm::mat4 proj);

private:
	void createCubeTexture();

private:
	shared_ptr<ShaderProgram> mRenderBgProgram;

	GLuint mCubeTex;
	GLuint mLoadImageTex;

	shared_ptr<VAO> mCubeVAO;
};