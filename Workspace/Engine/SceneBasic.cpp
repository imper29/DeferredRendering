#include "SceneBasic.h"
#include "SysTransformCalculator.h"

#include "SysPhysicsGravity.h"
#include "SysPhysicsDamping.h"
#include "SysPhysicsCollisionCollector.h"
#include "SysPhysicsCollisionResolver.h"
#include "SysPhysicsVelocityIntegration.h"

#include "SysCameraFreemode.h"
#include "SysRenderingPipeline.h"
#include "SysRenderingMeshRenderers.h"
#include "SysRenderingLighting.h"

SceneBasic::SceneBasic() {
	GetOrAddSystem<SysTransformCalculator>();

	GetOrAddSystem<SysPhysicsGravity>(float3(0, -5, 0));
	GetOrAddSystem<SysPhysicsDamping>();
	GetOrAddSystem<SysPhysicsCollisionCollector>();
	GetOrAddSystem<SysPhysicsCollisionResolver>();
	GetOrAddSystem<SysPhysicsVelocityIntegration>();

	GetOrAddSystem<SysCameraFreemode>();
	GetOrAddSystem<SysRenderingPipeline>();
	GetOrAddSystem<SysRenderingMeshRenderers>();
	//GetOrAddSystem<SysRenderingLighting>();
}
