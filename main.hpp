#ifndef FNV_CLIPCURSOR_MAIN_HPP
#define FNV_CLIPCURSOR_MAIN_HPP

#include "windows_lean.h"

#include "nvse/PluginAPI.h"

#include "utils.hpp"

bool version_check(const NVSEInterface* nvse);

void CALLBACK find_game_window_timer(
	HWND window,
	UINT wm_timer,
	UINT_PTR timer_id,
	DWORD tick_count);

void setup_hooks();

LRESULT CALLBACK injected_wndproc(
	HWND window,
	UINT message,
	WPARAM w_param,
	LPARAM l_param);

void setup_clipcursor(HWND window);

void unset_clipcursor();

#endif // Include guard.