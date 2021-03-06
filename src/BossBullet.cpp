#include "BossBullet.h"
#include "SDLGameObject.h"
#include "Vector2D.h"
#include "LoaderParams.h"
#include "Timer.h"
#include "Player.h"
#include "Game.h"
#include "InputHandler.h"
#include "Log.h"
#include "Enemy.h"
#include "Physics.h"
#include "GameOverState.h"
#include <iostream>
#include <cstdlib>
#include <ctime>


using namespace engine;

BossBullet::~BossBullet(){
	INFO("Boss Bullet destroyed")
}

BossBullet::BossBullet(Player *target) : SDLGameObject(){

	std::srand(std::time(0));

	setPlayer(target);
	timeToLive = 5000;
	m_active = true;

	m_textureID = "bullet" + std::to_string(1 + std::rand()%9);
}

void BossBullet::load(const LoaderParams* pParams){
	std::srand(std::time(0));
	m_velocity = Vector2D(0,0);
	
	m_textureID = "bullet" + std::to_string(1 + std::rand()%9);
	SDLGameObject::load(pParams);
}

double BossBullet::rotateTowards(Vector2D pPosition){
	Vector2D target = InputHandler::Instance().getMousePosition() - pPosition;
	target = target.norm();

	Vector2D horizontal(0,1);

	return Vector2D::angle(target, Vector2D(0, 1));
}
void BossBullet::load(Vector2D pVelocity, Vector2D pPosition){
	std::srand(std::time(0));
	double m_angle = 0;

	m_moveSpeed = 3;
	m_textureID = "bullet" + std::to_string(1 + std::rand()%9);
	LoaderParams* pParams = new LoaderParams(pPosition.getX(), pPosition.getY(), 48, 48, m_textureID, 0, 0, 0, m_angle);
	SDLGameObject::load(pParams);

	m_currentFrame = 0;
	bornTime = Timer::Instance().step();
	m_velocity = pVelocity.norm() * m_moveSpeed;
	//m_velocity = Vector2D(0.5, 0.5);
	
	Vector2D norm = pVelocity.norm();
	normal = Vector2D(norm.getY(), -norm.getX());
	originTime = Timer::Instance().step();
}

void BossBullet::draw(){
	SDLGameObject::draw();
}

void BossBullet::update(){
	Vector2D tmp = m_velocity;
	
	double sign = sin((Timer::Instance().step() - originTime)/125);
	sign /= fabs(sign);

	sign *= 1;
	m_velocity += Vector2D(normal.getX()*sign, normal.getY()*sign);

	m_position += m_velocity;
	m_velocity = tmp;

	

	if(Timer::Instance().step() >= bornTime + timeToLive){
		m_active = false;
		Game::Instance().getStateMachine()->currentState()->removeGameObject(this);
	}

	checkCollision();

}

void BossBullet::checkCollision(){
	if(m_active){
		Vector2D pos = m_player->getPosition();
		Vector2D thisPos = getPosition();
		
		if(Physics::Instance().checkCollision(dynamic_cast<SDLGameObject*>(m_player), dynamic_cast<SDLGameObject*>(this))){
			m_active = false;
			Game::Instance().getStateMachine()->currentState()->removeGameObject(this);
			//INFO("Bullet collided");
			//INFO("PLAYER LOST THE GAME");
			if(!m_player->getShieldActive())
				m_player->setLife((m_player->getLife()) - 1);
			else if(m_player->getShieldActive())
				m_player->setShieldHits();
			//Game::Instance().getStateMachine()->changeState(new GameOverState());
		}
	}
}

void BossBullet::clean(){
	SDLGameObject::clean();
}

