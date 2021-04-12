#include "pch.h"
#include "FastShots.h"


BAKKESMOD_PLUGIN(FastShots, "Fast Shots Plugin", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
float blueMinSpeed = 100.f;
float orangeMinSpeed = 100.f;
float blueMaxSpeed = 300.f;
float orangeMaxSpeed = 300.f;
bool blueEnabled = false;
bool orangeEnabled = false;

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

	cvarManager->registerCvar("fast_shots_blue", "0", "makes the blue team take fast shots", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string, CVarWrapper cvar) { blueEnabled = cvar.getBoolValue(); });

	cvarManager->registerCvar("fast_shots_blue_min_speed", std::to_string(blueMaxSpeed), "minimum shot speed allowed for blue")
		.addOnValueChanged([this](std::string, CVarWrapper cvar) {
		blueMinSpeed = cvar.getFloatValue();
			});

	cvarManager->registerCvar("fast_shots_orange", "0", "makes the orange team take fast shots", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string, CVarWrapper cvar) { orangeEnabled = cvar.getBoolValue(); });

	cvarManager->registerCvar("fast_shots_orange_min_speed", std::to_string(orangeMinSpeed), "minimum shot speed allowed for orange")
		.addOnValueChanged([this](std::string, CVarWrapper cvar) {
		orangeMinSpeed = cvar.getFloatValue();
			});

	cvarManager->registerCvar("fast_shots_orange_max_speed", std::to_string(orangeMaxSpeed), "maximum shot speed allowed for orange")
		.addOnValueChanged([this](std::string, CVarWrapper cvar) {
		orangeMaxSpeed = cvar.getFloatValue();
			});

	cvarManager->registerCvar("fast_shots_blue_max_speed", std::to_string(blueMaxSpeed), "maximum shot speed allowed for blue")
		.addOnValueChanged([this](std::string, CVarWrapper cvar) {
		blueMaxSpeed = cvar.getFloatValue();
			});
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

	BallWrapper ball = sw.GetBall();

	if (!ball) {
		return;
	}

	// end of goal is +-5200
	auto ballLoc = ball.GetLocation();
	auto velocity = ball.GetVelocity();
	auto speed = velocity.magnitude();
	// converts speed to km/h from cm/s
	speed *= 0.036f;
	speed += 0.5f;

	if (ballLoc.Y > 5028 && velocity.Y > 0) {
		// ball is going in orange net
		cvarManager->log("shot taken at orange net " + std::to_string(speed));
		if (!blueEnabled) {
			return;
		}
		if (speed < blueMinSpeed || speed > blueMaxSpeed) {
			velocity.Y = -velocity.Y;
			ball.SetVelocity(velocity);
		}
	} else if (ballLoc.Y < -5028 && velocity.Y < 0) {
		// ball is going in blue net
		cvarManager->log("shot taken at blue net " + std::to_string(speed));
		if (!orangeEnabled) {
			return;
		}
		if (speed < orangeMinSpeed || speed > orangeMaxSpeed) {
			velocity.Y = -velocity.Y;
			ball.SetVelocity(velocity);
		}
	}
}
