#include "SH.h"

#include "sh/default_image.h"
#include "sh/spherical_harmonics.h"

#include <iostream>

#include <embree3/rtcore.h>
#include <embree3/rtcore_ray.h>

#define M_PI 3.14159265358979323846

std::vector<glm::vec3> SphericalHarmonics::computeLightSHCoeff(const Image<glm::vec3>* env, unsigned int order)
{
	int width = env->width;
	int height = env->height;
	sh::DefaultImage sh_envmap(width, height);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			glm::vec3 c = (*env)(x, y);
			Eigen::Array3f pixel(c.x, c.y, c.z);
			sh_envmap.SetPixel(x, y, pixel);
		}
	}

	std::unique_ptr<std::vector<Eigen::Array3f>> sh = ProjectEnvironment(order, sh_envmap);

	// convert to glm::vec3
	std::vector<glm::vec3> coeff;
	for (int i = 0; i < sh->size(); ++i)
	{
		Eigen::Array3f vec = (*sh)[i];
		coeff.push_back(glm::vec3(vec[0], vec[1], vec[2]));
	}
	return coeff;
}

float _halton(int idx, int base)
{
	float f = 1.0f;
	float r = 0.f;
	while (idx > 0)
	{
		f = f / base;
		r = r + f * (idx%base);
		idx = idx / base;
	}
	return r;
}


struct SHSample
{
	glm::vec3 dir;
	std::vector<float> coeff;
};

static std::vector<SHSample> precomputed_samples;


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

void computeVertexSHCoeff(RTCScene scene,
	glm::vec3 normal, glm::vec3 pos,
	const int order,
	float* coeff)
{
	 //// for test?
	 //coeff[0] = 1.f;
	 //return;

	RTCIntersectContext context;
	rtcInitIntersectContext(&context);

	int nSamples = precomputed_samples.size();
	float sampleWeight = M_PI * 4 / nSamples;

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
				for (int k = 0; k < precomputed_samples[i].coeff.size(); ++k)
				{
					coeff[k] += precomputed_samples[i].coeff[k] * w;
				}
			}
		}
	}

}

//std::vector<std::vector<float>>	 SphericalHarmonics::computeSceneSHCoeff(const Scene * pScene, unsigned int order)
//{
//	 pre-compute SH samples
//	if (precomputed_samples.empty())
//	{
//		const int nSamples = 128;
//
//		precomputed_samples.resize(nSamples);
//
//		for (int i = 0; i < nSamples; ++i)
//		{
//			float u = _halton(i, 2);
//			float v = _halton(i, 3);
//
//			double phi = 2 * M_PI * u;
//			double theta = acos(1 - 2 * v);
//
//			double r = sin(theta);
//
//			 TODO: why reflect?
//			precomputed_samples[i].dir = -glm::vec3(r * cos(phi), cos(theta), r * sin(phi));
//
//			for (int l = 0; l <= order; ++l)
//			{
//				for (int m = -l; m <= l; ++m)
//				{
//					precomputed_samples[i].coeff.push_back(sh::EvalSH(l, m, phi, theta));
//				}
//			}
//		}
//	}
//
//	 initialize embree for occlusion test
//	RTCDevice device = rtcNewDevice(NULL);
//	
//	if (rtcGetDeviceError(device) != RTC_ERROR_NONE) {
//		std::cerr << "Failed to create device.\n";
//	}
//
//	RTCScene scene = rtcNewScene(device);
//	if (rtcGetDeviceError(device) != RTC_ERROR_NONE) {
//		std::cerr << "Failed to create scene.\n";
//	}
//
//	vector<glm::vec3> vertices;
//	Object* obj = nullptr;
//	for (unsigned int objIdx = 0; objIdx < pScene->getObjectSize(); objIdx++)
//	{
//		obj = pScene->getObject(objIdx);
//		for (int meshIdx = 0; meshIdx < obj->getMeshSize(); meshIdx++)
//		{
//			vertices.clear();
//			for (int idx = 0; idx < obj->getMeshVertexsSize(meshIdx); idx++)
//			{
//				vertices.push_back(obj->getMeshVertices(meshIdx)[idx].mPos);
//			}
//
//			RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
//
//			glm::vec3* vertex = (glm::vec3*)rtcSetNewGeometryBuffer(geom, RTC_BUFFER_TYPE_VERTEX,
//				0, RTC_FORMAT_FLOAT3, sizeof(glm::vec3),
//				vertices.size());
//			memcpy(vertex, vertices.data(), vertices.size() * sizeof(glm::vec3));
//
//			glm::ivec3* index = (glm::ivec3*)rtcSetNewGeometryBuffer(geom, RTC_BUFFER_TYPE_INDEX,
//				0, RTC_FORMAT_UINT3, sizeof(glm::ivec3),
//				obj->getMeshIndexSize(meshIdx) / 3);
//			memcpy(index, obj->getMeshIndices(meshIdx).data(), pScene->getObject(i)->getMeshIndexSize(k) * sizeof(unsigned int));
//
//			rtcCommitGeometry(geom);
//			rtcAttachGeometry(scene, geom);
//			rtcReleaseGeometry(geom);
//		}
//	}
//	rtcCommitScene(scene);
//
//	 compuate SH coeff
//	std::vector<std::vector<float>> coeffs(pScene->getObject(i)->getMeshSize());
//	int nCoeff = (order + 1) * (order + 1);
//
//	for (unsigned int idx = 0; idx < pScene->getObject(i)->getMeshSize(); i++)
//	{
//			
//		coeffs[idx].resize(pScene->getObject(i)->getMeshVertexsSize(idx) * nCoeff, 0.f);
//			
//		for (int j = 0; j <pScene->getObject(i)->getMeshVertexsSize(idx); j++)
//		{
//			glm::vec3 P = pScene->getObject(i)
//				mesh->positions[j];
//			glm::vec3 N = mesh->normals[j];
//
//			 SH for each vertex
//			computeVertexSHCoeff(scene, N, P, order, &coeffs[i][j * nCoeff]);
//		}
//
//
//		const Mesh* mesh = pScene->meshes[i];
//
//		 SH for each mesh
//		coeffs[i].resize(mesh->getNumVerts() * nCoeff, 0.f);
//
//#pragma omp parallel for
//		for (int j = 0; j < mesh->getNumVerts(); j++)
//		{
//			glm::vec3 P = mesh->positions[j];
//			glm::vec3 N = mesh->normals[j];
//
//			 SH for each vertex
//			computeVertexSHCoeff(scene, N, P, order, &coeffs[i][j * nCoeff]);
//		}
//	}
//
//	rtcReleaseScene(scene);
//	rtcReleaseDevice(device);
//
//	return coeffs;
//	}
//}
