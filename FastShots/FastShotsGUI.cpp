#include "pch.h"
#include "FastShots.h"

/*
// Do ImGui rendering here
void FastShots::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string FastShots::GetMenuName()
{
	return "FastShots";
}

// Title to give the menu
std::string FastShots::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void FastShots::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool FastShots::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool FastShots::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void FastShots::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void FastShots::OnClose()
{
	isWindowOpen_ = false;
}
*/
