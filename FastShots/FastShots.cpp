#include "pch.h"
#include "FastShots.h"


BAKKESMOD_PLUGIN(FastShots, "Fast Shots Plugin", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
float old_speed_sq = 0.0f;
float min_speed = 100.f;

void FastShots::onLoad()
{
	_globalCvarManager = cvarManager;

	cvarManager->registerCvar("fast_shots_enabled", "0", "Enable the fast shots plugin", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string, CVarWrapper cvar) {
		if (cvar.getBoolValue()) {
			hookEvents();
		}
		else {
			unhookEvents();
		}
			});

	cvarManager->registerCvar("fast_shots_min_speed", "100", "minimum shot speed allowed")
		.addOnValueChanged([this](std::string, CVarWrapper cvar) {
		min_speed = cvar.getFloatValue();
			});
	//cvarManager->log("Plugin loaded!");

	//cvarManager->registerNotifier("my_aweseome_notifier", [&](std::vector<std::string> args) {
	//	cvarManager->log("Hello notifier!");
	//}, "", 0);

	//auto cvar = cvarManager->registerCvar("template_cvar", "hello-cvar", "just a example of a cvar");
	//auto cvar2 = cvarManager->registerCvar("template_cvar2", "0", "just a example of a cvar with more settings", true, true, -10, true, 10 );

	//cvar.addOnValueChanged([this](std::string cvarName, CVarWrapper newCvar) {
	//	cvarManager->log("the cvar with name: " + cvarName + " changed");
	//	cvarManager->log("the new value is:" + newCvar.getStringValue());
	//});

	//cvar2.addOnValueChanged(std::bind(&FastShots::YourPluginMethod, this, _1, _2));

	// enabled decleared in the header
	//enabled = std::make_shared<bool>(false);
	//cvarManager->registerCvar("TEMPLATE_Enabled", "0", "Enable the TEMPLATE plugin", true, true, 0, true, 1).bindTo(enabled);

	//cvarManager->registerNotifier("NOTIFIER", [this](std::vector<std::string> params){FUNCTION();}, "DESCRIPTION", PERMISSION_ALL);
	//cvarManager->registerCvar("CVAR", "DEFAULTVALUE", "DESCRIPTION", true, true, MINVAL, true, MAXVAL);//.bindTo(CVARVARIABLE);
	//gameWrapper->HookEvent("FUNCTIONNAME", std::bind(&TEMPLATE::FUNCTION, this));
	//gameWrapper->HookEventWithCallerPost<ActorWrapper>("FUNCTIONNAME", std::bind(&FastShots::FUNCTION, this, _1, _2, _3));
	//gameWrapper->RegisterDrawable(bind(&TEMPLATE::Render, this, std::placeholders::_1));


	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", [this](std::string eventName) {
	//	cvarManager->log("Your hook got called and the ball went POOF");
	//});
	// You could also use std::bind here
	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", std::bind(&FastShots::YourPluginMethod, this);
}

void FastShots::onUnload()
{
}

// hooks events to allow the plugin to work
void FastShots::hookEvents() {
	gameWrapper->HookEventWithCallerPost<CarWrapper>("Function TAGame.Car_TA.SetVehicleInput",
		[this](CarWrapper caller, void*, std::string) { onTick(caller); });
}

// hooks events to allow the plugin to work
void FastShots::unhookEvents() {
	gameWrapper->UnhookEventPost("Function TAGame.Car_TA.SetVehicleInput");
}

ServerWrapper FastShots::getSW() {
	if (gameWrapper->IsInOnlineGame()) {
		cvarManager->log("null server");
		return NULL;
	}
	if (gameWrapper->IsInGame()) {
		auto server = gameWrapper->GetGameEventAsServer();

		if (!server) {
			cvarManager->log("null server");
			return NULL;
		}

		return server;
	}
	//cvarManager->log("no server");
	return NULL;
}

void FastShots::onTick(CarWrapper caller) {
	auto sw = getSW();

	if (!sw) return;

	CarWrapper myCar = gameWrapper->GetLocalCar();

	if (myCar.IsNull()) {
		return;
	}

	if (caller.memory_address != myCar.memory_address) {
		return;
	}

	BallWrapper ball = sw.GetBall();

	if (!ball) {
		return;
	}
	// end of goal is +-5200
	auto ballLoc = ball.GetLocation();
	auto velocity = ball.GetVelocity();
	auto speed = velocity.magnitude();
	// converts speed to km/h from cm/h
	speed *= 0.036f;
	speed += 0.5f;

	//if (new_speed_sq > old_speed_sq) {
	//if ((ballLoc.Y >= 5000 ) || (ballLoc.Y <= -5000 )) {
	bool positive = ballLoc.Y > 0;
	if ((ballLoc.Y > 5200 && velocity.Y > 0) || (ballLoc.Y < -5200 && velocity.Y < 0)) {
		cvarManager->log("shot taken at " + std::to_string(speed));
		if (speed < min_speed) {
			velocity.Y = -velocity.Y;
			ball.SetVelocity(velocity);
		}
	}
}
