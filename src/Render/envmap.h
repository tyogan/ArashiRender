#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"

#include "vao.h"
#include "shader.h"
#include "sh.h"
#include "Utility/image.h"
#include "Core/mesh.h"

#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;
using std::shared_ptr;

struct EnvmapLight
{
	glm::vec3 mEnvmapLightDir;
	glm::vec3 mEnvmapLightColor;
	int lightSize = 0;
	int lightCapacity = 128;
};

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

public:
	vector<glm::vec3> mSHLight;

private:
	shared_ptr<ShaderProgram> mRenderBgProgram;

	GLuint mCubeTex;
	GLuint mLoadImageTex;

	shared_ptr<VAO> mCubeVAO;


};