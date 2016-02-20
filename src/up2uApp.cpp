#include "cinder/app/AppNative.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include "cinder/Surface.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Font.h"
#include "cinder/app/AppImplMsw.h"
#include "boost/thread/mutex.hpp"

#if defined(WIN32) && !defined(_DEBUG)
#include "AppVerify.h"
#endif

#include "Config.h"
#include "Debug.h"
#include "BitException.h"
#include "BitExceptionHandler.h"
#include "BitShortcutKey.h"

#include "AClass.h"

//Additional include
#include "ConfigReader.h"
#include "PlayerManager.h"
#include "StageManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class up2uApp : public AppNative {
public:
	void prepareSettings(Settings *settings);
	void setup();
	void shutdown();
	void mouseDown(MouseEvent event);
	void keyDown(KeyEvent event);
	void update();
	void draw();
	void emitClose();
	
private:
#if defined(WIN32) && !defined(_DEBUG)
	AppVerify  appVerify_;
#endif
	void toggleFullscreen();
	void toggleDisplayParameters();
	void checkExceptionFromThread();

	Bit::Config config_;
	bool      borderless_;
	
	// debugging values
	bool      debug_;
	bool      paramsOn_;
	Bit::ShortcutKey shortcutKey_;
	bool setupCompleted_;

	AClass aClass_;

	PlayerManager	playerMng_;
	StageManager	stageMng_;
};

void up2uApp::prepareSettings( Settings *settings )
{
#if defined(WIN32) && !defined(_DEBUG)
	//if( !appVerify_.isValid() ) {
	//	showCursor();
	//	MessageBox( NULL, L"Application has expired.", L"ERROR", MB_OK );	
	//	exit( 1 );
	//}
#endif

	// initialize debugging values
	debug_ = false;
	paramsOn_ = false;
	setupCompleted_ = false;

	try {
		// register shutdown function to exception handler
		Bit::ExceptionHandler::registerShutdownFunction(std::bind(&up2uApp::emitClose, this));
		// read settings for window setup
		config_.readConfig();
		Bit::Config::DisplayConfig appConfig = config_.getDisplayConfig();
		// setup window 
		settings->setWindowSize(appConfig.windowSize.x, appConfig.windowSize.y);
		settings->setWindowPos(appConfig.windowPos.x, appConfig.windowPos.y);
		settings->setAlwaysOnTop(appConfig.alwaysOnTop);
		settings->setBorderless(appConfig.borderless);
		borderless_ = appConfig.borderless;
		// setup cursor
		if (appConfig.hideCursor)
			hideCursor();
		else
			showCursor();
	}
	catch (std::exception& e) {
		Bit::ExceptionHandler::handleException(&e);
	}
}

void up2uApp::setup()
{
	// setup shortcut keys
	shortcutKey_.setupDisplayDialog("Shortcut list", Vec2i(400, 200), ColorA(0.3f, 0.3f, 0.3f, 0.4f));
	shortcutKey_.addShortcut(KeyEvent::KEY_ESCAPE, std::bind(&up2uApp::emitClose, this), "close application");
	shortcutKey_.addShortcut('d', &debug_, "toggle display debug mode");
	shortcutKey_.addShortcut('f', std::bind(&up2uApp::toggleFullscreen, this), "toggle fullscreen mode");
	shortcutKey_.addShortcut('p', std::bind(&up2uApp::toggleDisplayParameters, this), "toggle display parameters dialog");
	shortcutKey_.addShortcut(KeyEvent::KEY_F1, std::bind(&Bit::ShortcutKey::toggleDisplay, &shortcutKey_), "toggle display shortcut keys list");
	
	try {
		config_.setup();
		
		// setup everything here
		config_.readConfigurableConfig(aClass_, "aClassConfig");	// this will eventually calls AClass::readConfig() with the Bit::JsonTree* node named "aClassConfig" as argument
		config_.readConfigurableParams(aClass_, "aClassParams");	// this will eventually calls AClass::readParams() with the Bit::JsonTree* node named "aClassParams" as argument
		config_.readConfigurableConfig(playerMng_, "kinectTemplate");
		config_.readConfigurableParams(playerMng_, "playerManagerParams");
		config_.readConfigurableConfig(stageMng_, "allStageConfig");
		config_.readConfigurableParams(stageMng_, "stageManagerParams");
		
		stageMng_.setup();
		playerMng_.setup();
		playerMng_.setupUsers();

		if (!playerMng_.isKinectReady()){
			MessageBox(NULL, L"Kinnect is not connected !", L"Message", NULL);
			shutdown();
			emitClose();
		}

		// mark setup complete at the end of setup.
		setupCompleted_ = true;
	}
	catch (std::exception& e) {
		Bit::ExceptionHandler::handleException(&e);
	}
}

void up2uApp::emitClose()

{
	// if setup is donw (we have window), emit the same event like clicking windows' close button (X button) on upper right corner
	// TODO: we need to handle multiple windows later
	if (setupCompleted_){
		WindowImplMsw* impl = reinterpret_cast<WindowImplMsw*>(::GetWindowLongPtr((HWND)ci::app::getWindow()->getNative(), GWLP_USERDATA));
		impl->getWindow()->emitClose();
		impl->privateClose();
		delete impl;
		// quit will call shutdown() for clean up and close the app
		quit();
	}
	else{	// otherwise, simply exit
		exit(Bit::Exception::getExitCode());
	}
}

void up2uApp::shutdown()
{
	//int exitCode = Bit::Exception::getExitCode();
	//exit( exitCode );	// we can not exit() here as memory leaks will occur
	playerMng_.shutdown();
}

void up2uApp::toggleFullscreen()
{
	setFullScreen(!isFullScreen());
}

void up2uApp::toggleDisplayParameters()
{
	paramsOn_ = !paramsOn_;
	if (paramsOn_) {
		showCursor();

		config_.showParams();
	}
	else {
		hideCursor();

		config_.hideParams();
	}
}

void up2uApp::keyDown( KeyEvent event )
{
	shortcutKey_.keyDown(event);
	if (event.getCode() == KeyEvent::KEY_d){
		playerMng_.isKinectDebugMode = !playerMng_.isKinectDebugMode;
	}
	if (event.getCode() == KeyEvent::KEY_w){
		playerMng_.moveMotorUp();
	}
	if (event.getCode() == KeyEvent::KEY_s){
		playerMng_.moveMotorDown();
	}
	if (event.getCode() == KeyEvent::KEY_z){
		stageMng_.addColor(Vec3f(-0.01f,0.0f,0.0f));
	}
	if (event.getCode() == KeyEvent::KEY_x){
		stageMng_.addColor(Vec3f(0.01f, 0.0f, 0.0f));
	}
	if (event.getCode() == KeyEvent::KEY_a){
		stageMng_.addTimePhub(1);
	}
	if (event.getCode() == KeyEvent::KEY_s){
		stageMng_.addTimeTalk(1);
	}
}

void up2uApp::mouseDown( MouseEvent event )
{
}

void up2uApp::update()
{
	try {
		// check if there is any exception from thread, for more info see Bit::ExceptionHandler::checkExceptionFromThread
		Bit::ExceptionHandler::checkExceptionFromThread();
		
		// added update part here
		//if (playerMng_.isDataReady()){
		//	if (playerMng_.isPlayerDetected()){
		//		stageMng_.setPlayerDetection(true);
		//		stageMng_.setPersons(playerMng_.getPersons());
		//	}
		//	else{
		//		stageMng_.setPlayerDetection(false);
		//	}
		//}
		playerMng_.updateUsers();
		
		stageMng_.setPersons(playerMng_.getPersons());
		if (playerMng_.getPersons().size() > 0) stageMng_.setPlayerDetection(true);
		else stageMng_.setPlayerDetection(false);
		stageMng_.update();
	}
	catch (std::exception& e) {
		Bit::ExceptionHandler::handleException(&e);
	}
}

void up2uApp::draw()
{
	if (!setupCompleted_)
		return;

	// clear out the window with black
	gl::clear(Color(0, 0, 0));
	
	// draw everything here
	playerMng_.draw();
	stageMng_.draw();
	
	
	// all debugging things 
	
	if (debug_) {
		// draw fps
		gl::drawString(toString(getAverageFps()), Vec2f(10, 10), ColorA(0.3f, 0.3f, 0.7f, 1.0f), Font("Arial", 30));
	}
	
	if (paramsOn_) {

		config_.drawParams();
	}
	
	// draw all shortcut keys to dialog
	shortcutKey_.draw();
}

CINDER_APP_NATIVE(up2uApp, RendererGl)
