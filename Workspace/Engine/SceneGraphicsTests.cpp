#include "SceneGraphicsTests.h"

#include "Texture2D.h"
#include "SysTransformCalculator.h"
#include "TransformComponents.h"

#include "SysPhysicsVelocityIntegration.h"
#include "PhysicsComponents.h"


#include "SysCameraFreemode.h"
#include "SysRenderingPipeline.h"
#include "SysRenderingMeshRenderers.h"
#include "SysRenderingLighting.h"
#include "SysRenderingSkybox.h"
#include "SysRenderingFog.h"

#include "TextureCube.h"
#include "MaterialPbr.h"
#include "CmpCamera.h"
#include "CmpCameraFreecam.h"
#include "CmpLightPoint.h"
#include "CmpLightAmbient.h"
#include "CmpLightDirectional.h"
#include "CmpMeshRenderer.h"

SceneGraphicsTests::SceneGraphicsTests() {
	//Register systems.
	GetOrAddSystem<SysTransformCalculator>();
	GetOrAddSystem<SysPhysicsVelocityIntegration>();
	
	GetOrAddSystem<SysCameraFreemode>();
	GetOrAddSystem<SysRenderingPipeline>();
	GetOrAddSystem<SysRenderingMeshRenderers>();
	GetOrAddSystem<SysRenderingLighting>();
	GetOrAddSystem<SysRenderingFog>();
	GetOrAddSystem<SysRenderingSkybox>();

	//Load meshes.
	std::shared_ptr<Mesh> sphereMesh = std::make_shared<Mesh>("Assets/meshes/sphere.obj");
	std::shared_ptr<Mesh> planeMesh = std::make_shared<Mesh>("Assets/meshes/plane_100x100.obj");
	std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>("Assets/meshes/cubeChamfered.obj");
	//Load shaders.
	std::shared_ptr<Shader> opaquePbr = std::make_shared<Shader>(std::initializer_list{
		ShaderSource::FromFile("Assets/shaders/opaque_pbr_vert.glsl", ShaderStage::Vert),
		ShaderSource::FromFile("Assets/shaders/opaque_pbr_frag.glsl", ShaderStage::Frag) });
	std::shared_ptr<Shader> opaquePbrTess = std::make_shared<Shader>(std::initializer_list{
		ShaderSource::FromFile("Assets/shaders/opaque_pbr_tess_vert.glsl", ShaderStage::Vert),
		ShaderSource::FromFile("Assets/shaders/opaque_pbr_tess_frag.glsl", ShaderStage::Frag),
		ShaderSource::FromFile("Assets/shaders/opaque_pbr_tess_tess_ctrl.glsl", ShaderStage::TessCtrl),
		ShaderSource::FromFile("Assets/shaders/opaque_pbr_tess_tess_eval.glsl", ShaderStage::TessEval) });
	std::shared_ptr<Shader> opaqueUnlitReflectRefract = std::make_shared<Shader>(std::initializer_list{
		ShaderSource::FromFile("Assets/shaders/opaque_unlit_reflect_refract_vert.glsl", ShaderStage::Vert),
		ShaderSource::FromFile("Assets/shaders/opaque_unlit_reflect_refract_frag.glsl", ShaderStage::Frag) });
	std::shared_ptr<Shader> transparentUnlitReflect = std::make_shared<Shader>(std::initializer_list{
		ShaderSource::FromFile("Assets/shaders/transparent_unlit_reflect_vert.glsl", ShaderStage::Vert),
		ShaderSource::FromFile("Assets/shaders/transparent_unlit_reflect_frag.glsl", ShaderStage::Frag) });

	//Load materials.
	std::shared_ptr<Texture> skyboxTexture;
	{
		skyboxTexture = std::make_shared<TextureCube>(
			Bitmap("Assets/textures/torontoSkyBox/negx.jpg"),
			Bitmap("Assets/textures/torontoSkyBox/posx.jpg"),
			Bitmap("Assets/textures/torontoSkyBox/negy.jpg"),
			Bitmap("Assets/textures/torontoSkyBox/posy.jpg"),
			Bitmap("Assets/textures/torontoSkyBox/negz.jpg"),
			Bitmap("Assets/textures/torontoSkyBox/posz.jpg"));
		if (HasSystem<SysRenderingSkybox>()) {
			auto skyboxShader = std::make_shared<Shader>(std::initializer_list{
				ShaderSource::FromFile("Assets/shaders/skyboxes/skybox_frag.glsl", ShaderStage::Frag),
				ShaderSource::FromFile("Assets/shaders/skyboxes/skybox_vert.glsl", ShaderStage::Vert) });
			auto& skyboxRenderer = GetSystem<SysRenderingSkybox>();
			skyboxRenderer.SetSkyboxTexture(skyboxTexture);
			skyboxRenderer.SetSkyboxShader(skyboxShader);
		}
	}
	std::shared_ptr<Material> earthMaterial;
	{
		Bitmap roughness = Bitmap("Assets/textures/earth/roughness.png");
		Bitmap metalness = Bitmap("Assets/textures/earth/metalness.png");
		Bitmap orm = Bitmap(roughness.width, roughness.height);
		orm.evaluate([&roughness, &metalness](const int2& pos) { return Color32(255, roughness[pos].r, metalness[pos].r, 255); });
		earthMaterial = std::make_shared<MaterialPbr>(opaquePbr,
			std::make_shared<Texture2D>(Bitmap("Assets/textures/earth/albedo.png")),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/flat.png")),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/earth/emission.png")),
			std::make_shared<Texture2D>(orm),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/flat.png")),
			skyboxTexture,
			1.0f);
	}
	std::shared_ptr<Material> dirtMaterial;
	{
		Bitmap ambientOcclusion = Bitmap("Assets/textures/megascan_forest/pftlS0_8K_AO.png");
		Bitmap roughness = Bitmap("Assets/textures/megascan_forest/pftlS0_8K_AO.png");
		Bitmap orm = Bitmap(ambientOcclusion.width, ambientOcclusion.height);
		orm.evaluate([&ambientOcclusion, &roughness](const int2& pos) { return Color32(ambientOcclusion[pos].r, roughness[pos].r, 0, 255); });
		dirtMaterial = std::make_shared<MaterialPbr>(opaquePbrTess,
			std::make_shared<Texture2D>(Bitmap("Assets/textures/megascan_forest/pftlS0_8K_Albedo.png")),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/megascan_forest/pftlS0_8K_Normal.png")),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/black.png")),
			std::make_shared<Texture2D>(orm),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/megascan_forest/pftlS0_8K_Displacement.png")),
			skyboxTexture,
			0.2f);
	}
	std::shared_ptr<Material> whiteMaterial;
	{
		Bitmap orm = Bitmap(1, 1);
		orm.evaluate([](const int2& pos) { return Color32((uint8_t)255, 255 - 32, 0, 255); });
		whiteMaterial = std::make_shared<MaterialPbr>(opaquePbr,
			std::make_shared<Texture2D>(Bitmap("Assets/textures/white.png")),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/flat.png")),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/black.png")),
			std::make_shared<Texture2D>(orm),
			nullptr,
			skyboxTexture,
			0.0f);
	}
	std::shared_ptr<Material> mirrorMaterial;
	{
		Bitmap orm = Bitmap(1, 1);
		orm.evaluate([](const int2& pos) { return Color32((uint8_t)255, 0, 255, 255); });
		mirrorMaterial = std::make_shared<MaterialPbr>(opaquePbr,
			std::make_shared<Texture2D>(Bitmap("Assets/textures/white.png")),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/flat.png")),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/black.png")),
			std::make_shared<Texture2D>(orm),
			nullptr,
			skyboxTexture,
			0.0f);
	}
	std::shared_ptr<Material> reflectRefractMaterial;
	{
		Bitmap orm = Bitmap(1, 1);
		orm.evaluate([](const int2& pos) { return Color32((uint8_t)255, 0, 255, 255); });
		reflectRefractMaterial = std::make_shared<MaterialPbr>(opaqueUnlitReflectRefract,
			std::make_shared<Texture2D>(Bitmap("Assets/textures/white.png")),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/flat.png")),
			nullptr,
			std::make_shared<Texture2D>(orm),
			nullptr,
			skyboxTexture,
			0.0f);
	}
	std::shared_ptr<Material> transparentReflectMaterial;
	{
		Bitmap orm = Bitmap(1, 1);
		orm.evaluate([](const int2& pos) { return Color32((uint8_t)255, 0, 255, 255); });
		transparentReflectMaterial = std::make_shared<MaterialPbr>(transparentUnlitReflect,
			std::make_shared<Texture2D>(Bitmap("Assets/textures/white.png")),
			std::make_shared<Texture2D>(Bitmap("Assets/textures/flat.png")),
			nullptr,
			std::make_shared<Texture2D>(orm),
			nullptr,
			skyboxTexture,
			0.0f);
	}

	//Create camera.
	if (true) {
		auto camera = entities.create();
		entities.emplace<CmpTransformLocalToWorld>(camera);
		entities.emplace<CmpCamera>(camera, 1.0f, 100.0f, 45.0f * math::DegToRad<>, false, 1, 1);
		entities.emplace<CmpCameraFreecam>(camera);
		entities.emplace<CmpRotation>(camera);
		entities.emplace<CmpPosition>(camera, float3(2, 0, 10));
	}
	//Create plane.
	if (true) {
		auto earth = entities.create();
		entities.emplace<CmpMeshRendererOpaque>(earth, dirtMaterial, planeMesh);
		entities.emplace<CmpTransformLocalToWorld>(earth, float4x4::translate(float3(0.0f, -10.0f, -0.0f)) * float4x4::scale(float3(1.0f, 1.0f, 1.0f)));
		//entities.emplace<CmpPosition>(earth, float3(0.0f, -10.0f, 0.0f));
	}
	//Create earths.
	if (true) {
		for (size_t i = 0; i < 10; ++i) {
			auto earth = entities.create();
			entities.emplace<CmpMeshRendererOpaque>(earth, math::random(0.0f, 1.0f) >= 0.5f ? earthMaterial : dirtMaterial, sphereMesh);
			entities.emplace<CmpTransformLocalToWorld>(earth);
			entities.emplace<CmpPosition>(earth, math::random(float3(-15.0f, -5.0f, -15.0f), float3(10.0f)));
			entities.emplace<CmpRotation>(earth, quaternion::fromAxisAngle(math::random<float3>(-1.0f, 1.0f).normalized(), math::random(0.0f, 360.0f) * math::DegToRad<>));
			entities.emplace<CmpVelocityAngular>(earth, math::random<float3>(-1.0f, 1.0f).normalized() * math::random(0.0f, 90.0f) * math::DegToRad<>);
		}
	}
	//Create dirt cubes.
	if (true) {
		for (size_t i = 0; i < 10; ++i) {
			auto cube = entities.create();
			entities.emplace<CmpMeshRendererOpaque>(cube, dirtMaterial, cubeMesh);
			entities.emplace<CmpTransformLocalToWorld>(cube);
			entities.emplace<CmpPosition>(cube, math::random(float3(-15.0f, -5.0f, -15.0f), float3(10.0f)));
			//entities.emplace<CmpRotation>(cube, quaternion::fromAxisAngle(math::random<float3>(-1.0f, 1.0f).normalized(), math::random(0.0f, 360.0f) * math::DegToRad<>));
			//entities.emplace<CmpVelocityAngular>(cube, math::random<float3>(-1.0f, 1.0f).normalized() * math::random(0.0f, 90.0f) * math::DegToRad<>);
		}
	}
	//Create mirror cubes and spheres.
	if (true) {
		for (size_t i = 0; i < 10; ++i) {
			auto cube = entities.create();
			entities.emplace<CmpMeshRendererOpaque>(cube, mirrorMaterial, math::random(0.0f, 1.0f) >= 0.5f ? cubeMesh : sphereMesh);
			entities.emplace<CmpTransformLocalToWorld>(cube);
			entities.emplace<CmpPosition>(cube, math::random(float3(-15.0f, -5.0f, -15.0f), float3(10.0f)));
			entities.emplace<CmpRotation>(cube, quaternion::fromAxisAngle(math::random<float3>(-1.0f, 1.0f).normalized(), math::random(0.0f, 360.0f) * math::DegToRad<>));
			entities.emplace<CmpVelocityAngular>(cube, math::random<float3>(-1.0f, 1.0f).normalized() * math::random(0.0f, 90.0f) * math::DegToRad<>);
		}
	}
	//Create white cubes.
	if (true) {
		for (size_t i = 0; i < 10; ++i) {
			auto cube = entities.create();
			entities.emplace<CmpMeshRendererOpaque>(cube, whiteMaterial, math::random(0.0f, 1.0f) >= 0.5f ? cubeMesh : sphereMesh);
			entities.emplace<CmpTransformLocalToWorld>(cube);
			entities.emplace<CmpPosition>(cube, math::random(float3(-15.0f, -5.0f, -15.0f), float3(10.0f)));
			entities.emplace<CmpRotation>(cube, quaternion::fromAxisAngle(math::random<float3>(-1.0f, 1.0f).normalized(), math::random(0.0f, 360.0f) * math::DegToRad<>));
			entities.emplace<CmpVelocityAngular>(cube, math::random<float3>(-1.0f, 1.0f).normalized() * math::random(0.0f, 90.0f) * math::DegToRad<>);
		}
	}
	//Create reflection and refraction cubes and spheres.
	if (false) {
		for (size_t i = 0; i < 10; ++i) {
			auto cube = entities.create();
			entities.emplace<CmpMeshRendererOpaque>(cube, CmpMeshRendererOpaque(reflectRefractMaterial, math::random(0.0f, 1.0f) >= 0.5f ? cubeMesh : sphereMesh));
			entities.emplace<CmpTransformLocalToWorld>(cube);
			entities.emplace<CmpPosition>(cube, math::random(float3(-15.0f, -5.0f, -15.0f), float3(10.0f)));
			entities.emplace<CmpRotation>(cube, quaternion::fromAxisAngle(math::random<float3>(-1.0f, 1.0f).normalized(), math::random(0.0f, 360.0f) * math::DegToRad<>));
			entities.emplace<CmpVelocityAngular>(cube, math::random<float3>(-1.0f, 1.0f).normalized() * math::random(0.0f, 90.0f) * math::DegToRad<>);
		}
	}
	//Create transparent cubes and spheres.
	if (true) {
		for (size_t i = 0; i < 10; ++i) {
			auto cube = entities.create();
			entities.emplace<CmpMeshRendererTransparent>(cube, CmpMeshRendererTransparent(transparentReflectMaterial, math::random(0.0f, 1.0f) >= 0.5f ? cubeMesh : sphereMesh));
			entities.emplace<CmpTransformLocalToWorld>(cube);
			entities.emplace<CmpPosition>(cube, math::random(float3(-15.0f, -5.0f, -15.0f), float3(10.0f)));
			entities.emplace<CmpRotation>(cube, quaternion::fromAxisAngle(math::random<float3>(-1.0f, 1.0f).normalized(), math::random(0.0f, 360.0f) * math::DegToRad<>));
			entities.emplace<CmpVelocityAngular>(cube, math::random<float3>(-1.0f, 1.0f).normalized() * math::random(0.0f, 90.0f) * math::DegToRad<>);
		}
	}
	//Create ambient light.
	if (false) {
		auto light = entities.create();
		entities.emplace<CmpLightAmbient>(light, float3(0.05f, 0.05f, 0.05f));
	}
	//Create directional light.
	if (true) {
		auto light = entities.create();
		entities.emplace<CmpLightDirectional>(light, float3(238, 175, 97) / 255.0f);
		entities.emplace<CmpTransformLocalToWorld>(light, float4x4::rotate(quaternion::fromAxisAngle(float3(1.0f, 0.0f, 0.0f), -30.0f * math::DegToRad<>)));
	}
	//Create point lights
	if (true) {
		for (size_t i = 0; i < 15; ++i) {
			auto light = entities.create();
			entities.emplace<CmpMeshRendererOpaque>(light, whiteMaterial, sphereMesh);
			entities.emplace<CmpLightPoint>(light, math::random<float3>(0.3f, 1.0f), float2(1.0f, 15.0f));
			entities.emplace<CmpTransformLocalToWorld>(light);
			entities.emplace<CmpPosition>(light, math::random(float3(-25.0f, -10.0f, -25.0f), float3(25, 5, 25.0f)));
			entities.emplace<CmpScale>(light, 0.3f);
		}
	}
}
