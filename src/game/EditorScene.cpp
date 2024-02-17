#include "game/EditorScene.h"
#include "engine/GameEngine.h"


EditorScene::EditorScene(GameEngine* engine) :
	Scene(engine) {

}

void EditorScene::saveScene() {}
void EditorScene::sceneEditor() {}
void EditorScene::update(float dt){}
void EditorScene::sDoAction(const Action& action){}
void EditorScene::sDraggable(){}
void EditorScene::sRender(){}