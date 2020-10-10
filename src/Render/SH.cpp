//#include "SH.h"
//
//#include "sh/default_image.h"
//#include "sh/spherical_harmonics.h"
//
//#include <iostream>
//
//#include <embree3/rtcore.h>
//#include <embree3/rtcore_ray.h>
//
//#define M_PI 3.14159265358979323846
//
//std::vector<glm::vec3> SphericalHarmonics::computeLightSHCoeff(const Image<float>* env, unsigned int order)
//{
//	int width = env->width;
//	int height = env->height;
//	sh::DefaultImage sh_envmap(width, height);
//
//	for (int y = 0; y < height; y++)
//	{
//		for (int x = 0; x < width; x++)
//		{
//			float r = (*env)(x, y, 0);
//			float g = (*env)(x, y, 1);
//			float b = (*env)(x, y, 2);
//			Eigen::Array3f color(r, g, b);
//			sh_envmap.SetPixel(x, y, color);
//		}
//	}
//	std::unique_ptr<std::vector<Eigen::Array3f>> sh = ProjectEnvironment(order, sh_envmap);
//
//	std::vector<glm::vec3> coeff;
//	for (int i = 0; i < sh->size(); i++)
//	{
//		Eigen::Array3f vec = (*sh)[i];
//		coeff.push_back(glm::vec3(vec[0], vec[1], vec[2]));
//	}
//
//	return coeff;
//}
//
//RTCRay _getRTCRay(const glm::vec3& o, const glm::vec3& d) {
//	RTCRay ray;
//	ray.org_x = o[0];
//	ray.org_y = o[1];
//	ray.org_z = o[2];
//
//	ray.dir_x = d[0];
//	ray.dir_y = d[1];
//	ray.dir_z = d[2];
//
//	ray.tnear = FLT_EPSILON;
//	ray.tfar = FLT_MAX;
//	ray.flags = 0;
//
//	return ray;
//}
//
//
//RTCScene initEmbree(const Scene * pScene,RTCDevice device)
//{	
//	RTCScene scene = rtcNewScene(device);
//	if (rtcGetDeviceError(device) != RTC_ERROR_NONE) {
//		std::cerr << "Failed to create scene.\n";
//	}
//	
//	Object* obj = nullptr;
//	for (unsigned int objIdx = 0; objIdx < pScene->getObjectSize(); objIdx++)
//	{
//		obj = pScene->getObject(objIdx);
//		for (int meshIdx = 0; meshIdx < obj->getMeshSize(); meshIdx++)
//		{
//			Mesh* mesh = obj->getMesh(meshIdx);
//			RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
//	
//			glm::vec3* vertex = (glm::vec3*)rtcSetNewGeometryBuffer(geom, RTC_BUFFER_TYPE_VERTEX,
//				0, RTC_FORMAT_FLOAT3, sizeof(glm::vec3),
//				mesh->getVertexNums());
//			memcpy(vertex, mesh->mPositions.data(), mesh->getVertexNums() * sizeof(glm::vec3));
//	
//			glm::ivec3* index = (glm::ivec3*)rtcSetNewGeometryBuffer(geom, RTC_BUFFER_TYPE_INDEX,
//				0, RTC_FORMAT_UINT3, sizeof(unsigned int),
//				mesh->getIndicesNums());
//			memcpy(index, mesh->mIndices.data(), mesh->getIndicesNums() * sizeof(unsigned int));
//	
//			rtcCommitGeometry(geom);
//			rtcAttachGeometry(scene, geom);
//			rtcReleaseGeometry(geom);
//		}
//	}
//	rtcCommitScene(scene);
//	return scene;
//}
//
//float _halton(int idx, int base)
//{
//	float f = 1.0f;
//	float r = 0.f;
//	while (idx > 0)
//	{
//		f = f / base;
//		r = r + f * (idx%base);
//		idx = idx / base;
//	}
//	return r;
//}
//
//struct SHSample
//{
//	glm::vec3 dir;
//	std::vector<float> coeff;
//};
//
//static std::vector<SHSample> precomputed_samples;
//
//void initSampleLights(int order)
//{
//	const int nSamples = 128;
//	precomputed_samples.resize(nSamples);
//	for (int i = 1; i <= nSamples; i++)
//	{
//		float u = _halton(i, 2);
//		float v = _halton(i, 3);
//		
//		double phi = 2 * M_PI * u;
//		double theta = acos(1 - 2 * v);
//		
//		double r = sin(theta);
//		
//		//TODO: why reflect?
//		precomputed_samples[i-1].dir = -glm::vec3(r * cos(phi), cos(theta), r * sin(phi));
//		
//		for (int l = 0; l <= order; ++l)
//		{
//			for (int m = -l; m <= l; ++m)
//			{
//				precomputed_samples[i-1].coeff.push_back(sh::EvalSH(l, m, phi, theta));
//			}
//		}
//	}
//
//}
//
//void computeVertexSHCoeff(RTCScene scene, glm::vec3 normal, glm::vec3 pos,
//	const int order, std::vector<float> &coeff)
//{
//	//// for test?
//	//coeff[0] = 1.f;
//	//return;
//
//	RTCIntersectContext context;
//	rtcInitIntersectContext(&context);
//
//	int nSamples = precomputed_samples.size();
//	float sampleWeight = M_PI * 4 / nSamples;
//
//
//	for (int k = 0; k < precomputed_samples[0].coeff.size(); ++k)
//	{
//		float data = 0;
//		for (int i = 0; i < nSamples; ++i)
//		{
//			glm::vec3 sampleDir = precomputed_samples[i].dir;
//			float nDotL = glm::dot(glm::normalize(sampleDir), glm::normalize(normal));
//			if (nDotL > 0)
//			{
//				float w = sampleWeight * nDotL;
//
//				RTCRay sample_ray = _getRTCRay(pos + 0.001f * normal, sampleDir);
//				rtcOccluded1(scene, &context, &sample_ray);
//
//				if (sample_ray.tfar > 0)
//				{
//					coeff[k] += precomputed_samples[i].coeff[k] * w;
//				}
//			}
//		}
//		coeff.push_back(data);
//	}
//}
//
//vector<vector<vector<float>>> SphericalHarmonics::computeSceneSHTrans(const Scene* scene, unsigned int order)
//{
//	if (precomputed_samples.empty())
//	{
//		initSampleLights(order);
//	}
//	RTCDevice device = rtcNewDevice(NULL);
//
//	if (rtcGetDeviceError(device) != RTC_ERROR_NONE) {
//		std::cerr << "Failed to create device.\n";
//	}
//	RTCScene rtscene = initEmbree(scene, device);
//
//	vector<vector<vector<float>>> trans(scene->getObjectSize());
//
//	for (int objId = 0; objId < scene->getObjectSize(); objId++)
//	{
//		Object* obj = scene->getObject(objId);
//		trans[objId].resize(obj->getMeshSize());
//		for (int meshId = 0; meshId < obj->getMeshSize(); meshId++)
//		{
//			Mesh* mesh = obj->getMesh(meshId);
//			for (int i = 0; i < mesh->getVertexNums(); i++)
//			{
//				glm::vec3 pos = mesh->mPositions[i];
//				glm::vec3 normal = mesh->mNormals[i];
//				computeVertexSHCoeff(rtscene, normal, pos, order, trans[objId][meshId]);
//			}
//		}
//	}
//	rtcReleaseScene(rtscene);
//	rtcReleaseDevice(device);
//
//	return trans;
//};