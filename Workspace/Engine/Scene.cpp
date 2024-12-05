#include "Scene.h"
#include <GL/glew.h>

Scene::Scene() : systems(), entities(), m_Time(0.0f) {

}
Scene::~Scene() {
	for (auto system : systems)
		delete system;
}

void Scene::Update(float timestep) {
	m_Time += timestep;
	SendMessage(EvtUpdate(m_Time, timestep));
}
void Scene::Render() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SendMessage(EvtRenderPre(m_Time));
	SendMessage(EvtRender(m_Time));
	SendMessage(EvtRenderPost(m_Time));
}
