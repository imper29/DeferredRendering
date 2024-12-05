#include "ScenePhysicsTests.h"

#include "Texture2D.h"
#include "SysTransformCalculator.h"
#include "TransformComponents.h"

#include "SysPhysicsGravity.h"
#include "SysPhysicsConstraintSolver.h"
#include "SysPhysicsCollisionCollector.h"
#include "SysPhysicsCollisionResolver.h"
#include "SysPhysicsVelocityIntegration.h"
#include "SysPhysicsDamping.h"
#include "PhysicsComponents.h"


#include "SysCameraFreemode.h"
#include "SysRenderingPipeline.h"
#include "SysRenderingMeshRenderers.h"
#include "SysRenderingLighting.h"
#include "SysRenderingSkybox.h"

#include "TextureCube.h"
#include "MaterialPbr.h"
#include "CmpCamera.h"
#include "CmpCameraFreecam.h"
#include "CmpLightPoint.h"
#include "CmpLightAmbient.h"
#include "CmpLightDirectional.h"
#include "CmpMeshRenderer.h"
#include "CmpConstraintSpring.h"

ScenePhysicsTests::ScenePhysicsTests() {
	//Register systems.
	GetOrAddSystem<SysPhysicsGravity>();
	GetOrAddSystem<SysTransformCalculator>();
	GetOrAddSystem<SysPhysicsConstraintSolver>();
	GetOrAddSystem<SysPhysicsCollisionCollector>();
	GetOrAddSystem<SysPhysicsCollisionResolver>();
	GetOrAddSystem<SysPhysicsVelocityIntegration>();
	GetOrAddSystem<SysPhysicsDamping>();

	GetOrAddSystem<SysCameraFreemode>();
	GetOrAddSystem<SysRenderingPipeline>();
	GetOrAddSystem<SysRenderingMeshRenderers>();
	GetOrAddSystem<SysRenderingLighting>();
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
			std::make_shared<Texture2D>(Bitmap("Assets/textures/black.png")),
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
	std::array<std::shared_ptr<Material>, 6> materials = {
		transparentReflectMaterial,
		earthMaterial,
		dirtMaterial,
		whiteMaterial,
		mirrorMaterial,
		reflectRefractMaterial,
	};

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
	if (false) {
		auto entity = entities.create();
		entities.emplace<CmpMeshRendererOpaque>(entity, dirtMaterial, planeMesh);
		entities.emplace<CmpTransformLocalToWorld>(entity, float4x4::translate(float3(0.0f, -10.0f, 0.0f)));
	}
	//Create spheres.
	for (int i = 0; i < materials.size(); ++i) {
		auto anchor = entities.create();
		entities.emplace<CmpMeshRendererOpaque>(anchor, whiteMaterial, cubeMesh);
		entities.emplace<CmpTransformLocalToWorld>(anchor, float4x4::translate(float3(5.0f * i, 10.0f, 0.0f)));
		auto entity = entities.create();
		if (materials[i] == transparentReflectMaterial)
			entities.emplace<CmpMeshRendererTransparent>(entity, materials[i], sphereMesh);
		else
			entities.emplace<CmpMeshRendererOpaque>(entity, materials[i], sphereMesh);
		entities.emplace<CmpTransformLocalToWorld>(entity);
		entities.emplace<CmpPosition>(entity, float3(5.0f * i, 5.0f, 0.0f) + math::random<float3>(-1.0f, 1.0f) + float3(0, math::random<>(-2.0f, 2.0f), 0.0f));
		entities.emplace<CmpRotation>(entity);
		entities.emplace<CmpVelocityAngular>(entity);
		entities.emplace<CmpVelocity>(entity);
		entities.emplace<CmpMass>(entity, 10.0f);
		entities.emplace<TagGravity>(entity);
		entities.emplace<CmpDampingAngular>(entity, 0.99f);
		entities.emplace<CmpDamping>(entity, 0.9f);
		auto spring = entities.create();
		entities.emplace<CmpConstraintSpring>(spring, anchor, entity, float3(), float3(0, 1, 0), 30.0f, 7.0f);
		auto anchorRenderer = entities.create();
		entities.emplace<CmpParent>(anchorRenderer, entity);
		entities.emplace<CmpMeshRendererOpaque>(anchorRenderer, whiteMaterial, sphereMesh);
		entities.emplace<CmpTransformLocalToWorld>(anchorRenderer);
		entities.emplace<CmpPosition>(anchorRenderer, entities.get<CmpConstraintSpring>(spring).bAnchor);
		entities.emplace<CmpScale>(anchorRenderer, 0.2f);
	}
	//Create ambient light.
	if (true) {
		auto light = entities.create();
		entities.emplace<CmpLightAmbient>(light, float3(0.05f, 0.05f, 0.05f));
	}
	//Create directional light.
	if (true) {
		auto light = entities.create();
		entities.emplace<CmpLightDirectional>(light, float3(238, 175, 97) / 255.0f);
		entities.emplace<CmpTransformLocalToWorld>(light, float4x4::rotate(quaternion::fromAxisAngle(float3(1.0f, 0.0f, 0.0f), -30.0f * math::DegToRad<>)));
	}
}
