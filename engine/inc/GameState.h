#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "GameObject.h"

#include <algorithm>
#include <string>
#include <vector>
#include "SDLGameObject.h"

namespace engine{

	class GameState{
	public:
		virtual void update();
		virtual void render();

		virtual bool onEnter() = 0;
		virtual bool onExit();

		virtual std::string getStateID() const = 0;

		virtual ~GameState() {}

		void addGameObject(GameObject* gameObject){
			m_gameObjects.push_back(gameObject);
		}

		void removeGameObject(GameObject* gameObject){
			std::vector<GameObject*>::iterator position = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
			if(position != m_gameObjects.end()){
				m_gameObjects.erase(position);
			}
		}
		bool can_update = true;

		void addShieldObject(SDLGameObject *obj){
			m_shieldObjects.push_back(obj);
		}

		std::vector<SDLGameObject*> getShieldObjects(){
			return m_shieldObjects;
		}

	protected:
		std::vector<GameObject*> m_gameObjects;
		std::vector<std::string> m_textureIDList;
		std::vector<SDLGameObject*> m_shieldObjects;
	};
}
#endif
