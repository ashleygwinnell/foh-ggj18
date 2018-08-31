
#include "DefaultGame.h"

DefaultGame::DefaultGame(string title):
	StateBasedGame(title) {

}

void DefaultGame::initStates(GameContainer* container) {

	stateBlank = new BlankState();
	stateSplash = new SplashState();
	stateMenu = new MenuState();
	stateInstructions = new InstructionsState();
	stateInGame = new InGameState();
	stateSummary = new SummaryState();

	addState(stateBlank);
	addState(stateSplash);
	addState(stateMenu);
	addState(stateInstructions);
	addState(stateInGame);
	addState(stateSummary);

	bgColor = new Color("#000000");

	spritesheet = Resource::get("all.png")->asImage();
	desc = Resource::get("all.spritesheetdescription")->asSpriteSheetDescription();
	SpriteSheetStore::addSheet(desc,spritesheet);

	m_musicMenu = Resource::get("music/MUS_Menu.ogg")->asSound();
	m_musicGame = Resource::get("music/MUS_Level1.ogg")->asSound();
	m_musicGameover = Resource::get("music/MUS_GameOver.ogg")->asSound();
	m_music = m_musicMenu;

	//m_sfxTest = Resource::get("sfx/SFX_SelectStart.wav")->asSound();
	//m_sfxSelectPress = Resource::get("sfx/SFX_SelectPress.wav")->asSound();

	m_sfxBeePickup = new vector<Sound*>();
	m_sfxSelectChange = new vector<Sound*>();
	
	//m_sfxBeeDie = Resource::get("sfx/SFX_SelectChange1.ogg")->asSound();
	m_sfxBeePickup->push_back( Resource::get("sfx/SFX_BeePickup1.ogg")->asSound() );
	m_sfxBeePickup->push_back( Resource::get("sfx/SFX_BeePickup2.ogg")->asSound() );
	m_sfxBeePickup->push_back( Resource::get("sfx/SFX_BeePickup3.ogg")->asSound() );
	m_sfxBeePowerupBig = Resource::get("sfx/SFX_BeePowerupBig.ogg")->asSound();
	m_sfxBeePowerupSmall = Resource::get("sfx/SFX_BeePowerupSmall.ogg")->asSound();
	m_sfxBeeShoot = Resource::get("sfx/SFX_BeeShoot.ogg")->asSound();
	m_sfxBeeSplatCrow = Resource::get("sfx/SFX_BeeSplatCrow.ogg")->asSound();
	m_sfxBeeSplatRock = Resource::get("sfx/SFX_BeeSplatRock.ogg")->asSound();

	m_sfxBushRustle = Resource::get("sfx/SFX_BushRustle.ogg")->asSound();
	m_sfxCrowAppear = Resource::get("sfx/SFX_CrowAppear.ogg")->asSound();
	m_sfxCrowShoot = Resource::get("sfx/SFX_CrowShoot.ogg")->asSound();

	m_sfxExplosion = Resource::get("sfx/SFX_Explosion.ogg")->asSound();
	m_sfxSelectChange->push_back( Resource::get("sfx/SFX_SelectChange1.ogg")->asSound() );
	m_sfxSelectChange->push_back( Resource::get("sfx/SFX_SelectChange2.ogg")->asSound() );
	m_sfxSelectChange->push_back( Resource::get("sfx/SFX_SelectChange3.ogg")->asSound() );
	m_sfxSelectLocked = Resource::get("sfx/SFX_SelectLocked.ogg")->asSound();
	m_sfxSelectPress = Resource::get("sfx/SFX_SelectPress.ogg")->asSound();
	m_sfxSelectStart = Resource::get("sfx/SFX_SelectStart.ogg")->asSound();	
	m_sfxShieldDown = Resource::get("sfx/SFX_ShieldDown.ogg")->asSound();	

	m_voStart = new vector<Sound*>();
	m_voEating = new vector<Sound*>();
	m_voPowerup = new vector<Sound*>();
	m_voDie = new vector<Sound*>();
	
	m_voStart->push_back( Resource::get("sfx/bee_vox/VOX_Happy_YippidyDoo.ogg")->asSound() );
	m_voStart->push_back( Resource::get("sfx/bee_vox/VOX_Happy_Wee2.ogg")->asSound() );
	
	m_voEating->push_back( Resource::get("sfx/bee_vox/VOX_Happy_MmDelicious.ogg")->asSound() );
	m_voEating->push_back( Resource::get("sfx/bee_vox/VOX_Happy_MmTasty.ogg")->asSound() );
	m_voEating->push_back( Resource::get("sfx/bee_vox/VOX_Happy_OhHoo.ogg")->asSound() );
	m_voEating->push_back( Resource::get("sfx/bee_vox/VOX_Happy_SlurpMm.ogg")->asSound() );
	m_voEating->push_back( Resource::get("sfx/bee_vox/VOX_Happy_SlurpMmEnergetic.ogg")->asSound() );

	m_voPowerup->push_back( Resource::get("sfx/bee_vox/VOX_Happy_BuzzedeeBoo.ogg")->asSound() );
	m_voPowerup->push_back( Resource::get("sfx/bee_vox/VOX_Happy_ZoomZoom2.ogg")->asSound() );
	m_voPowerup->push_back( Resource::get("sfx/bee_vox/VOX_Happy_ZoomZoom.ogg")->asSound() );

	m_voDie->push_back( Resource::get("sfx/bee_vox/VOX_Sad_NastyCrows.ogg")->asSound() );
	m_voDie->push_back( Resource::get("sfx/bee_vox/VOX_Sad_OhNo.ogg")->asSound() );
	m_voDie->push_back( Resource::get("sfx/bee_vox/VOX_Sad_OhNo2.ogg")->asSound() );
	m_voDie->push_back( Resource::get("sfx/bee_vox/VOX_Sad_OhoOo.ogg")->asSound() );
	m_voDie->push_back( Resource::get("sfx/bee_vox/VOX_Sad_Ow.ogg")->asSound() );
	
	ARK2D::getRenderer()->setInterpolation(Renderer::INTERPOLATION_NEAREST);
	font = Resource::get("fonts/SG12_small.fnt")->asFont()->asBMFont();
	font->setKerning(0);
	//font->scale(0.8f);
	//ARK2D::getRenderer()->setDefaultFont(font);
	ARK2D::getRenderer()->setFont(font);

	//if (ARK2D::getPlatform() == ARK2D::PLATFORM_BROWSER_JS) {
	//	container->setSize(398 * 2, 224 * 2);
	//} else {
		container->setSize(398 * 3, 224 * 3);	
	//}

	

	m_music->play();

	m_renderTarget = new FBO(398, 224);
	m_renderTarget->name = "renderTarget";
	m_renderTarget->setClearScreenOnBind(true);

	enterState(stateMenu, 
		NULL,
		new FadeFromColourTransition(0.4f, bgColor)
	);
}

void DefaultGame::update(GameContainer* container, GameTimer* timer) {
	StateBasedGame::update(container, timer);

	Input* in = ARK2D::getInput();
	if (in->isGamepadButtonPressed(Gamepad::BUTTON_ACTIVATE) || in->isKeyPressed(Input::KEY_F)) {
		container->setFullscreen(!container->isFullscreen());
	}

	if (in->isKeyPressed(Input::KEY_1)) {
		container->setSize(398 * 1, 224 * 1);
	} else if (in->isKeyPressed(Input::KEY_2)) {
		container->setSize(398 * 2, 224 * 2);
	} else if (in->isKeyPressed(Input::KEY_3)) {
		container->setSize(398 * 3, 224 * 3);
	} else if (in->isKeyPressed(Input::KEY_4)) {
		container->setSize(398 * 4, 224 * 4);
	}else if (in->isKeyPressed(Input::KEY_5)) {
		container->setSize(398 * 5, 224 * 5);
	}

	if (getCurrentState() == stateMenu) {
		if (m_musicGame->isPlaying()) { m_musicGame->stop(); }
		if (m_musicGameover->isPlaying()) { m_musicGameover->stop(); }
		if (!m_musicMenu->isPlaying()) {
			m_musicMenu->play();
		}
    }
	else if (getCurrentState() == stateInGame) {
		if (m_musicMenu->isPlaying()) { m_musicMenu->stop(); }
		if (m_musicGameover->isPlaying()) { m_musicGameover->stop(); }
		if (!m_musicGame->isPlaying()) {
			m_musicGame->play();
		}
	}
	else if (getCurrentState() == stateSummary) {
		if (m_musicMenu->isPlaying()) { m_musicMenu->stop(); }
		if (m_musicGame->isPlaying()) { m_musicGame->stop(); }
		
	}
	if (in->isKeyPressed(Input::KEY_M)) {
		float vol = m_musicGame->getVolume();
		SoundStore::getInstance()->setVolumeByGroupId(0, ((vol == 0.0f)?1.0f:0.0f));
	}
	if (in->isKeyPressed(Input::KEY_P)) {
		m_debug = !m_debug; 
	}
}

void DefaultGame::render(GameContainer* container, Renderer* r) {
	StateBasedGame::render(container, r);
}
void DefaultGame::render() {
	//m_renderTarget->setClearColor(Color::black_0a);
	m_renderTarget->bind();
	m_renderTarget->bind_2d();
	
	ARK2D::getRenderer()->setBlendMode(Renderer::BLEND_NORMAL);
	StateBasedGame::render();

	m_renderTarget->unbind_2d();
	m_renderTarget->unbind();

	ARK2D::getRenderer()->setDrawColor(Color::white);
	(*m_renderTarget->getImage())->draw(0,0);
}

void DefaultGame::resize(GameContainer* container, int width, int height) {
	StateBasedGame::resize(container, width, height);
}

DefaultGame* DefaultGame::getInstance() {
    return dynamic_cast<DefaultGame*>(ARK2D::getGame());
}

DefaultGame::~DefaultGame() {

}
