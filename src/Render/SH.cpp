#include "SH.h"

#include "sh/default_image.h"
#include "sh/spherical_harmonics.h"

#include <iostream>
#include <embree3/rtcore.h>
#include <embree3/rtcore_ray.h>

#define M_PI 3.14159265358979323846

std::vector<glm::vec3> SphericalHarmonics::computeLightSHCoeff(const Image<float>& env, unsigned int order)
{
	int width = env.width;
	int height = env.height;
	sh::DefaultImage sh_envmap(width, height);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float r = env(x, y, 0);
			float g = env(x, y, 1);
			float b = env(x, y, 2);
			Eigen::Array3f color(r, g, b);
			sh_envmap.SetPixel(x, y, color);
		}
	}
	std::unique_ptr<std::vector<Eigen::Array3f>> sh = ProjectEnvironment(order, sh_envmap);

	std::vector<glm::vec3> coeff;
	for (int i = 0; i < sh->size(); i++)
	{
		Eigen::Array3f vec = (*sh)[i];
		coeff.push_back(glm::vec3(vec[0], vec[1], vec[2]));
	}

	return coeff;
}

RTCRay _getRTCRay(const glm::vec3& o, const glm::vec3& d) {
	RTCRay ray;
	ray.org_x = o[0];
	ray.org_y = o[1];
	ray.org_z = o[2];

	ray.dir_x = d[0];
	ray.dir_y = d[1];
	ray.dir_z = d[2];

	ray.tnear = FLT_EPSILON;
	ray.tfar = FLT_MAX;
	ray.flags = 0;

	return ray;
}

RTCScene initEmbree(shared_ptr<Scene> pScene, RTCDevice device)
{
	RTCScene scene = rtcNewScene(device);
	if (rtcGetDeviceError(device) != RTC_ERROR_NONE) {
		std::cerr << "Failed to create scene.\n";
	}

	int meshSize = pScene->mMeshes.size();
	for (unsigned int meshIdx = 0; meshIdx < meshSize; meshIdx++)
	{
		shared_ptr<Mesh> mesh = pScene->mMeshes[meshIdx];
		RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

		glm::vec3* vertex = (glm::vec3*)rtcSetNewGeometryBuffer(geom, RTC_BUFFER_TYPE_VERTEX,
			0, RTC_FORMAT_FLOAT3, sizeof(glm::vec3),
			mesh->mPositions.size());
		memcpy(vertex, mesh->mPositions.data(), mesh->mPositions.size() * sizeof(glm::vec3));

		glm::ivec3* index = (glm::ivec3*)rtcSetNewGeometryBuffer(geom, RTC_BUFFER_TYPE_INDEX,
			0, RTC_FORMAT_UINT3, sizeof(unsigned int),
			mesh->mIndices.size());
		memcpy(index, mesh->mIndices.data(), mesh->mIndices.size() * sizeof(unsigned int));

		rtcCommitGeometry(geom);
		rtcAttachGeometry(scene, geom);
		rtcReleaseGeometry(geom);
	}
	rtcCommitScene(scene);
	return scene;
}

struct SHSample
{
	glm::vec3 dir;
	std::vector<float> coeff;
};

static vector<SHSample> precomputed_samples;
void initSampleLights(int order)
{
	const int nSamples = 128;
	precomputed_samples.resize(nSamples);
	for (int i = 1; i <= nSamples; i++)
	{
		float u = Samplefunc::halton(i, 2);
		float v = Samplefunc::halton(i, 3);

		double phi = 2 * M_PI * u;
		double theta = acos(1 - 2 * v);

		double r = sin(theta);

		//TODO: why reflect?
		precomputed_samples[i - 1].dir = -glm::vec3(r * cos(phi), cos(theta), r * sin(phi));

		for (int l = 0; l <= order; ++l)
		{
			for (int m = -l; m <= l; ++m)
			{
				precomputed_samples[i - 1].coeff.push_back(sh::EvalSH(l, m, phi, theta));
			}
		}
	}

}

void computeVertexSHCoeff(RTCScene scene, glm::vec3 normal, glm::vec3 pos,
	vector<float> &coeff)
{
	RTCIntersectContext context;
	rtcInitIntersectContext(&context);

	int nSamples = precomputed_samples.size();
	float sampleWeight = M_PI * 4 / nSamples;


	for (int k = 0; k < precomputed_samples[0].coeff.size(); ++k)
	{
		float data = 0.f;
		for (int i = 0; i < nSamples; ++i)
		{
			glm::vec3 sampleDir = precomputed_samples[i].dir;
			float nDotL = glm::dot(glm::normalize(sampleDir), glm::normalize(normal));
			if (nDotL > 0)
			{
				float w = sampleWeight * nDotL;

				RTCRay sample_ray = _getRTCRay(pos + 0.001f * normal, sampleDir);
				rtcOccluded1(scene, &context, &sample_ray);

				if (sample_ray.tfar > 0)
				{
					data += precomputed_samples[i].coeff[k] * w;
				}
			}
		}
		coeff.push_back(data);
	}
}

vector<vector<float>> SphericalHarmonics::computeSceneSHTrans(shared_ptr<Scene> pScene, unsigned int order)
{
	if (precomputed_samples.empty())
	{
		initSampleLights(order);
	}
	RTCDevice device = rtcNewDevice(NULL);

	if (rtcGetDeviceError(device) != RTC_ERROR_NONE) {
		std::cerr << "Failed to create device.\n";
	}
	RTCScene rtscene = initEmbree(pScene, device);

	vector<vector<float>> trans(pScene->mMeshes.size());

	int meshSize = pScene->mMeshes.size();
	for (int meshIdx = 0; meshIdx < meshSize; meshIdx++)
	{
		shared_ptr<Mesh> mesh = pScene->mMeshes[meshIdx];
		int vertexNums = mesh->mPositions.size();
		for (int i = 0; i < vertexNums; i++)
		{
			glm::vec3 pos = mesh->mPositions[i];
			glm::vec3 normal = mesh->mNormals[i];
			computeVertexSHCoeff(rtscene, normal, pos, trans[meshIdx]);
		}
	}
	rtcReleaseScene(rtscene);
	rtcReleaseDevice(device);

	return trans;
};



