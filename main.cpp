#include "main.hpp"

#include "windows_lean.h"

#include <string>
#include <sstream>

#include "nvse/PluginAPI.h"
#include "nvse/nvse_version.h"

#include "utils.hpp"

static const int TryFindWindowIntervalMs = 1000;
static const char* ModName = "fnv_clipcursor";
static const char* GameClassName = "Fallout: New Vegas";

static HWND GameWindow = NULL;
static WNDPROC GameWndProc = NULL;

BOOL WINAPI DllMain(HANDLE, DWORD, LPVOID)
{
	return true;
}

extern "C" {

EXPORT bool NVSEPlugin_Query(
	const NVSEInterface* nvse,
	PluginInfo* info
) {
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = ModName;
	info->version = 2;

	std::string log_filename(ModName);
	log_filename.append(".log");
	gLog.Open(log_filename.c_str());

	if (!version_check(nvse))
		return false;

	return true;
}

EXPORT bool NVSEPlugin_Load(
	const NVSEInterface* nvse
) {
	if (
		SetTimer(
			NULL,
			0,
			TryFindWindowIntervalMs,
			find_game_window_timer) == 0
	) {
		fatal_error("Failed to set timer");
		return false;
	}

	return true;
}

}

bool version_check(const NVSEInterface* nvse) {
	if (nvse->isEditor)
		return false;

	if (nvse->nvseVersion < NVSE_VERSION_INTEGER) {
		std::stringstream ss;
		ss
			<< "NVSE version is too old ("
			<< nvse->nvseVersion
			<< "), expected at least "
			<< NVSE_VERSION_INTEGER;

		std::string str = ss.str();
		fatal_error(str.c_str(), false);
		return false;
	}

	return true;
}

void CALLBACK find_game_window_timer(
	HWND window,
	UINT wm_timer,
	UINT_PTR timer_id,
	DWORD tick_count
) {
	GameWindow = FindWindowA(GameClassName, NULL);
	if (GameWindow == NULL)
		return;

	KillTimer(window, timer_id);
	inject_wndproc();
}

void inject_wndproc() {
	GameWndProc = reinterpret_cast<WNDPROC>(
		GetWindowLongPtrA(GameWindow, GWLP_WNDPROC));

	if (GameWndProc == NULL) {
		fatal_error("Failed to get game wndproc");
		return;
	}

	if (
		SetWindowLongPtrA(
			GameWindow,
			GWLP_WNDPROC,
			reinterpret_cast<LONG>(&injected_wndproc)) == 0
	) {
		fatal_error("Failed to set injected wndproc");
		return;
	}

	setup_clipcursor(GameWindow);

	_MESSAGE("fnv_clipcursor setup correctly");
}

LRESULT CALLBACK injected_wndproc(
	HWND window,
	UINT message,
	WPARAM w_param,
	LPARAM l_param
) {
	switch (message) {
		case WM_ACTIVATE: {
			if (w_param == WA_INACTIVE)
				unset_clipcursor();
			else
				setup_clipcursor(window);

			break;
		}
		case WM_MOUSEMOVE: {
			setup_clipcursor(window);
			break;
		}
		case WM_SETFOCUS: {
			setup_clipcursor(window);
			break;
		}
		case WM_KILLFOCUS: {
			unset_clipcursor();
			break;
		}
	}

	return GameWndProc(window, message, w_param, l_param);
}

void setup_clipcursor(HWND window) {
	RECT rect;
	if (!GetWindowRect(window, &rect)) {
		fatal_error("Failed to get window rect");
		return;
	}

	if (!ClipCursor(&rect))
		fatal_error("Failed to setup clip cursor");
}

void unset_clipcursor() {
	if (!ClipCursor(NULL))
		fatal_error("Failed to unset clip cursor");
}
