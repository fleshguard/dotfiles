#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "FiraCode:size=8:FontAwesome:size=15" };
static const char dmenufont[]       = "SourceCodePro:size=8";
static const char col_gray1[]       = "#191919";
static const char col_gray2[]       = "#665c54";
static const char col_gray3[]       = "#ebdbb2";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#665c54";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
/*const char *spcmd3[] = {"keepassxc", NULL };*/
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
/*	{"keepassxc",   spcmd3}, */
};

 

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ NULL,		  "spterm",		NULL,		SPTAG(0),		1,			 -1 },
	{ NULL,		  "spfm",		NULL,		SPTAG(1),		1,			 -1 },
	{ NULL,		  "keepassxc",	NULL,		SPTAG(2),		0,			 -1 },
 
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define Mod1 Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ Mod4Mask,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ Mod4Mask|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ Mod4Mask|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ Mod4Mask|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *fileManager[]  = { "dolphin", NULL };
static const char *menu[]  = { "rofi", "-show", "drun", NULL };

/* Multimedia Keys*/
static const char *volUp[]  = { "pamixer", "-i", "5", NULL };
static const char *volUpAlt[]  = { "pamixer", "-i", "1", NULL };
static const char *volDown[]  = { "pamixer", "-d", "5", NULL };
static const char *volDownAlt[]  = { "pamixer", "-d", "1", NULL };
static const char *volMute[]  = { "pamixer", "-t", NULL };
static const char *brightnessUp[] = { "brightnessctl", "set", "+5%", NULL};
static const char *brightnessDown[] = { "brightnessctl", "set", "5-%", NULL};

static const char *screenshot[] = { "scrot", NULL};
static const char *screenshotAlt[] = { "scrot", "-s", NULL};

#include "movestack.c"

static const Key keys[] = {
	/* modifier                     key        function        argument */
       
	{ 0,            XF86XK_AudioMute,                       spawn,          {.v = volMute } },
	{ 0,            XF86XK_AudioLowerVolume,                spawn,          {.v = volDown } },
	{ 0,            XF86XK_AudioRaiseVolume,                spawn,          {.v = volUp   } },
	{ 0|ShiftMask,  XF86XK_AudioLowerVolume,                spawn,          {.v = volDownAlt } },
	{ 0|ShiftMask,  XF86XK_AudioRaiseVolume,                spawn,          {.v = volUpAlt   } },
	{ 0, 		XF86XK_MonBrightnessUp,			spawn,		{.v = brightnessUp } },
	{ 0, 		XF86XK_MonBrightnessDown,        	spawn,		{.v = brightnessDown } },

	{ 0,                       XK_Print,  spawn,          {.v = screenshot } },
	{ 0|ShiftMask,             XK_Print,  spawn,          {.v = screenshotAlt } },

	{ Mod4Mask,                       XK_d,      spawn,          {.v = menu } },
	{ Mod4Mask,                       XK_v,      spawn,          {.v = fileManager } },
	{ Mod4Mask,                       XK_Return, spawn,          {.v = termcmd } },
	{ Mod4Mask|ShiftMask,             XK_b,      togglebar,      {0} },
	{ Mod4Mask,                       XK_o,      focusstack,     {.i = +0.35 } },
	{ Mod4Mask,                       XK_p,      focusstack,     {.i = -0.35 } },
	{ Mod4Mask,                       XK_i,      incnmaster,     {.i = +1 } },
	{ Mod4Mask,                       XK_r,      incnmaster,     {.i = -1 } },
	{ Mod4Mask,                       XK_h,      setmfact,       {.f = -0.05} },
	{ Mod4Mask,                       XK_l,      setmfact,       {.f = +0.05} },

	/* Move stack left and right */
	{ Mod4Mask|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ Mod4Mask|ShiftMask,             XK_k,      movestack,      {.i = -1 } },

	/* Scratchpads */
	{ Mod4Mask,            			XK_y,  	   togglescratch,  {.ui = 0 } },
	{ Mod4Mask,            			XK_u,	   togglescratch,  {.ui = 1 } },
	{ Mod4Mask,            			XK_x,	   togglescratch,  {.ui = 2 } },

	{ Mod4Mask,                       XK_Return, zoom,           {0} },
	{ Mod4Mask,                       XK_Tab,    view,           {0} },
	{ Mod4Mask|ShiftMask,             XK_q,      killclient,     {0} },
	{ Mod4Mask,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ Mod4Mask,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ Mod4Mask,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ Mod4Mask,                       XK_space,  setlayout,      {0} },
	{ Mod4Mask|ShiftMask,             XK_space,  togglefloating, {0} },
	{ Mod4Mask,                       XK_w,      fullscreen, {0} },
	{ Mod4Mask,                       XK_0,      view,           {.ui = ~0 } },
	{ Mod4Mask|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ Mod4Mask,                       XK_comma,  focusmon,       {.i = -1 } },
	{ Mod4Mask,                       XK_period, focusmon,       {.i = +1 } },
	{ Mod4Mask|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ Mod4Mask|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	
	/* Workspaces */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_0,                      9)
	{ Mod4Mask|ShiftMask,             XK_e,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         Mod4Mask,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         Mod4Mask,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         Mod4Mask,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            Mod4Mask,         Button1,        tag,            {0} },
	{ ClkTagBar,            Mod4Mask,         Button3,        toggletag,      {0} },
};

