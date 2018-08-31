
#ifndef ARK2D_DEFAULTGAME_INGAMESTATE_H_
#define ARK2D_DEFAULTGAME_INGAMESTATE_H_

#include <ARK2D/Core/State/GameState.h>
#include <ARK2D/Util/CameraShake.h>

#include "../objects/EightWayMovementComponent.h"
#include "../objects/Object.h"
#include "../objects/Player.h"
#include "../objects/Bullet.h"
#include "../objects/ScrollObject.h"
#include "../objects/MyParticle.h"
#include "../objects/Explosion.h"
#include "../objects/Enemy.h"

class DefaultGame;

class Object;
class BlankObject;
class Player;
class Pickup;
class Explosion;
class MyParticle;
class Bullet;


class Collectable : public ScrollObject {
	public:
		static Image* s_imageRing;
		static Image* s_imageFlower1;
		static Image* s_imageFlower2;
		static Image* s_imageFlower3;
		static AnimationFrameList* s_animSparkle;
		static const unsigned int TYPE_RING = 0;
		static const unsigned int TYPE_FLOWER = 1;
	public:
		Animation* m_animation;
		Animation* m_animationSparkle;
		unsigned int m_type;
		Collectable();
		void reset();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
};

class Obstacle : public ScrollObject {
	public:
		static const unsigned int TYPE_ROCK = 0;
		static const unsigned int TYPE_BUSH = 1;
		static const unsigned int TYPE_TREE = 2;
	public:
		static Image* s_imageRock;
		static Image* s_imageBush;
		static Image* s_imageTree;
		Image* m_image;
		unsigned int m_type;
		Obstacle();
		void die();
		virtual void render(GameContainer* container, Renderer* r);
};


class Floor : public ScrollObject {
	public:
		float depth;
	public:
		Floor();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
};

class SpawnEnemyEvent {
	public:
		int enemyType;
		float delay;
		float stopAt;
};

class Wave {
	public:
		float m_spawnObstacleDuration;
		float m_spawnCollectableDuration;
		float speedMultiplier;
		float m_farScale;
};

class InGameState : public GameState {
	public:

		static float generalSpeed;
		Player* m_player;

		float m_floorMinY;
		float m_floorMaxY;
		float m_floorY;
		float m_floorYTarget;
		//int m_floorSegments;
		Pool<ScrollObject*>* m_scrollObjects;
		Pool<Collectable*>* m_collectables;
		Pool<Obstacle*>* m_obstacles;
		Pool<Bullet*>* m_bullets;
		Pool<MyParticle*>* m_particles;
		Pool<Explosion*>* m_explosions;
		Pool<Floor*>* m_floors;
		Pool<Enemy*>* m_enemies;
		vector<SpawnEnemyEvent> m_spawnEnemies;

		Rectangle* m_farBounds;
		float m_farScale;
		float m_farScaleInverse;
		float m_farDistance;
		
		Image* m_skyImage;
		Image* m_skyCloudImage;
		Image* m_floorImage;
		Color* m_skyColor;
		Color* m_floorColor;
		

		Image* m_skyImageNORMAL;
		Image* m_skyCloudImageNORMAL;
		Image* m_floorImageNORMAL;
		Color* m_skyColorNORMAL;
		Color* m_floorColorNORMAL;

		Image* m_skyImageSUNSET;
		Image* m_skyCloudImageSUNSET;
		Image* m_floorImageSUNSET;
		Color* m_skyColorSUNSET;
		Color* m_floorColorSUNSET;
		
		

		Rectangle* m_nearBounds;

		float m_spawnObstacleTimer;
		float m_spawnObstacleDuration;

		float m_spawnFloorTimer;
		float m_spawnFloorDuration;

		float m_spawnSideItemTimer;
		float m_spawnSideItemDuration;

		float m_spawnCollectableTimer;
		float m_spawnCollectableDuration;

		CameraShake* m_cameraShake;
		signed int m_cameraCenterX;
		signed int m_cameraCenterY;
		signed int m_cameraTargetCenterX;
		signed int m_cameraTargetCenterY;

		float m_cameraZoom;
		float m_cameraRotate;
		float m_cameraRotateWorld;
		Timer* m_cameraZoomTimer;
		Timer* m_cameraRotateTimer;

		float m_barrelRollTimer;
		float m_barrelRollDuration;
		bool m_barrelRollClockwise;

		int m_pollenCount;
		float speedMultiplier;

		int m_waveNumber;
		float m_waveChangeTimer;
		float m_waveChangeDuration;

		float m_waveTimer;
		float m_waveDuration;

		float m_deathTimer;
		float m_deathDuration;

		float m_flashTimer;
		float m_flashDuration;

		Vector2<float> m_lastEnemySpawnedAt;

		InGameState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		void startWave(int wave);
		void doCollisions();

		Obstacle* spawnObstacle();
		Enemy* spawnEnemy(int type);

		unsigned int id();
		void reset();
		void start();
		void init(GameContainer* container, StateBasedGame* game);
		
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void updateBackground(GameContainer* container, StateBasedGame* game, GameTimer* timer);

		void render(GameContainer* container, StateBasedGame* game, Renderer* r);
		void renderBackground(GameContainer* container, StateBasedGame* game, Renderer* r);

		void startExplosion(float px, float py, float z, float speed);

		void updateCameraVars(float m);
		void startGameCamera(float scaleX, float scaleY);
		void endGameCamera();
		float getCurrentCameraX() { return m_cameraCenterX + m_cameraShake->getXOffset(); }
		float getCurrentCameraY() { return m_cameraCenterY + m_cameraShake->getYOffset(); }

		Vector2<float> getPlayerAveragePosition();

		virtual ~InGameState();
};

#endif /* ARK2D_DEFAULTGAME_INGAMESTATE_H_ */
