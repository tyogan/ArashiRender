#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"

#include "vao.h"
#include "shader.h"
#include "sh.h"
#include "ibltree.h"

#include "Utility/image.h"
#include "Core/mesh.h"
#include "Core/light.h"

#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;
using std::shared_ptr;

struct EnvmapLight
{
	glm::vec3 mDir;
	glm::vec3 mColor;
};

class RENDER_API Envmap
{
public:
	~Envmap();

	void init();
	void load(string path);

	void drawBackground(glm::mat4 view, glm::mat4 proj);

private:
	void createCubeTexture();
	void createIrradianceTexture();

public:
	vector<glm::vec3> mSHLight;
	vector<EnvmapLight> mEnvLights;

private:
	GLuint mEnvmapImageTex;
	GLuint mEnvmapCubeTex;
	GLuint mIrradianceCubeTex;

	shared_ptr<VAO> mSphereVAO;
	shared_ptr<ShaderProgram> mRenderBgProgram;
	shared_ptr<ShaderProgram> mCreateCubeTexProgram;
	shared_ptr<ShaderProgram> mCreateIrradianceTexProgram;
};