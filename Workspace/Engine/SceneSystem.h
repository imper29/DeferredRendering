#pragma once
class Scene;
class SceneSystem
{
public:
	SceneSystem(Scene& scene);
	virtual ~SceneSystem();

	Scene& scene;
};
