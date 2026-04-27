#pragma once

namespace UI
{
	enum class BackendFlags : unsigned int
	{
		None            = 0,
		FloatingPanels  = 1 << 0,  // no host window, panels float as OS windows (width/height ignored)
		InstantQuitWithEsc = 1 << 1, //Instant quit when pressing esc key
	};

	inline BackendFlags operator|(BackendFlags a, BackendFlags b)
	{
		return static_cast<BackendFlags>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
	}

	inline bool HasFlag(BackendFlags flags, BackendFlags flag)
	{
		return (static_cast<unsigned int>(flags) & static_cast<unsigned int>(flag)) != 0;
	}
}