/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx       = 1;        /* border pixel of windows */
static const unsigned int gappx          = 5;        /* gaps between windows */
static const unsigned int snap           = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */                                             
static const unsigned int systrayspacing = 2;   /* systray spacing */                
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/                 
static const int showsystray             = 1;
static const int showbar                 = 1;        /* 0 means no bar */
static const int topbar                  = 1;        /* 0 means bottom bar */
static const unsigned int colorfultag    = 1;  /* 0 means use SchemeSel for selected tag */
static const int user_bh                 = 20;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]               = { "JetBrainsMono Nerd Font Mono:Medium:size=13" };
static const char dmenufont[]            = "JetBrainsMono Nerd Font:Italic:size=13";
static const char col_gray1[]            = "#222222";
static const char col_gray2[]            = "#444444";
static const char col_gray3[]            = "#bbbbbb";
static const char col_gray4[]            = "#eeeeee";
static const char col_cyan[]             = "#742040";

#include "themes/dracula.h"

static const char *colors[][3]      = {
	/*                 fg           bg      border   */
	[SchemeNorm]       = { gray3,   black,  gray2 },
	[SchemeSel]        = { white,   black,  gray3 },
	[SchemeTag]        = { gray3,   black,  black},
	[SchemeTag1]       = { red,     black,  black },
	[SchemeTag2]       = { orange,  black,  black },
	[SchemeTag3]       = { yellow,  black,  black },
	[SchemeTag4]       = { blue,    black,  black },
	[SchemeTag5]       = { pink,    black,  black },
	[SchemeTag6]       = { red,     black,  black },
	[SchemeTag7]       = { orange,  black,  black },
	[SchemeTag8]       = { yellow,  black,  black },
	[SchemeTag9]       = { blue,    black,  black },
	[SchemeLayout]     = { white,   black,  black },
};

/* tagging */
static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };
static const int tagschemes[] = { SchemeTag1, SchemeTag2, SchemeTag3,
                                  SchemeTag4, SchemeTag5, SchemeTag6,
                                  SchemeTag7, SchemeTag8, SchemeTag9 };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask

#define BrightUp XF86XK_MonBrightnessUp
#define BrightDown XF86XK_MonBrightnessDown

#define AudioPlay XF86XK_AudioPlay
#define AudioPrev XF86XK_AudioPrev
#define AudioNext XF86XK_AudioNext

#define AudioMute XF86XK_AudioMute
#define AudioUp XF86XK_AudioRaiseVolume
#define AudioDown XF86XK_AudioLowerVolume	

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", gray2, "-nf", white, "-sb", blue, "-sf", gray4, NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *spotify[]  = { "spotify", NULL };
static const char *yt_music[]  = { "youtube-music", NULL };
static const char *flameshot[]  = { "flameshot", "gui", NULL };

/* backlight */
static const char *brightnessup[] = { "brillo", "-A", "5", NULL };
static const char *brightnessdown[] = { "brillo", "-U", "5", NULL };

/* audio control */
static const char *playpause[] = { "playerctl", "play-pause", NULL };
static const char *next[] = { "playerctl", "next", NULL };
static const char *prev[] = { "playerctl", "previous", NULL };
static const char *volup[] = { "pulseaudio-ctl", "up", NULL };
static const char *voldown[] = { "pulseaudio-ctl", "down", NULL };
static const char *volmute[] = { "pulseaudio-ctl", "mute", NULL };

/* kb layout */
static const char *cg_layout[] = {"switch_kbd_locale", NULL};
static const char *cg_bg[] = {"wper", NULL};

static Key keys[] = {
	/* modifier                     key            function        argument */
	{ 0,                            BrightUp,      spawn,          {.v = brightnessup} },
	{ 0,                            BrightDown,    spawn,          {.v = brightnessdown} },
	{ 0,                            AudioPlay,     spawn,          {.v = playpause} },
	{ 0,                            AudioPrev,     spawn,          {.v = prev} },
	{ 0,                            AudioNext,     spawn,          {.v = next} },
	{ 0,                            AudioMute,     spawn,          {.v = volmute} },
	{ 0,                            AudioUp,       spawn,          {.v = volup} },
	{ 0,                            AudioDown,     spawn,          {.v = voldown} },
	{ MODKEY|ShiftMask,             XK_l,          spawn,          {.v = cg_layout} },
	{ MODKEY,                       XK_w,          spawn,          {.v = cg_bg} },
	{ MODKEY,                       XK_p,          spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,     spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_s,          spawn,          {.v = flameshot} },
	{ MODKEY|ControlMask,           XK_s,          spawn,          {.v = spotify} },
	{ MODKEY|ControlMask,           XK_y,          spawn,          {.v = yt_music} },
	{ MODKEY,                       XK_b,          togglebar,      {0} },
	{ MODKEY,                       XK_j,          focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,          focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,          incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,          incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,          setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,          setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,     zoom,           {0} },
	{ MODKEY,                       XK_Tab,        view,           {0} },
	{ MODKEY,                       XK_q,          killclient,     {0} },
	{ MODKEY,                       XK_t,          setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,          setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,          setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,      togglefloating, {0} },
	{ MODKEY,                       XK_0,          view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,          tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,     focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,     tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_c,          quit,           {0} },
	{ MODKEY,                       XK_minus,      setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,      setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,      setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                          0)
	TAGKEYS(                        XK_2,                          1)
	TAGKEYS(                        XK_3,                          2)
	TAGKEYS(                        XK_4,                          3)
	TAGKEYS(                        XK_5,                          4)
	TAGKEYS(                        XK_6,                          5)
	TAGKEYS(                        XK_7,                          6)
	TAGKEYS(                        XK_8,                          7)
	TAGKEYS(                        XK_9,                          8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
