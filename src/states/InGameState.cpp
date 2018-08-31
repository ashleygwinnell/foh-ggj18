
#include "InGameState.h"
#include <ARK2D/Core/Geometry/Rectangle.h>
#include <ARK2D/Util/CameraShake.h>
#include "../DefaultGame.h"

#include <ARK2D/Core/State/Transition/FadeToColourTransition.h>
#include <ARK2D/Core/State/Transition/FadeFromColourTransition.h>


Image* Collectable::s_imageRing = NULL;
Image* Collectable::s_imageFlower1 = NULL;
Image* Collectable::s_imageFlower2 = NULL;
Image* Collectable::s_imageFlower3 = NULL;
AnimationFrameList* Collectable::s_animSparkle = NULL;
Collectable::Collectable():ScrollObject() {
    DefaultGame* dg = DefaultGame::getInstance();
	if (s_imageRing == NULL) {
		s_imageRing = SpriteSheetStore::getImage("sprites/object-honeyring.png");
		s_imageFlower1 = SpriteSheetStore::getImage("sprites/object-flower1.png");
		s_imageFlower2 = SpriteSheetStore::getImage("sprites/object-flower2.png");
		s_imageFlower3 = SpriteSheetStore::getImage("sprites/object-flower3.png");

		s_animSparkle = AnimationFrameList::create("sprites/object-sparkle*.png", 1, 3, dg->spritesheet, dg->desc);
	}
	m_animation = new Animation();
	m_animationSparkle = new Animation();
	m_animationSparkle->setFrameTime(0.2f);
	m_animationSparkle->addFrames(s_animSparkle);
}
void Collectable::reset() {
	m_animation->clear();
	sizex = 40;
	sizey = 40;
}
void Collectable::update(GameContainer* container, GameTimer* timer) {
	ScrollObject::update(container, timer);
	m_animation->update(timer);
	m_animationSparkle->update(timer);
}
void Collectable::render(GameContainer* container, Renderer* r) {
	r->setDrawColor(Color::white);
	//ScrollObject::render(container, r);
	

	if (m_type == TYPE_FLOWER) {
		Image* img = m_animation->getCurrentFrame();
		img->setAlpha(1.0f);
		renderImage(container, r, img);

		Image* img2 = m_animationSparkle->getCurrentFrame();
		renderImage(container, r, img2, -0.13f, -0.04);
	}
	else if (m_type == TYPE_RING) {
		Image* img = m_animation->getCurrentFrame();
		img->setAlpha(1.0f);
		img->setRotation(0);
		renderImage(container, r, img, 0.12f);
	}
}

Image* Obstacle::s_imageRock = NULL;
Image* Obstacle::s_imageBush = NULL;
Image* Obstacle::s_imageTree = NULL;
Obstacle::Obstacle():ScrollObject() {
	if (s_imageRock == NULL) {
		s_imageRock = SpriteSheetStore::getImage("sprites/object-rock.png");
		s_imageBush = SpriteSheetStore::getImage("sprites/object-bush.png");
		s_imageTree = SpriteSheetStore::getImage("sprites/object-tree.png");
	}
}
void Obstacle::die(){
    InGameState* igs = DefaultGame::getInstance()->stateInGame;
	igs->startExplosion(percentageX, percentageY, z, speed);
	igs->m_cameraShake->setSpeedMultiplier(2, 0.75f);
	igs->m_cameraShake->start(2, 5.0f);
}
void Obstacle::render(GameContainer* container, Renderer* r) {
	r->setDrawColor(Color::white);
	renderImage(container, r, m_image);
}



Floor::Floor():
	ScrollObject()
	{
    depth = InGameState::generalSpeed/10.0f;
	m_bounds->asRectangle()->setWidth(9999);

}
void Floor::update(GameContainer* container, GameTimer* timer) {
	ScrollObject::update(container, timer);
	setPendingRemoval(false);

	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	if ((speed > 0 && z+depth < 0.5f) || (speed < 0 && z+depth > igs->m_farDistance)) {
		setPendingRemoval(true);
	}
}
void Floor::render(GameContainer* container, Renderer* r) 
{
    InGameState* igs = DefaultGame::getInstance()->stateInGame;
    float backy = Easing::easebetween(Easing::QUARTIC_IN, igs->m_farDistance - z - depth, starty, endy, igs->m_farDistance);
	float actualy = Easing::easebetween(Easing::QUARTIC_IN, igs->m_farDistance - z, starty, endy, igs->m_farDistance);
	
	r->setDrawColor("#004739", 0.3f);
	r->fillRect(igs->m_nearBounds->getMinX(), backy, igs->m_nearBounds->getWidth(), actualy-backy);

}




float InGameState::generalSpeed = 20.0f;

InGameState::InGameState():
	GameState(),
	m_player(NULL),

	m_floorMinY(0),
	m_floorMaxY(0),
	m_floorY(0),
	m_floorYTarget(0),
	
	m_scrollObjects(NULL),
	m_collectables(NULL),
	m_obstacles(NULL),
	m_bullets(NULL),
	m_particles(NULL),
	m_explosions(NULL),
	m_floors(NULL),
	m_enemies(NULL),
	m_spawnEnemies(),

	m_farBounds(NULL),
	m_farScale(0),
	m_farScaleInverse(0),
	m_farDistance(0),
	
	m_skyImage(NULL),
	m_skyCloudImage(NULL),
	m_floorImage(NULL),
	m_skyColor(NULL),
	m_floorColor(NULL),
	

	m_skyImageNORMAL(NULL),
	m_skyCloudImageNORMAL(NULL),
	m_floorImageNORMAL(NULL),
	m_skyColorNORMAL(NULL),
	m_floorColorNORMAL(NULL),

	m_skyImageSUNSET(NULL),
	m_skyCloudImageSUNSET(NULL),
	m_floorImageSUNSET(NULL),
	m_skyColorSUNSET(NULL),
	m_floorColorSUNSET(NULL),
	
	m_nearBounds(NULL),

	m_spawnObstacleTimer(0),
	m_spawnObstacleDuration(0),

	m_spawnFloorTimer(0),
	m_spawnFloorDuration(0),

	m_spawnSideItemTimer(0),
	m_spawnSideItemDuration(0),

	m_spawnCollectableTimer(0),
	m_spawnCollectableDuration(0),

	m_cameraShake(NULL),
	m_cameraCenterX(0),
	m_cameraCenterY(0),
	m_cameraTargetCenterX(0),
	m_cameraTargetCenterY(0),

	m_cameraZoom(0),
	m_cameraRotate(0),
	m_cameraRotateWorld(0),
	m_cameraZoomTimer(NULL),
	m_cameraRotateTimer(NULL),

	m_barrelRollTimer(0),
	m_barrelRollDuration(0),
	m_barrelRollClockwise(0),

	m_pollenCount(0),
	speedMultiplier(0),

	m_waveNumber(0),
	m_waveChangeTimer(0),
	m_waveChangeDuration(0),

	m_waveTimer(0),
	m_waveDuration(0),

	m_deathTimer(0),
	m_deathDuration(0),

	m_flashTimer(0),
	m_flashDuration(0),

	m_lastEnemySpawnedAt(0,0)
	{

}

unsigned int InGameState::id() {
	return StatesList::STATE_INGAME;
}

void InGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}

void InGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void InGameState::init(GameContainer* container, StateBasedGame* game) {
	m_floorMinY = container->getHeight()*0.35;
	m_floorMaxY = container->getHeight()*0.80;

	m_floorYTarget = m_floorMaxY;
	m_floorY = m_floorYTarget;
//    /m_floorSegments = 8;

	m_skyImageNORMAL = SpriteSheetStore::getImage("sprites/bg-sky.png");
	m_skyCloudImageNORMAL = SpriteSheetStore::getImage("sprites/bg-cloud.png");
	m_floorImageNORMAL = SpriteSheetStore::getImage("sprites/bg-ground.png");
	m_skyImageNORMAL->scale(3.0, 1.0f);

	m_skyImageSUNSET = SpriteSheetStore::getImage("sprites/bg-sky-sunset.png");
	m_skyCloudImageSUNSET = SpriteSheetStore::getImage("sprites/bg-cloud-sunset.png");
	m_floorImageSUNSET = SpriteSheetStore::getImage("sprites/bg-ground-sunset.png");
	m_skyImageSUNSET->scale(3.0, 1.0f);

	m_skyColorNORMAL = new Color("#0096dc");
	m_skyColorSUNSET = new Color("#ff9057");
	
	m_floorColorNORMAL = new Color("#396e35");
	m_floorColorSUNSET = new Color("#66752e");

	m_skyImage = m_skyImageNORMAL;
	m_skyCloudImage = m_skyCloudImageNORMAL;
	m_floorImage = m_floorImageNORMAL;
	m_skyColor = m_skyColorNORMAL;
	m_floorColor = m_floorColorNORMAL;
	

	m_scrollObjects = new Pool<ScrollObject*>();
	m_collectables = new Pool<Collectable*>();
	m_obstacles = new Pool<Obstacle*>();
	m_bullets = new Pool<Bullet*>();
	m_particles = new Pool<MyParticle*>();
	m_explosions = new Pool<Explosion*>();
	m_floors = new Pool<Floor*>();
	m_enemies = new Pool<Enemy*>();

	m_nearBounds = new Rectangle(0.0f, 0.0f, (398+200), 224);
	m_nearBounds->setLocationByAlignment(container->getWidth() * 0.5f, container->getHeight(), Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM);

	m_farScale = 1.0 / 20.0f;
	m_farScaleInverse = 20.0f;
	m_farBounds = new Rectangle(0.0f, 0.0f, (398+1600) / m_farScaleInverse, 224 / m_farScaleInverse);
	m_farBounds->setLocationByAlignment(container->getWidth() * 0.5f, m_floorYTarget, Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM);
	
	
	m_farDistance = 80.0f;




	m_player = new Player();
	
	

	//
	m_cameraShake = new CameraShake(game, 3);

	

	//m_cameraShake->setSpeedMultiplier(CAMERASHAKE_LAYER_RESIZING, 1.0f);
	//m_cameraShake->setSpeedMultiplier(CAMERASHAKE_LAYER_UNSTABLE, 1.0f);
	//m_cameraShake->setSpeedMultiplier(CAMERASHAKE_LAYER_PLAYER_FIRING, 1.5f);

	//ARK::Core::Geometry::Rectangle* m_cameraRect;
	// float m_cameraX; 
	// float m_cameraY; 
	// signed int m_cameraTargetX;
	// signed int m_cameraTargetY;

	// float m_cameraZoom;
	// float m_cameraRotate;
	// Timer* m_cameraZoomTimer;
	// Timer* m_cameraRotateTimer;
	// m_cameraX = 0; //(m_player->m_bounds->getCenterX() * -1)/2;
	// m_cameraY = 0; //(m_player->m_bounds->getCenterY() * -1)/2;
	// m_cameraTargetX = m_cameraX;
	// m_cameraTargetY = m_cameraY;

	m_cameraZoom = 1.0f;
	m_cameraRotate = 0.0f;
	m_cameraRotateWorld = 0.0f;
	m_cameraZoomTimer = new Timer(0.5f);
	m_cameraRotateTimer = new Timer(0.5f);

	

	reset();
}

void InGameState::reset() {
    GameContainer* container = ARK2D::getContainer();
    Input* in = ARK2D::getInput();

    generalSpeed = 20.0f;
    
    DefaultGame* dg = DefaultGame::getInstance();
    dg->m_musicGame->setVolume(speedMultiplier);

	m_player->m_bounds->setLocationByCenter(m_nearBounds->getCenterX(), m_nearBounds->getCenterY());
	m_player->reset();


	m_cameraCenterX = container->getWidth() * 0.5f;
	m_cameraCenterY = container->getHeight() * 0.5f;
	m_cameraTargetCenterX = m_cameraCenterX;
	m_cameraTargetCenterY = m_cameraCenterY;

	m_obstacles->reset();
	m_enemies->reset();
	m_bullets->reset();
	m_explosions->reset();
	m_collectables->reset();
	m_spawnEnemies.clear();

	m_pollenCount = 0;
	speedMultiplier = 1.0f;

	m_spawnObstacleTimer = 0.0f;
	m_spawnObstacleDuration = 0.4f;

	m_spawnFloorTimer = 0.0f;
	m_spawnFloorDuration = 0.2f;

	m_spawnSideItemTimer = 0.0f;
	m_spawnSideItemDuration = 0.25f;;

	m_spawnCollectableTimer = 0.0f;
	m_spawnCollectableDuration = 1.5f;

	m_barrelRollTimer = 0.0f;
	m_barrelRollDuration = 1.0f;

	m_waveNumber = 0;
	m_waveChangeTimer = 0.01f;
	m_waveChangeDuration = 2.0f;

	m_deathTimer = 0.0f;
	m_deathDuration = 2.0f;

	m_flashTimer = 0.01f;
	m_flashDuration = 1.0f;;

	m_lastEnemySpawnedAt.set(0.5, 0.5);
}

void InGameState::start() {
	reset();

	startWave(m_waveNumber);

	DefaultGame* dg = DefaultGame::getInstance();
	dg->m_voStart->at(MathUtil::randBetween(0, dg->m_voStart->size()))->play();
}
void InGameState::startWave(int num) {
	// flying dudes
	int numFlying = std::min<int>(num+1, 10);
	for(int i = 0; i <= numFlying; i++) {
		SpawnEnemyEvent ev;
		ev.enemyType = Enemy::TYPE_CROW;
		ev.delay = m_waveChangeDuration + 1 + (i * 2.0f);
		ev.stopAt = MathUtil::randBetweenf(10.0f,15.0f);
		//if (MathUtil::randBetweenf(0.0f, 1.0f) < 0.2f) {
		//	ev.stopAt = -100;
		//}
		m_spawnEnemies.push_back(ev);
	}

	if (num % 2 == 0) {
		m_skyImage = m_skyImageNORMAL;
		m_skyCloudImage = m_skyCloudImageNORMAL;
		m_floorImage = m_floorImageNORMAL;
		m_skyColor = m_skyColorNORMAL;
		m_floorColor = m_floorColorNORMAL;

		m_flashTimer  = 0.01f;
	} else {
		m_skyImage = m_skyImageSUNSET;
		m_skyCloudImage = m_skyCloudImageSUNSET;
		m_floorImage = m_floorImageSUNSET;

		m_skyColor = m_skyColorSUNSET;
		m_floorColor = m_floorColorSUNSET;

		m_flashTimer  = 0.01f;
	}
}

Enemy* InGameState::spawnEnemy(int type) {
	int iterateCount = 0; 
	float mindist = 0.2f;
	bool blah = false;
	float spawnX;
	float spawnY;
	do {
		spawnX = MathUtil::randBetweenf(0.3f, 0.7f);
		spawnY = MathUtil::randBetweenf(0.3f, 0.6f);
		blah = false;

		PoolIterator<Enemy*> it = m_enemies->newiteratorref();
		while (it.hasNext()) {
			Enemy* e = it.next();
			if (e->isPendingRemoval()) { continue; }
			float dist = MathUtil::distance(spawnX, spawnY, e->percentageX, e->percentageY);
			if (dist < mindist) {
				blah = true;
				break;
			}
		}
		iterateCount++;
	} 
	while (blah && iterateCount < 10);
	// MathUtil::distance(spawnX, spawnY, m_lastEnemySpawnedAt.getX(), m_lastEnemySpawnedAt.getY()) <= 0.15f

	Enemy* e = m_enemies->get();
	e->reset();
	e->percentageX = spawnX;
	e->percentageY = spawnY;
	e->recalcPositions();
	e->speed = generalSpeed;
	e->z = m_farDistance;
	e->m_fireDuration = Easing::easebetween(Easing::LINEAR, m_waveNumber, 3.0f, 1.0f, 5);
	e->start(type);

	if (type == Enemy::TYPE_CROW) {
		DefaultGame::getInstance()->m_sfxCrowAppear->play();
	}

	m_lastEnemySpawnedAt.set(spawnX, spawnY);
	return e;
}

Obstacle* InGameState::spawnObstacle() {
	Obstacle* s = m_obstacles->get(); //new ScrollObject();
	//s->percentageX = float(i) / 10.0f;
	s->percentageX = MathUtil::randBetweenf(0.1f, 0.9f);
	s->percentageY = 1.0;
	s->recalcPositions();
	s->speed = generalSpeed;
	s->z = m_farDistance;

	float r = MathUtil::randBetweenf(0.0f, 1.0f);
	if (r < 0.5f) {
		s->m_type = Obstacle::TYPE_ROCK;
		s->m_image = Obstacle::s_imageRock;
	} else {
		s->m_type = Obstacle::TYPE_BUSH;
		s->m_image = Obstacle::s_imageBush;
	}	
	return s;
}

void InGameState::updateBackground(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	m_spawnFloorTimer += timer->getDelta();
	if (m_spawnFloorTimer >= m_spawnFloorDuration) {
		m_spawnFloorTimer -= m_spawnFloorDuration;

		Obstacle* o;
		o = spawnObstacle();
		o->percentageX = - 0.1;
		o->recalcPositions();
		
		o = spawnObstacle();
		o->percentageX = 1.1;
		o->recalcPositions();

		Floor* f = m_floors->get();
		f->percentageY = 1;
		f->z = m_farDistance;
		f->speed = generalSpeed;
		f->recalcPositions();
	}

	// floor decorations
    Input* in = ARK2D::getInput();
	if (in->isKeyPressed(Input::KEY_L)) { 
		m_spawnSideItemTimer = m_spawnSideItemDuration;
	}
	m_spawnSideItemTimer += timer->getDelta();
	if (m_spawnSideItemTimer >= m_spawnSideItemDuration) {
		//ScrollObject* s = m_scrollObjects->get(); //new ScrollObject();
		Obstacle* s = spawnObstacle();
		s->m_image = Obstacle::s_imageTree;
		s->m_type = Obstacle::TYPE_TREE;
		float r = MathUtil::randBetweenf(0.0f,1.0f);
		if (r < 0.5f) { 
			s->percentageX = MathUtil::randBetweenf(-1, 0);
		} else {
			s->percentageX = MathUtil::randBetweenf(1, 2);
		}
		s->percentageY = 1.0;
		s->recalcPositions();
		m_spawnSideItemTimer = 0.0f;
		m_spawnSideItemDuration = MathUtil::randBetweenf(0.25f, 0.35f);
	}

	m_obstacles->updateAll(container, timer);
    m_floors->updateAll(container, timer);

    m_obstacles->pruneAll();
    m_floors->pruneAll();
}

void InGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	DefaultGame* dg = DefaultGame::getInstance();
	Input* in = ARK2D::getInput();

	if (in->isKeyPressed(Input::KEY_ESCAPE) ||
		in->isKeyPressed(Input::KEY_ENTER) ||
		in->isKeyPressed(Input::KEY_BACKSPACE) ||
		in->isGamepadButtonPressed(Gamepad::BUTTON_START)) {
		game->enterState(dg->stateMenu, 
			new FadeToColourTransition(0.4f, dg->bgColor),
			new FadeFromColourTransition(0.4f, dg->bgColor)
		);
	}

	if (m_flashTimer > 0.0f) {
		m_flashTimer += timer->getDelta();
		if (m_flashTimer >= m_flashDuration) {
			m_flashTimer = 0.0f;
		}
	}

	updateBackground(container, game, timer);

	if (m_deathTimer > 0.0f) {
		m_deathTimer += timer->getDelta();
		speedMultiplier = Easing::easebetween(Easing::QUADRATIC_IN, m_deathTimer, 1.0f, 0.0f, m_deathDuration);
		dg->m_musicGame->setVolume(speedMultiplier);
		if (m_deathTimer >= m_deathDuration) {
			//reset();
			m_deathTimer = 0.0f;
			//reset();
            //dg->enterState(dg->stateSummary);//, new FadeToColorTransition());
	        game->enterState(dg->stateSummary, 
				new FadeToColourTransition(0.4f, dg->bgColor),
				new FadeFromColourTransition(0.4f, dg->bgColor)
			);

			return;
		}
	}
	else if (m_deathTimer == 0.0f) {

		// Spawn enemies
		for(int i = 0; i < m_spawnEnemies.size(); i++){
			SpawnEnemyEvent* ev = &m_spawnEnemies.at(i);
			ev->delay -= timer->getDelta();
			if (ev->delay < 0.0f) {
				Enemy* e = spawnEnemy(ev->enemyType);
				e->m_stopAt = ev->stopAt;
				m_spawnEnemies.erase(m_spawnEnemies.begin() + i);
				break;		
			}
		}

		m_waveTimer += timer->getDelta();
		if (m_waveTimer >= m_waveDuration) {
			
			if (m_enemies->sizeActive() == 0 && m_spawnEnemies.size() == 0) {
				
				if (m_waveTimer >= m_waveDuration+1){
					m_waveNumber++;
					m_waveTimer = 0.0f;
					m_waveChangeTimer = 0.01;
					//generalSpeed += 5;

					m_waveTimer -= m_waveDuration;

					dg->m_sfxBeePowerupBig->play();
					startWave(m_waveNumber);
				}
			} else {
				m_waveTimer = m_waveDuration;
			}
		}

		if (m_waveChangeTimer > 0.0f) {
			m_waveChangeTimer += timer->getDelta();
			if (m_waveChangeTimer >= m_waveChangeDuration) {
				m_waveChangeTimer = 0.0f;
			}
		}

		if (m_player->m_fireTimer == 0.0f) {
			if (in->isKeyPressed(Input::KEY_SPACE) || in->isGamepadButtonPressed(Gamepad::BUTTON_A)) {
				Bullet* b = m_bullets->get();
				b->player = true;
				b->reset();
				b->percentageX = m_player->getPercentageX();
				b->percentageY = m_player->getPercentageY();
				b->percentageY += (b->sizey/m_nearBounds->getMaxY())*0.5f;
				b->recalcPositions();
				b->z = 0.0f;

				dg->m_sfxBeeShoot->play();

				m_player->m_fireTimer = 0.01f;
			}
		} 
		if (in->isKeyReleased(Input::KEY_SPACE) || in->isGamepadButtonReleased(Gamepad::BUTTON_A)) {
			m_player->m_fireTimer = 0.0f;
		}
		if (m_player->m_fireTimer > 0.0f) {
			m_player->m_fireTimer += timer->getDelta();
			if (m_player->m_fireTimer >= m_player->m_fireDuration) {
				m_player->m_fireTimer = 0.0f;
			}
		}

		if (m_barrelRollTimer == 0.0f) {
			if (in->isKeyPressed(Input::KEY_B) || in->isGamepadButtonPressed(Gamepad::BUTTON_RBUMPER)) {
				m_barrelRollTimer = 0.01f;
				m_barrelRollClockwise = true;
				m_player->additionalVelocity.set(400, 0);
			}
			else if (in->isKeyPressed(Input::KEY_V) || in->isGamepadButtonPressed(Gamepad::BUTTON_LBUMPER)) {
				m_barrelRollTimer = 0.01f;
				m_barrelRollClockwise = false;
				m_player->additionalVelocity.set(-400, 0);
			}
		}

		m_spawnObstacleTimer += timer->getDelta();
		if (m_spawnObstacleTimer >= m_spawnObstacleDuration) {
			m_spawnObstacleTimer -= m_spawnObstacleDuration;
			spawnObstacle();
		}

		m_spawnCollectableTimer += timer->getDelta();
		if (m_spawnCollectableTimer >= m_spawnCollectableDuration) {
			Collectable* s = m_collectables->get();
			s->reset();
			s->speed = generalSpeed;
			s->z = m_farDistance;
			float r = MathUtil::randBetweenf(0.0f, 1.0f);
			if (r < 0.5f) { 
				s->m_type = Collectable::TYPE_RING;
				s->m_animation->addFrame(Collectable::s_imageRing);
				s->m_animation->setFrameTime(1.0f);
				s->percentageX = MathUtil::randBetweenf(0.2f, 0.8f);
				s->percentageY = MathUtil::randBetweenf(0.1f, 0.5f);
				s->sizex = 60;
				s->sizey = 60;
			} else {
				s->m_type = Collectable::TYPE_FLOWER;
				s->sizey = 80;
				s->m_animation->addFrame(Collectable::s_imageFlower1);
				s->m_animation->addFrame(Collectable::s_imageFlower2);
				s->m_animation->addFrame(Collectable::s_imageFlower3);
				s->m_animation->setFrameTime(0.2f);
				s->percentageX = MathUtil::randBetweenf(0.2f, 0.8f);
				s->percentageY = 1.0f;
			}
			s->recalcPositions();
				
				//m_scrollObjects->push_back(s);
	        m_spawnCollectableTimer -= m_spawnCollectableDuration;
		}

		if (in->isKeyPressed(Input::KEY_O)) {
			Enemy* e = m_enemies->get();
			e->reset();
			e->start(Enemy::TYPE_CROW);
		}

		// playable area.
		if (in->isKeyPressed(Input::KEY_K)) { 
			ScrollObject* s = m_scrollObjects->get(); //new ScrollObject();
			// s->startx = MathUtil::randBetweenf(m_farBounds->getMinX(), m_farBounds->getMaxX());
			// s->starty = m_farBounds->getMaxY();//MathUtil::randBetweenf(m_farBounds->getMinY(), m_farBounds->getMaxY());

			// s->endx = m_nearBounds->getMinX() + (((s->startx - m_farBounds->getMinX()) / m_farBounds->getWidth()) * m_nearBounds->getWidth());
			// s->endy = m_nearBounds->getMinY() + (((s->starty - m_farBounds->getMinY()) / m_farBounds->getHeight()) * m_nearBounds->getHeight());
			s->percentageX = MathUtil::randBetweenf(0.0f, 1.0f);
			s->percentageY = 1.0;
			s->recalcPositions();
			s->speed = generalSpeed; //20.0f;
			s->z = m_farDistance;
			//m_scrollObjects->push_back(s);
		}




	    
	   	doCollisions();

	}

	

	m_scrollObjects->updateAll(container, timer);
    m_collectables->updateAll(container, timer);
    m_enemies->updateAll(container, timer);
    m_bullets->updateAll(container, timer);
    m_explosions->updateAll(container, timer);
    
    
    m_particles->updateAll(container, timer);


	
	
	
	
	// player update 
	m_player->update(timer);
	if (m_deathTimer == 0.0f) { 
		if (m_player->m_bounds->getMinX() <= m_nearBounds->getMinX()) {
			m_player->m_bounds->setXByAlignment(m_nearBounds->getMinX(), Renderer::ALIGN_LEFT);
			m_player->m_velocity->m_x *= -0.5f;
		}
		if (m_player->m_bounds->getMaxX() >= m_nearBounds->getMaxX()) {
			m_player->m_bounds->setXByAlignment(m_nearBounds->getMaxX(), Renderer::ALIGN_RIGHT);
			m_player->m_velocity->m_x *= -0.5f;
		}

		if (m_player->m_bounds->getMaxY() >= m_nearBounds->getMaxY()) {
			m_player->m_bounds->setLocationByCenter(m_player->m_bounds->getCenterX(), m_nearBounds->getMaxY() - (m_player->m_bounds->getHeight()*0.5f));
			m_player->m_velocity->m_y *= -0.5f;
		}
		if (m_player->m_bounds->getMinY() <= 0) {
	        m_player->m_bounds->setLocationByCenter(m_player->m_bounds->getCenterX(),  m_player->m_bounds->getHeight()*0.5f);
	        m_player->m_velocity->m_y *= -0.5f;
		}
	}

	m_floorYTarget = m_floorMaxY;
	float halfHeightMin = container->getHeight() * 0.25f;
	float halfHeight = container->getHeight() * 0.75f;
	//if (m_player->m_bounds->getCenterY() > halfHeight) {
	//	m_floorY = halfHeight;
	//} else 
	if (m_player->m_bounds->getCenterY() < halfHeightMin) {
		m_floorYTarget = m_floorMinY;
	}
	else if (m_player->m_bounds->getCenterY() < halfHeight && m_player->m_bounds->getCenterY() > halfHeightMin) {
		m_floorYTarget = Easing::easebetween(Easing::LINEAR, m_player->m_bounds->getCenterY() - halfHeightMin, m_floorMinY, m_floorMaxY, halfHeight - halfHeightMin);
	} //else {
	//	m_floorY = halfHeightMin;
	//}


	float floorDistance = MathUtil::distance(0, m_floorY, 0, m_floorYTarget);
	float floorSpeed = Easing::easebetween(Easing::QUADRATIC_OUT, floorDistance, 0.0f, (m_floorMaxY-m_floorMinY)*1.5f, m_floorMaxY-m_floorMinY);
	//ARK2D::getLog()->e(StringUtil::append("floor y:", m_floorY));
	//ARK2D::getLog()->e(StringUtil::append("floor y target:", m_floorYTarget));
	//ARK2D::getLog()->e(StringUtil::append("floor distance:", floorDistance));
	//ARK2D::getLog()->e(StringUtil::append("floor speed:", floorSpeed));

	float newFloorY = m_floorY;
	if (m_floorY < m_floorYTarget) {
		newFloorY += (floorSpeed * timer->getDelta());
	} else if (m_floorY > m_floorYTarget) {
		newFloorY -= (floorSpeed * timer->getDelta());
	} else {
		newFloorY = m_floorYTarget;
	}
	m_floorY = newFloorY;
	m_farBounds->setYByAlignment(m_floorY, Renderer::ALIGN_BOTTOM);

    
	// for(int i = 0; i < m_scrollObjects->size(); i++) {
	// 	m_scrollObjects->at(i)->update(container, timer);
	// }
	

    

    m_scrollObjects->pruneAll();
    m_collectables->pruneAll();
    m_bullets->pruneAll();
    m_explosions->pruneAll();
    m_enemies->pruneAll();
    m_particles->pruneAll();


	updateCameraVars(1.0f);
	
}

void InGameState::doCollisions() {
	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	 // collisions
    PoolIterator<Collectable*> it_collectables = m_collectables->newiteratorref();
    while (it_collectables.hasNext()) {
    	Collectable* c = it_collectables.next();
    	//ARK2D::getLog()->e("AHHHH ");
    	if (c->isPendingRemoval()) { continue; }
    	bool collides = Shape<float>::collision_rectangleRectangle(c->m_bounds->getMinX(), c->m_bounds->getMinY(), c->m_bounds->getWidth(), c->m_bounds->getHeight(), 
         		m_player->m_bounds->getMinX(), m_player->m_bounds->getMinY(), m_player->m_bounds->getWidth(), m_player->m_bounds->getHeight()
        		);
    	//ARK2D::getLog()->e(StringUtil::append("z ", c->z));
    	//ARK2D::getLog()->e(StringUtil::append("b ", collides));
        if (c->z <= 1.0f && collides) {
        	if (c->m_type == Collectable::TYPE_FLOWER) {
        		dg->m_sfxBeePickup->at(MathUtil::randBetween(0,dg->m_sfxBeePickup->size()))->play();

        		dg->m_voEating->at(MathUtil::randBetween(0, dg->m_voStart->size()))->play();
        		m_pollenCount++;


        		MyParticle* p = m_particles->get();
				p->reset();
				p->m_animation->addFrame(Collectable::s_animSparkle->getFrames()->at(0));
				p->m_bounds->asRectangle()->setSize(40, 40);
				p->m_bounds->setLocationByCenter(c->m_bounds->getCenterX(), c->m_bounds->getCenterY() - 15);
				p->m_useGravity = false;
				p->m_lifetime = true;
				p->m_lifeDuration = 0.8f;
				p->m_lifeScaleStart = 1.0f;
				p->m_lifeScaleEnd = 2.0f;
				p->m_lifeAlphaStart = 1.0f;
				p->m_lifeAlphaEnd = 0.0f;	
				p->m_lifeRotationStart = 0.0f;
				p->m_lifeRotationEnd = 0.0f;
				p->m_rotateToVelocity = false;


        	} else if (c->m_type == Collectable::TYPE_RING) {
        		dg->m_sfxBeePowerupSmall->play();

        		dg->m_voPowerup->at(MathUtil::randBetween(0, dg->m_voStart->size()))->play();


        		MyParticle* p = m_particles->get();
				p->reset();
				p->m_animation->addFrame(Collectable::s_imageRing);
				p->m_bounds->asRectangle()->setSize(40, 40);
				p->m_bounds->setLocationByCenter(c->m_bounds->getCenterX(), c->m_bounds->getCenterY());
				p->m_useGravity = false;
				p->m_lifetime = true;
				p->m_lifeDuration = 0.8f;
				p->m_lifeScaleStart = 1.0f;
				p->m_lifeScaleEnd = 2.0f;
				p->m_lifeAlphaStart = 1.0f;
				p->m_lifeAlphaEnd = 0.0f;	
				p->m_lifeRotationStart = 0.0f;
				p->m_lifeRotationEnd = 720.0f;
				p->m_rotateToVelocity = false;

        	}
        	c->setPendingRemoval(true);
            
            ARK2D::getLog()->e("playsound");
        }
    }
    PoolIterator<Obstacle*> it_obstacles = m_obstacles->newiteratorref();
    while (it_obstacles.hasNext()) {
    	Obstacle* c = it_obstacles.next();
    	if (c->isPendingRemoval()) { continue; }
    	bool collides = Shape<float>::collision_rectangleRectangle(c->m_bounds->getMinX(), c->m_bounds->getMinY(), c->m_bounds->getWidth(), c->m_bounds->getHeight(), 
        		m_player->m_bounds->getMinX(), m_player->m_bounds->getMinY(), m_player->m_bounds->getWidth(), m_player->m_bounds->getHeight()
        		);
    	//ARK2D::getLog()->e(StringUtil::append("z", c->z));
    	//ARK2D::getLog()->e(StringUtil::append("b", collides));
    	
	    if (c->z <= 1.0f && collides) {
	        if (c->m_type == Obstacle::TYPE_ROCK) {
	        	//c->m_bounds->collides(m_player->m_bounds)) {
	        	ARK2D::getLog()->e("bam");
	        	c->die();
	        	c->setPendingRemoval(true);
	        	//dg->m_sfxBeePowerup1->play();
	        	
	        	dg->m_sfxBeeSplatRock->play();
	        	m_player->hit();

	        	continue; 
	        }
	        else if (c->m_type == Obstacle::TYPE_BUSH) {
	        	m_player->additionalVelocity.set(0,-400);
	        	dg->getTimeline()->addEvent([this]() -> void {
	        		//ErrorDialog::createAndShow("stuff");
	        		m_player->additionalVelocity.set(0,0); 
	        	}, 0.5f);
	        	m_player->m_velocity->addY(-200);
	        	if (!dg->m_sfxBushRustle->isPlaying()) {
	        		dg->m_sfxBushRustle->play();
	        	}
	        }
	    }

        PoolIterator<Bullet*> it_bullets = m_bullets->newiteratorref();
        while (it_bullets.hasNext()) {
        	Bullet* b = it_bullets.next();
        	if (b->isPendingRemoval()) { continue; }
        	if (b->player && b->z > c->z && b->m_bounds->collides(c->m_bounds)) {
        		ARK2D::getLog()->e("bam");
        		c->die();
        		c->setPendingRemoval(true);
        		b->setPendingRemoval(true);
        		//dg->m_sfxBeePowerup2->play();

        		
        	}
        }
    }

    PoolIterator<Enemy*> it_enemies = m_enemies->newiteratorref();
    while (it_enemies.hasNext()) {
    	Enemy* c = it_enemies.next();
    	if (c->isPendingRemoval()) { continue; }

    	PoolIterator<Bullet*> it_bullets = m_bullets->newiteratorref();
        while (it_bullets.hasNext()) {
        	Bullet* b = it_bullets.next();
        	if (b->isPendingRemoval()) { continue; }
        	if (b->z > c->z && b->m_bounds->collides(c->m_bounds)) {
        		c->die();
        		c->setPendingRemoval(true);

        		b->setPendingRemoval(true);
        		//dg->m_sfxBeePowerup2->play();

        		
        	}
        }
    	
	    // if (c->z <= 1.0f && c->m_bounds->collides(m_player->m_bounds)) {
	    //     if (c->m_type == Obstacle::TYPE_ROCK) {
	    //     	ARK2D::getLog()->e("bam");
	    //     	c->die();
	    //     	c->setPendingRemoval(true);
	    //     	//dg->m_sfxBeePowerup1->play();
	        	
	    //     	dg->m_sfxBeeSplatRock->play();
	    //     	m_player->die();

	    //     	continue; 
	    //     }
	    //     else if (c->m_type == Obstacle::TYPE_BUSH) {
	    //     	m_player->m_velocity->addY(-200);
	    //     }
	    // }
    }

    PoolIterator<Bullet*> it_bullets = m_bullets->newiteratorref();
    while (it_bullets.hasNext()) {
    	Bullet* b = it_bullets.next();
    	if (b->isPendingRemoval()) { continue; }
    	if (!b->player) { 
    		bool collides = Shape<float>::collision_rectangleRectangle(b->m_bounds->getMinX(), b->m_bounds->getMinY(), b->m_bounds->getWidth(), b->m_bounds->getHeight(), 
        		m_player->m_bounds->getMinX(), m_player->m_bounds->getMinY(), m_player->m_bounds->getWidth(), m_player->m_bounds->getHeight()
        		);

    		//ARK2D::getLog()->e(StringUtil::append("bz:",b->z));
    		//ARK2D::getLog()->e(StringUtil::append("pz:",m_player->z));
    		if (b->z <= 1.0f && collides) {
	    		b->setPendingRemoval(true);
	    		m_player->hit();
	    		if (m_player->canBeHit()) dg->m_sfxBeeSplatCrow->play();
	    		// TODO: shield breaking;
	    		//ErrorDialog::createAndShow("yo");
	    		
	    	}
	    }
    }

}


void InGameState::startExplosion(float percentageX, float percentageY, float z, float speed) {
	Explosion* e = m_explosions->get();
	e->reset();
	e->m_animation->setFrameTime(0.06f);
	if (percentageY == 1.0f) {	
		e->m_animation->addFrames(Explosion::s_explosion1);
	} else {
		e->m_animation->addFrames(Explosion::s_explosion2);
	}
	e->percentageX = percentageX;
	e->percentageY = percentageY;
	e->z = z;
	e->speed = speed;
	e->recalcPositions();

	DefaultGame::getInstance()->m_sfxExplosion->play();
}

void InGameState::updateCameraVars(float sc) {
	Vector2<float> playerAverage = getPlayerAveragePosition(); //Vector2<float>(m_player->m_bounds->getCenterX(), m_player->m_bounds->getCenterY());

	// Camera Target
    GameContainer* container = ARK2D::getContainer();
    GameTimer* timer = ARK2D::getTimer();
    
	m_cameraTargetCenterX = playerAverage.getX();

	if (playerAverage.getX() > m_nearBounds->getMaxX() - (container->getWidth()/2)) {
	 	m_cameraTargetCenterX = (m_nearBounds->getMaxX() - (container->getWidth()/2));
	} 
	else if (playerAverage.getX() < m_nearBounds->getMinX() + (container->getWidth()/2)) {
	 	m_cameraTargetCenterX = m_nearBounds->getMinX() + (container->getWidth()/2);
	} 
	// else if (playerAverage.getX() > m_nearBounds->getCenterX() + (container->getWidth()/2)) {
	// 	m_cameraTargetCenterX = playerAverage.getX() * ((playerAverage.getX() - (m_nearBounds->getCenterX() + (container->getWidth()/2))) / m_nearBounds->getWidth());
	// } 
	else {
		m_cameraTargetCenterX = playerAverage.getX();
	}
	
	// m_cameraTargetCenterY = container->getHeight()*0.5;
	// if (playerAverage.getY() > m_nearBounds->getHeight() - (container->getHeight()/2)) {
	// 	m_cameraTargetY = (m_nearBounds->getHeight() - (container->getHeight())) * -1;
	// } else if (playerAverage.getY() > container->getHeight()/2) {
	// 	m_cameraTargetY = (container->getHeight()/2) - playerAverage.getY();
	// }



	// Chase Camera Target
	float cameraDistance = MathUtil::distance(m_cameraCenterX, m_cameraCenterY, m_cameraTargetCenterX, m_cameraTargetCenterY);
	float cameraSpeed = Easing::ease(Easing::QUADRATIC_OUT, cameraDistance, 0.0f, 150.0f, 100.0f, true);
	float newCameraAngle = MathUtil::anglef(m_cameraCenterX, m_cameraCenterY, m_cameraTargetCenterX, m_cameraTargetCenterY);
	float newCameraX = m_cameraCenterX;
	float newCameraY = m_cameraCenterY;
	MathUtil::moveAngle<float>(newCameraX, newCameraY, newCameraAngle, cameraSpeed * timer->getDelta());

	float newCameraDistance = MathUtil::distance(m_cameraCenterX, m_cameraCenterY, newCameraX, newCameraY);
	if (newCameraDistance > cameraDistance) {
		newCameraX = m_cameraTargetCenterX;
		newCameraY = m_cameraTargetCenterY;
	}

	//newCameraX = m_cameraTargetCenterX;
	//newCameraY = m_cameraTargetCenterY;

	m_cameraCenterX = newCameraX;
	m_cameraCenterY = newCameraY;
	//m_cameraRect->setLocation(m_cameraX*-1, m_cameraY*-1);

	//m_cameraX += m_cameraShake->getXOffset();
	//m_cameraY += m_cameraShake->getYOffset();

	m_cameraShake->update(container, timer);

	//updateCameraVars(1.0f);

	if (m_cameraZoomTimer->update()) {
		m_cameraZoomTimer->loop();
	}
	if (m_cameraRotateTimer->update()) {
		m_cameraRotateTimer->loop();
	}

	// ZOOM
	/*float cameraZoomAmount = 0.01f;
	//if (m_cameraNauseating) { cameraZoomAmount = 0.2f; }
	cameraZoomAmount *= sc;

	float cameraZoomHalf = m_cameraZoomTimer->getDuration()/2.0f;
	if (m_cameraZoomTimer->getTime() < cameraZoomHalf) {
		m_cameraZoom = Easing::ease(Easing::QUADRATIC_IN_OUT, m_cameraZoomTimer->getTime(), 1.0f, cameraZoomAmount, cameraZoomHalf);
	} else {
		m_cameraZoom = Easing::ease(Easing::QUADRATIC_IN_OUT, m_cameraZoomTimer->getTime()-cameraZoomHalf, 1.0f + cameraZoomAmount, cameraZoomAmount*-1.0f, cameraZoomHalf);
	}*/
	m_cameraZoom = 1.0f;

	// ROTATE
	/*float cameraRotateAmount = 0.25f;
	//if (m_cameraNauseating) { cameraRotateAmount = 2.0f; }
	cameraRotateAmount *= sc;

	float cameraRotateQuarter = m_cameraRotateTimer->getDuration()/4.0f;
	if (m_cameraRotateTimer->getTime() < cameraRotateQuarter) {
		m_cameraRotate = Easing::ease(Easing::QUADRATIC_OUT, m_cameraRotateTimer->getTime(), 0.0f, cameraRotateAmount, cameraRotateQuarter);
	} else if (m_cameraRotateTimer->getTime() < 2*cameraRotateQuarter) {
		m_cameraRotate = Easing::ease(Easing::QUADRATIC_IN, m_cameraRotateTimer->getTime()-cameraRotateQuarter, cameraRotateAmount, cameraRotateAmount * -1.0f, cameraRotateQuarter);
	} else if (m_cameraRotateTimer->getTime() < 3*cameraRotateQuarter) {
		m_cameraRotate = Easing::ease(Easing::QUADRATIC_OUT, m_cameraRotateTimer->getTime()-(2*cameraRotateQuarter), 0.0f, cameraRotateAmount * -1.0f, cameraRotateQuarter);
	} else {
		m_cameraRotate = Easing::ease(Easing::QUADRATIC_IN, m_cameraRotateTimer->getTime()-(3*cameraRotateQuarter), cameraRotateAmount * -1.0f, cameraRotateAmount, cameraRotateQuarter);
	}*/

	m_cameraRotate = 0;
	m_cameraRotateWorld = 0;

	if (m_barrelRollTimer > 0.0f) {
		m_barrelRollTimer += timer->getDelta();
		if (m_barrelRollTimer >= m_barrelRollDuration) {
			m_barrelRollTimer = 0.0f;
			m_player->additionalVelocity.set(0, 0);
		}
		if (m_barrelRollClockwise) {
			m_cameraRotateWorld = Easing::easebetween(Easing::QUADRATIC_IN_OUT, m_barrelRollTimer, 0.0f, 360.0f, m_barrelRollDuration);
		} else {
			m_cameraRotateWorld = Easing::easebetween(Easing::QUADRATIC_IN_OUT, m_barrelRollTimer, 0.0f, -360.0f, m_barrelRollDuration);
		}
	}
	
	
}

bool sortScene(ScrollObject* first, ScrollObject* second) { 
	return first->z > second->z;
}

void InGameState::renderBackground(GameContainer* container, StateBasedGame* game, Renderer* r) {
	//r->pushMatrix();
	//r->translate(m_cameraCenterX, m_cameraCenterY);
	r->setDrawColor(m_skyColor);
	r->fillRect(0,0,container->getWidth(), container->getHeight());

	startGameCamera(1.0f, 1.0f);
	r->setDrawColor(Color::white);
	//float cameraCenterOffset = m_nearBounds->getCenterX() - m_cameraTargetCenterX;
	//float cameraCenterOffset = (container->getWidth()*0.5f) - m_cameraTargetCenterX;//m_nearBounds->getCenterX() - m_cameraTargetCenterX;
	float cameraCenterOffset = m_cameraTargetCenterX;
	m_skyImage->drawAligned(m_nearBounds->getCenterX(), m_floorY+0.1f, 0, 1);
    m_skyCloudImage->flip(true, false);
    m_skyCloudImage->drawAligned(m_nearBounds->getCenterX() - 150 + (cameraCenterOffset*-0.25), m_floorY+0.1f, 0, 1);
    m_skyCloudImage->flip(true, false);
    m_skyCloudImage->drawAligned(m_nearBounds->getCenterX() + 250 + (cameraCenterOffset*-0.25), m_floorY+0.1f, 0, 1);

	r->setDrawColor(m_floorColor);
	r->fillRect(m_nearBounds->getMinX() -50, m_floorY, m_nearBounds->getWidth()+100, container->getHeight() - m_floorY + 500 );
	r->setDrawColor(Color::white);
	m_floorImage->draw(m_nearBounds->getMinX() - 50, m_floorY, m_nearBounds->getWidth()+100, m_floorImage->getHeight());
 
	m_floors->renderAll(container, r);

	endGameCamera();
}
void InGameState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
	
    DefaultGame* dg = DefaultGame::getInstance();
    r->setDrawColor(Color::white);
	

	renderBackground(container, game, r);

	
	startGameCamera(1.0f, 1.0f);
	
	vector<ScrollObject*> objects;

	PoolIterator<ScrollObject*> it_scrollObjects = m_scrollObjects->newiteratorref();
    while (it_scrollObjects.hasNext()) {
    	ScrollObject* s = it_scrollObjects.next();
		objects.push_back(s);
	}

	PoolIterator<Collectable*> it_collectables = m_collectables->newiteratorref();
    while (it_collectables.hasNext()) {
    	Collectable* c = it_collectables.next();
    	objects.push_back(c);
    }

    PoolIterator<Obstacle*> it_obstacles = m_obstacles->newiteratorref();
    while (it_obstacles.hasNext()) {
    	Obstacle* c = it_obstacles.next();
    	objects.push_back(c);
    }

    PoolIterator<Bullet*> it_bullets = m_bullets->newiteratorref();
    while (it_bullets.hasNext()) {
    	Bullet* c = it_bullets.next();
    	objects.push_back(c);
    }

    PoolIterator<Explosion*> it_explosions = m_explosions->newiteratorref();
    while (it_explosions.hasNext()) {
    	Explosion* c = it_explosions.next();
    	objects.push_back(c);
    }

    PoolIterator<Enemy*> it_enemies = m_enemies->newiteratorref();
    while (it_enemies.hasNext()) {
    	Enemy* c = it_enemies.next();
    	objects.push_back(c);
    }

    std::sort(objects.begin(), objects.end(), sortScene);

    for(int i = 0; i < objects.size(); i++) {
    	objects[i]->render(container, r);
    	if (dg->m_debug ) objects[i]->renderBounds(container, r);
    }

    r->setDrawColor(Color::red);
	if (dg->m_debug) r->drawRect(m_player->m_bounds->getMinX(), m_player->m_bounds->getMinY(), m_player->m_bounds->getWidth(), m_player->m_bounds->getHeight());
    //m_player->renderBounds(container, r);



	r->setDrawColor(Color::red);
	if (dg->m_debug) r->drawRect(m_farBounds);
	if (dg->m_debug) r->drawRect(m_nearBounds);

	r->setDrawColor(Color::white);

	m_particles->renderAll(container, r);

	endGameCamera();

	m_cameraRotateWorld = 0;
	startGameCamera(1.0f, 1.0f);
		//r->setDrawColor(Color::yellow);
		//r->fillCircle(m_player->m_bounds->getCenterX(), m_player->m_bounds->getCenterY(), m_player->m_bounds->getWidth()*0.5, 25);


		if (m_player->m_hitTimer == 0.0f || fmod(m_player->m_hitTimer, 0.3f) < 0.15f) { 

			float shadowScale = 1.0f; 
			if (m_deathTimer > 0.0f) {
				shadowScale = Easing::easebetween(Easing::QUADRATIC_OUT, m_deathTimer, 1.0f, 0.0f, m_deathDuration );
			}
			
			if (m_barrelRollTimer > 0.0f) {
				shadowScale = Easing::easebetweenback(Easing::QUADRATIC_OUT, Easing::QUADRATIC_IN, m_barrelRollTimer, 1.0f, 0.0f, m_barrelRollDuration );
			}

			m_player->m_shadow->drawCenteredScaled(m_player->m_bounds->getCenterX(), m_nearBounds->getMaxY() - (m_player->m_shadow->getHeight()) - 1, shadowScale, shadowScale);

			r->setDrawColor(Color::white);
			Image* pla = m_player->getCurrentAnimation()->getCurrentFrame();
			pla->drawCentered(m_player->m_bounds->getCenterX(), m_player->m_bounds->getCenterY());
			if (m_player->m_shield) {
				m_player->m_animShield->getCurrentFrame()->drawCentered(m_player->m_bounds->getCenterX(), m_player->m_bounds->getCenterY());
			}
		}
		
	endGameCamera();

	

	float textShadow = 2;//dg->textShadow;
	//r->drawString(StringUtil::append("cx:", m_cameraCenterX), 10, 10, Renderer::ALIGN_LEFT,Renderer::ALIGN_TOP, 0.0f, 1.0f);
	//r->drawString(StringUtil::append("px:", m_player->m_bounds->getCenterX()), 10, 20, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP, 0.0f, 1.0f);
	//r->drawString(StringUtil::append("bx:", m_nearBounds->getMinX()), 10, 30, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP, 0.0f, 1.0f);
	r->setDrawColor(Color::black);
	r->drawString(StringUtil::append("", m_pollenCount), 10+textShadow, 10+textShadow, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP, 0.0f, 2.0f);
	r->setDrawColor(Color::white);
	r->drawString(StringUtil::append("", m_pollenCount), 10, 10, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP, 0.0f, 2.0f);

	if (m_waveChangeTimer > 0.0f) {
		float alphaMod = 1.0f;
		float scaleMod = 1.0f;
		float rotateMod = 0.0f;
		if (m_waveChangeTimer < m_waveChangeDuration * 0.25f) {
			scaleMod = Easing::easebetween(Easing::QUADRATIC_OUT, m_waveChangeTimer, 0.0f, 1.0f, m_waveChangeDuration*0.25f);
		} else if (m_waveChangeTimer > m_waveChangeDuration * 0.75f) {
			scaleMod = Easing::easebetween(Easing::QUADRATIC_OUT, m_waveChangeTimer-(m_waveChangeDuration * 0.75f), 1.0f, 0.0f, m_waveChangeDuration*0.25f);
			alphaMod = Easing::easebetween(Easing::QUADRATIC_OUT, m_waveChangeTimer-(m_waveChangeDuration * 0.75f), 1.0f, 0.0f, m_waveChangeDuration*0.25f);
			rotateMod = Easing::easebetween(Easing::QUADRATIC_OUT, m_waveChangeTimer-(m_waveChangeDuration * 0.75f), 0.0f, 360.0f, m_waveChangeDuration*0.25f);
		}
		string waveNum = StringUtil::append("SEASON ", m_waveNumber+1);
		r->setDrawColor(Color::black);
		r->drawString(waveNum, (container->getWidth()*0.5f)+textShadow, (container->getHeight()*0.3f)+2, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0f, 3.0f * scaleMod);
		r->setDrawColor(Color::white);
		r->drawString(waveNum, (container->getWidth()*0.5f), (container->getHeight()*0.3f), Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0f, 3.0f * scaleMod);
	}
	
	r->setDrawColor(Color::white);

	if (m_flashTimer > 0.0f) {
		float alpha = Easing::easebetween(Easing::QUADRATIC_IN, m_flashTimer, 1.0f, 0.0f, m_flashDuration);
		r->setDrawColorf(1.0f, 1.0f, 1.0f, alpha);
		r->fillRect(0,0,container->getWidth(), container->getHeight());
	}
	r->setDrawColor(Color::white);

	//r->setDrawColor(Color::black);
	//r->drawString(StringUtil::append("", m_player->z), m_player->m_bounds->getCenterX(), m_player->m_bounds->getCenterY());
}

Vector2<float> InGameState::getPlayerAveragePosition() {
	return Vector2<float>(m_player->m_bounds->getCenterX(), m_player->m_bounds->getCenterY());
}

void InGameState::startGameCamera(float scaleX, float scaleY) {
	Renderer* r = ARK2D::getRenderer();
    
    GameContainer* container = ARK2D::getContainer();
	r->pushMatrix();
	r->translate(
		((getCurrentCameraX()*-1) + (container->getWidth()*0.5f)) * scaleX, 
		((getCurrentCameraY()*-1) + (container->getHeight()*0.5f)) * scaleY
	);

	Vector2<float> averagePlayerPosition = getPlayerAveragePosition();

	r->pushMatrix();
	r->translate(averagePlayerPosition.getX(), averagePlayerPosition.getY());
	r->scale(m_cameraZoom, m_cameraZoom);
	r->translate(averagePlayerPosition.getX()*-1, averagePlayerPosition.getY()*-1);

	r->pushMatrix();
	r->translate(averagePlayerPosition.getX(), averagePlayerPosition.getY());
	r->rotate(m_cameraRotate);
	r->translate(averagePlayerPosition.getX()*-1, averagePlayerPosition.getY()*-1);

	r->pushMatrix();
	r->translate(m_farBounds->getCenterX(), m_farBounds->getCenterY());
	r->rotate(m_cameraRotateWorld);
	r->translate(m_farBounds->getCenterX()*-1, m_farBounds->getCenterY()*-1);

	//r->pushMatrix();
	//r->translate(m_player->m_bounds->getCenterX(), m_player->m_bounds->getCenterY());
	//r->rotate(m_player->getCurrentRotation() - 180);
	//r->translate(m_player->m_bounds->getCenterX()*-1, m_player->m_bounds->getCenterY()*-1);

	r->pushMatrix();
}
void InGameState::endGameCamera() {
	Renderer* r = ARK2D::getRenderer();
	r->popMatrix();
	r->popMatrix();

	r->popMatrix();
	r->popMatrix();
	
	r->popMatrix();
}

InGameState::~InGameState() {

}

