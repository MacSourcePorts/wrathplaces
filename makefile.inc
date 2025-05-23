# Invalid call detection
CHECKLEVEL1 = @if [ "$(LEVEL)" != 1 ]; then $(MAKE) help; false; fi
CHECKLEVEL2 = @if [ "$(LEVEL)" != 2 ]; then $(MAKE) help; false; fi

# Choose the compiler you want to use
CC?=gcc

# athlon optimizations
#CPUOPTIMIZATIONS?=-march=athlon
# athlon xp optimizations
#CPUOPTIMIZATIONS?=-march=athlon-xp
# athlon 64 optimizations
#CPUOPTIMIZATIONS?=-march=athlon64 -m32
# Pentium 3 optimizations
#CPUOPTIMIZATIONS?=-march=pentium3
# Pentium 4 optimizations
#CPUOPTIMIZATIONS?=-march=pentium4
# 686 (Pentium Pro/II) optimizations
#CPUOPTIMIZATIONS?=-march=i686
# No specific CPU (386 compatible)
#CPUOPTIMIZATIONS?=
# Experimental
#CPUOPTIMIZATIONS?=-fno-math-errno -ffinite-math-only -fno-rounding-math -fno-signaling-nans -fassociative-math -freciprocal-math -fno-signed-zeros -fno-trapping-math
# Normal
CPUOPTIMIZATIONS?=-fno-math-errno -fno-rounding-math -fno-trapping-math
# NOTE: *never* *ever* use the -ffast-math or -funsafe-math-optimizations flag

SDL_CONFIG?=sdl2-config
# ATS - 2022 - I added this here as on some linux distro's it doesn't find it
# and instead complains that --cflags isn't valid.  So lets just use the whole thing.
SDLCONFIG_UNIXCFLAGS?=`sdl2-config --cflags --libs`
SDLCONFIG_UNIXCFLAGS_X11?=
SDLCONFIG_UNIXLIBS?=`sdl2-config --cflags --libs`
SDLCONFIG_UNIXLIBS_X11?=-lX11
SDLCONFIG_UNIXSTATICLIBS?=`sdl2-config --cflags --static-libs`
SDLCONFIG_UNIXSTATICLIBS_X11?=-lX11
SDLCONFIG_MACOSXCFLAGS=$(SDLCONFIG_UNIXCFLAGS)
SDLCONFIG_MACOSXLIBS=$(SDLCONFIG_UNIXLIBS)
SDLCONFIG_MACOSXSTATICLIBS=$(SDLCONFIG_UNIXSTATICLIBS)
STRIP?=strip

###### Sound and audio CD #####

OBJ_SND_COMMON=snd_main.o snd_mem.o snd_mix.o snd_ogg.o snd_wav.o

# No sound
OBJ_SND_NULL=snd_null.o
LIB_SND_NULL=

# Open Sound System (Linux, FreeBSD and Solaris)
OBJ_SND_OSS=$(OBJ_SND_COMMON) snd_oss.o
LIB_SND_OSS=

# Core Audio (Mac OS X)
OBJ_SND_COREAUDIO=$(OBJ_SND_COMMON) snd_coreaudio.o
LIB_SND_COREAUDIO=-framework CoreAudio

# BSD / Sun audio API (NetBSD and OpenBSD)
OBJ_SND_BSD=$(OBJ_SND_COMMON) snd_bsd.o
LIB_SND_BSD=

# DirectX and Win32 WAVE output (Win32)
OBJ_SND_WIN=$(OBJ_SND_COMMON) snd_win.o
LIB_SND_WIN=

# Qantourisc's 3D Realtime Acoustic Lib (3D RAS)
OBJ_SND_3DRAS=snd_3dras.o
LIB_SND_3DRAS=

# CD objects
OBJ_CD_COMMON=cd_shared.o
OBJ_NOCD=cd_null.o


###### Common objects and flags #####

# Common objects
OBJ_COMMON= \
	bih.o \
	crypto.o \
	cl_collision.o \
	cl_demo.o \
	cl_dyntexture.o \
	cl_input.o \
	cl_main.o \
	cl_parse.o \
	cl_particles.o \
	cl_screen.o \
	cl_steam.o \
	cl_video.o \
	clvm_cmds.o \
	cmd.o \
	collision.o \
	common.o \
	console.o \
	csprogs.o \
	curves.o \
	cvar.o \
	dpsoftrast.o \
	dpvsimpledecode.o \
	filematch.o \
	fractalnoise.o \
	fs.o \
	ft2.o \
	utf8lib.o \
	gl_backend.o \
	gl_draw.o \
	gl_rmain.o \
	gl_rsurf.o \
	gl_textures.o \
	hmac.o \
	host.o \
	host_cmd.o \
	image.o \
	image_png.o \
	jpeg.o \
	keys.o \
	lhnet.o \
	libcurl.o \
	mathlib.o \
	matrixlib.o \
	mdfour.o \
	meshqueue.o \
	mod_skeletal_animatevertices_sse.o \
	mod_skeletal_animatevertices_generic.o \
	model_alias.o \
	model_brush.o \
	model_shared.o \
	model_sprite.o \
	netconn.o \
	nodegraph.o \
	palette.o \
	polygon.o \
	portals.o \
	protocol.o \
	prvm_cmds.o \
	prvm_edict.o \
	prvm_exec.o \
	r_explosion.o \
	r_lerpanim.o \
	r_lightning.o \
	r_modules.o \
	r_shadow.o \
	r_sky.o \
	r_sprites.o \
	sbar.o \
	sv_demo.o \
	sv_main.o \
	sv_move.o \
	sv_phys.o \
	sv_user.o \
	svbsp.o \
	svvm_cmds.o \
	sys_shared.o \
	vid_shared.o \
	view.o \
	wad.o \
	world.o \
	zone.o

OBJ_MENU= \
	menu.o \
	mvm_cmds.o

# note that builddate.c is very intentionally not compiled to a .o before
# being linked, because it should be recompiled every time an executable is
# built to give the executable a proper date string
OBJ_SV= builddate.c vid_null.o thread_null.o $(OBJ_SND_NULL) $(OBJ_COMMON)
OBJ_SDL= builddate.c sys_sdl.o vid_sdl.o thread_sdl.o $(OBJ_MENU) $(OBJ_SND_COMMON) snd_sdl.o $(OBJ_SDLCD) $(OBJ_VIDEO_CAPTURE) $(OBJ_COMMON)


# Compilation
CFLAGS_COMMON=$(CFLAGS_MAKEDEP) $(CFLAGS_PRELOAD) $(CFLAGS_FS) $(CFLAGS_WARNINGS) $(CFLAGS_LIBZ) $(CFLAGS_LIBJPEG) $(CFLAGS_D3D) -D_FILE_OFFSET_BITS=64 -D__KERNEL_STRICT_NAMES -I../../../
CFLAGS_CLIENT=-DCONFIG_MENU -DCONFIG_CD $(CFLAGS_VIDEO_CAPTURE)
CFLAGS_SERVER=
CFLAGS_DEBUG=-ggdb
CFLAGS_PROFILE=-g -pg -ggdb -fprofile-arcs
CFLAGS_RELEASE=
CFLAGS_RELEASE_PROFILE=-fbranch-probabilities
CFLAGS_SDL=$(SDLCONFIG_CFLAGS)

ifeq ($(DP_MACHINE),x86_64)
	CFLAGS_SSE=-msse
	CFLAGS_SSE2=-msse2
endif

OPTIM_DEBUG=$(CPUOPTIMIZATIONS)
#OPTIM_RELEASE=-O2 -fno-strict-aliasing -ffast-math -funroll-loops $(CPUOPTIMIZATIONS)
#OPTIM_RELEASE=-O2 -fno-strict-aliasing -fno-math-errno -fno-trapping-math -ffinite-math-only -fno-signaling-nans -fcx-limited-range -funroll-loops $(CPUOPTIMIZATIONS)
#OPTIM_RELEASE=-O2 -fno-strict-aliasing -funroll-loops $(CPUOPTIMIZATIONS)
#OPTIM_RELEASE=-O2 -fno-strict-aliasing $(CPUOPTIMIZATIONS)
OPTIM_RELEASE=-O3 -fno-strict-aliasing $(CPUOPTIMIZATIONS)
# NOTE: *never* *ever* use the -ffast-math or -funsafe-math-optimizations flag

DO_CC=$(CC) -arch $(ARCH) -mmacosx-version-min=10.9 $(CFLAGS) -c $< -o $@


# Link
LDFLAGS_DEBUG=-g -ggdb $(OPTIM_DEBUG) -DSVNREVISION=`{ test -d .svn && svnversion; } || { test -d .git && git describe --always; } || echo -` -DBUILDTYPE=debug
LDFLAGS_PROFILE=-g -pg -fprofile-arcs $(OPTIM_RELEASE) -DSVNREVISION=`{ test -d .svn && svnversion; } || { test -d .git && git describe --always; } || echo -` -DBUILDTYPE=profile
LDFLAGS_RELEASE=$(OPTIM_RELEASE) -DSVNREVISION=`{ test -d .svn && svnversion; } || { test -d .git && git describe --always; } || echo -` -DBUILDTYPE=release


##### UNIX specific variables #####

OBJ_GLX= builddate.c keysym2ucs.o $(OBJ_MENU) $(OBJ_SOUND) $(OBJ_CD) $(OBJ_VIDEO_CAPTURE) $(OBJ_COMMON)

LDFLAGS_UNIXCOMMON=-lm $(LIB_ODE) $(LIB_Z) $(LIB_JPEG) $(LIB_CRYPTO) $(LIB_CRYPTO_RIJNDAEL)
LDFLAGS_UNIXCL=-L$(UNIX_X11LIBPATH) -lX11 -lXpm -lXext -lXxf86vm -pthread $(LIB_SOUND)
LDFLAGS_UNIXCL_PRELOAD=-lz -ljpeg -lpng -logg -ltheora -lvorbis -lvorbisenc -lvorbisfile -lcurl
LDFLAGS_UNIXSV_PRELOAD=-lz -ljpeg -lpng -lcurl
LDFLAGS_UNIXSDL_PRELOAD=-lz -ljpeg -lpng -logg -ltheora -lvorbis -lvorbisenc -lvorbisfile -lcurl
CFLAGS_UNIX_PRELOAD=-DPREFER_PRELOAD

LDFLAGS_UNIXSDL=$(SDLCONFIG_LIBS)
EXE_UNIXCL=wrath
EXE_UNIXSV=wrath-dedicated
EXE_UNIXSDL=wrath
EXE_UNIXCLNEXUIZ=nexuiz-glx
EXE_UNIXSVNEXUIZ=nexuiz-dedicated
EXE_UNIXSDLNEXUIZ=nexuiz-sdl

CMD_UNIXRM=rm -rf
CMD_UNIXCP=cp -f
CMD_UNIXMKDIR=mkdir -p


##### Linux specific variables #####

# Link
LDFLAGS_LINUXCL=$(LDFLAGS_UNIXCOMMON) -lrt -ldl $(LDFLAGS_UNIXCL)
LDFLAGS_LINUXSV=$(LDFLAGS_UNIXCOMMON) -lrt -ldl
LDFLAGS_LINUXSDL=$(LDFLAGS_UNIXCOMMON) -lrt -ldl $(LDFLAGS_UNIXSDL)


##### Mac OS X specific variables #####


# Link
LDFLAGS_MACOSXSV=$(LDFLAGS_UNIXCOMMON) -ldl
LDFLAGS_MACOSXSDL=$(LDFLAGS_UNIXCOMMON) -F/System/Library/Frameworks/ -ldl -framework IOKit -L$(LIB_DIR) -lSDL2 -framework Cocoa -framework Foundation

##### SunOS specific variables #####

# No CD support available
OBJ_SUNOSCD=$(OBJ_NOCD)

CFLAGS_SUNOS=-I/usr/lib/oss/include -DBSD_COMP -DSUNOS

# Link
LDFLAGS_SUNOSCL=$(LDFLAGS_UNIXCOMMON) -lrt -ldl -lsocket -lnsl -R$(UNIX_X11LIBPATH) -L$(UNIX_X11LIBPATH) -lX11 -lXpm -lXext -lXxf86vm $(LIB_SOUND)
LDFLAGS_SUNOSSV=$(LDFLAGS_UNIXCOMMON) -lrt -ldl -lsocket -lnsl
LDFLAGS_SUNOSSDL=$(LDFLAGS_UNIXCOMMON) -lrt -ldl -lsocket -lnsl $(LDFLAGS_UNIXSDL)


##### BSD specific variables #####

# Link
LDFLAGS_BSDCL=$(LDFLAGS_UNIXCOMMON) -lutil $(LDFLAGS_UNIXCL)
LDFLAGS_BSDSV=$(LDFLAGS_UNIXCOMMON) 
LDFLAGS_BSDSDL=$(LDFLAGS_UNIXCOMMON) $(LDFLAGS_UNIXSDL)


##### Win32 specific variables #####

WINDRES ?= windres

OBJ_WGL= builddate.c sys_win.o vid_wgl.o thread_null.o $(OBJ_MENU) $(OBJ_SND_WIN) $(OBJ_WINCD) $(OBJ_VIDEO_CAPTURE) $(OBJ_COMMON)

# Link
# see LDFLAGS_WINCOMMON in makefile
LDFLAGS_WINCL=$(LDFLAGS_WINCOMMON) $(LIB_CRYPTO) $(LIB_CRYPTO_RIJNDAEL) -static-libgcc -mwindows -lwinmm -luser32 -lgdi32 -ldxguid -ldinput -lcomctl32 -lws2_32 $(LDFLAGS_D3D) $(LIB_Z) $(LIB_JPEG)
LDFLAGS_WINSV=$(LDFLAGS_WINCOMMON) $(LIB_CRYPTO) $(LIB_CRYPTO_RIJNDAEL) -static-libgcc -mconsole -lwinmm -lws2_32 $(LIB_Z) $(LIB_JPEG)
LDFLAGS_WINSDL=$(LDFLAGS_WINCOMMON) $(LIB_CRYPTO) $(LIB_CRYPTO_RIJNDAEL) $(SDLCONFIG_LIBS) -static-libgcc -lwinmm -lws2_32 $(LIB_Z) $(LIB_JPEG)
EXE_WINCL=Wrath.exe
EXE_WINSV=wrath-dedicated.exe
EXE_WINSDL=Wrath.exe
EXE_WINCLNEXUIZ=nexuiz.exe
EXE_WINSVNEXUIZ=nexuiz-dedicated.exe
EXE_WINSDLNEXUIZ=nexuiz-sdl.exe

VPATH := ../../../

##### Commands #####

.PHONY : clean clean-profile help \
	 debug profile release \
	 cl-debug cl-profile cl-release \
	 sv-debug sv-profile sv-release \
	 sdl-debug sdl-profile sdl-release

help:
	@echo "TARGET $(DP_MAKE_TARGET) ARCH $(DP_MACHINE)"
	@echo
	@echo "===== Choose one ====="
	@echo "* $(MAKE) clean                : delete all files produced by the build except"
	@echo "                              profiling information"
	@echo "* $(MAKE) clean-profile        : delete all files produced by the build, including"
	@echo "                              profiling informaiton"
	@echo "* $(MAKE) help                 : this help"
	@echo "* $(MAKE) debug                : make client and server binaries (debug versions)"
	@echo "* $(MAKE) profile              : make client and server binaries (profile versions)"
	@echo "* $(MAKE) release              : make client and server binaries (release versions)"
	@echo "* $(MAKE) release-profile      : make client and server binaries (release versions)"
	@echo "                              (with profiling optimizations) The profiled"
	@echo "                              version of the program must have been"
	@echo "                              previously compiled"
	@echo "* $(MAKE) nexuiz               : make client and server binaries with nexuiz icon"
	@echo "                              (release versions)"
	@echo "* $(MAKE) cl-debug             : make client (debug version)"
	@echo "* $(MAKE) cl-profile           : make client (profile version)"
	@echo "* $(MAKE) cl-release-profile   : make client (release profile version)"
	@echo "* $(MAKE) cl-release           : make client (release version)"
	@echo "* $(MAKE) cl-nexuiz            : make client with nexuiz icon (release version)"
	@echo "* $(MAKE) sv-debug             : make dedicated server (debug version)"
	@echo "* $(MAKE) sv-profile           : make dedicated server (profile version)"
	@echo "* $(MAKE) sv-release-profile   : make dedicated server (release profile version)"
	@echo "* $(MAKE) sv-release           : make dedicated server (release version)"
	@echo "* $(MAKE) sv-nexuiz            : make dedicated server with nexuiz icon"
	@echo "                              (release version)"
	@echo "* $(MAKE) sdl-debug            : make SDL client (debug version)"
	@echo "* $(MAKE) sdl-profile          : make SDL client (profile version)"
	@echo "* $(MAKE) sdl-release-profile  : make SDL client (release version)"
	@echo "* $(MAKE) sdl-release          : make SDL client (release version)"
	@echo "* $(MAKE) sdl-nexuiz           : make SDL client with nexuiz icon (release version)"
	@echo "* $(MAKE) sdl2-debug           : make SDL2 client (debug version)"
	@echo "* $(MAKE) sdl2-profile         : make SDL2 client (profile version)"
	@echo "* $(MAKE) sdl2-release-profile : make SDL2 client (release profile version)"
	@echo "* $(MAKE) sdl2-release         : make SDL2 client (release version)"
	@echo "* $(MAKE) sdl2-nexuiz          : make SDL2 client with nexuiz icon (release version)"
	@echo

debug :
	$(MAKE) $(TARGETS_DEBUG)

profile :
	$(MAKE) $(TARGETS_PROFILE)

release :
	$(MAKE) $(TARGETS_RELEASE)

release-profile :
	$(MAKE) $(TARGETS_RELEASE_PROFILE)

nexuiz :
	$(MAKE) $(TARGETS_NEXUIZ)

cl-debug :
	$(MAKE) bin-debug \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_CL)' CFLAGS_FEATURES='$(CFLAGS_CLIENT)' LDFLAGS_COMMON='$(LDFLAGS_CL)' LEVEL=1

cl-profile :
	$(MAKE) bin-profile \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_CL)' CFLAGS_FEATURES='$(CFLAGS_CLIENT)' LDFLAGS_COMMON='$(LDFLAGS_CL)' LEVEL=1

cl-release :
	$(MAKE) bin-release \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_CL)' CFLAGS_FEATURES='$(CFLAGS_CLIENT)' LDFLAGS_COMMON='$(LDFLAGS_CL)' LEVEL=1

cl-release-profile :
	$(MAKE) bin-release-profile \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_CL)' CFLAGS_FEATURES='$(CFLAGS_CLIENT)' LDFLAGS_COMMON='$(LDFLAGS_CL)' LEVEL=1

cl-nexuiz :
	$(MAKE) bin-release \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_CLNEXUIZ)' CFLAGS_FEATURES='$(CFLAGS_CLIENT)' LDFLAGS_COMMON='$(LDFLAGS_CL)' LEVEL=1

sv-debug :
	$(MAKE) bin-debug \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_SV)' CFLAGS_FEATURES='$(CFLAGS_SERVER)' LDFLAGS_COMMON='$(LDFLAGS_SV)' LEVEL=1

sv-profile :
	$(MAKE) bin-profile \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_SV)' CFLAGS_FEATURES='$(CFLAGS_SERVER)' LDFLAGS_COMMON='$(LDFLAGS_SV)' LEVEL=1

sv-release :
	$(MAKE) bin-release \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_SV)' CFLAGS_FEATURES='$(CFLAGS_SERVER)' LDFLAGS_COMMON='$(LDFLAGS_SV)' LEVEL=1

sv-release-profile :
	$(MAKE) bin-release-profile \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_SV)' CFLAGS_FEATURES='$(CFLAGS_SERVER)' LDFLAGS_COMMON='$(LDFLAGS_SV)' LEVEL=1

sv-nexuiz :
	$(MAKE) bin-release \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_SVNEXUIZ)' CFLAGS_FEATURES='$(CFLAGS_CLIENT)' LDFLAGS_COMMON='$(LDFLAGS_SV)' LEVEL=1

sdl-debug :
	$(MAKE) bin-debug \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_SDL)' CFLAGS_FEATURES='$(CFLAGS_CLIENT)' LDFLAGS_COMMON='$(LDFLAGS_SDL)' LEVEL=1

sdl-profile :
	$(MAKE) bin-profile \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_SDL)' CFLAGS_FEATURES='$(CFLAGS_CLIENT)' LDFLAGS_COMMON='$(LDFLAGS_SDL)' LEVEL=1

sdl-release :
	$(MAKE) bin-release \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_SDL)' CFLAGS_FEATURES='$(CFLAGS_CLIENT)' LDFLAGS_COMMON='$(LDFLAGS_SDL)' LEVEL=1

sdl-release-profile :
	$(MAKE) bin-release-profile \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_SDL)' CFLAGS_FEATURES='$(CFLAGS_CLIENT)' LDFLAGS_COMMON='$(LDFLAGS_SDL)' LEVEL=1

sdl-nexuiz :
	$(MAKE) bin-release \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		EXE='$(EXE_SDLNEXUIZ)' CFLAGS_FEATURES='$(CFLAGS_CLIENT)' LDFLAGS_COMMON='$(LDFLAGS_SDL)' LEVEL=1

sdl2-debug :
	$(MAKE) SDL_CONFIG=$(SDL2_CONFIG) sdl-debug

sdl2-profile :
	$(MAKE) SDL_CONFIG=$(SDL2_CONFIG) sdl-profile

sdl2-release :
	$(MAKE) SDL_CONFIG=$(SDL2_CONFIG) sdl-release

sdl2-release-profile :
	$(MAKE) SDL_CONFIG=$(SDL2_CONFIG) sdl-release-profile

sdl2-nexuiz :
	$(MAKE) SDL_CONFIG=$(SDL2_CONFIG) sdl-nexuiz

bin-debug :
	$(CHECKLEVEL1)
	@echo
	@echo '========== $(EXE) (debug) =========='
	$(MAKE) prepare BUILD_DIR=build-obj/debug/$(EXE)
	$(MAKE) -C build-obj/debug/$(EXE) $(EXE) \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		CFLAGS='$(CFLAGS_COMMON) $(CFLAGS_FEATURES) $(CFLAGS_EXTRA) $(CFLAGS_DEBUG) $(OPTIM_DEBUG)'\
		LDFLAGS='$(LDFLAGS_DEBUG) $(LDFLAGS_COMMON)' LEVEL=2

bin-profile :
	$(CHECKLEVEL1)
	@echo
	@echo '========== $(EXE) (profile) =========='
	$(MAKE) prepare BUILD_DIR=build-obj/profile/$(EXE)
	$(MAKE) -C build-obj/profile/$(EXE) $(EXE) \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		CFLAGS='$(CFLAGS_COMMON) $(CFLAGS_FEATURES) $(CFLAGS_EXTRA) $(CFLAGS_PROFILE) $(OPTIM_RELEASE)'\
		LDFLAGS='$(LDFLAGS_PROFILE) $(LDFLAGS_COMMON)' LEVEL=2

bin-release :
	$(CHECKLEVEL1)
	@echo
	@echo '========== $(EXE) (release) =========='
	$(MAKE) prepare BUILD_DIR=build-obj/release/$(EXE)
	$(MAKE) -C build-obj/release/$(EXE) $(EXE) \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		CFLAGS='$(CFLAGS_COMMON) $(CFLAGS_FEATURES) $(CFLAGS_EXTRA) $(CFLAGS_RELEASE) $(OPTIM_RELEASE)'\
		LDFLAGS='$(LDFLAGS_RELEASE) $(LDFLAGS_COMMON)' LEVEL=2
	$(STRIP) $(EXE)

bin-release-profile :
	$(CHECKLEVEL1)
	@echo
	@echo '========== $(EXE) (release) =========='
	$(MAKE) prepare BUILD_DIR=build-obj/release-profile/$(EXE)
	$(MAKE) -C build-obj/release-profile/$(EXE) $(EXE) \
		DP_MAKE_TARGET=$(DP_MAKE_TARGET) DP_SOUND_API=$(DP_SOUND_API) \
		CFLAGS='$(CFLAGS_COMMON) $(CFLAGS_FEATURES) $(CFLAGS_EXTRA) $(CFLAGS_RELEASE_PROFILE) $(OPTIM_RELEASE)'\
		LDFLAGS='$(LDFLAGS_RELEASE) $(LDFLAGS_COMMON)' LEVEL=2
	$(STRIP) $(EXE)

prepare :
	$(CMD_MKDIR) $(BUILD_DIR)
	$(CMD_CP) makefile.inc $(BUILD_DIR)/
	$(CMD_CP) $(MAKEFILE) $(BUILD_DIR)/

#this checks USEODE when compiling so it needs the ODE flags as well
prvm_cmds.o: prvm_cmds.c
	$(CHECKLEVEL2)
	$(DO_CC) $(CFLAGS_ODE)

world.o: world.c
	$(CHECKLEVEL2)
	$(DO_CC) $(CFLAGS_ODE)

keysym2ucs.o: keysym2ucs.c
	$(CHECKLEVEL2)
	$(DO_CC) -I/usr/X11R6/include

vid_sdl.o: vid_sdl.c
	$(CHECKLEVEL2)
	$(DO_CC) $(CFLAGS_SDL)

sys_sdl.o: sys_sdl.c
	$(CHECKLEVEL2)
	$(DO_CC) $(CFLAGS_SDL)

snd_sdl.o: snd_sdl.c
	$(CHECKLEVEL2)
	$(DO_CC) $(CFLAGS_SDL)

thread_sdl.o: thread_sdl.c
	$(CHECKLEVEL2)
	$(DO_CC) $(CFLAGS_SDL)

snd_3dras.o: snd_3dras.c
	$(CHECKLEVEL2)
	$(DO_CC)

cd_sdl.o: cd_sdl.c
	$(CHECKLEVEL2)
	$(DO_CC) $(CFLAGS_SDL)

crypto.o: crypto.c
	$(CHECKLEVEL2)
	$(DO_CC) $(CFLAGS_CRYPTO) $(CFLAGS_CRYPTO_RIJNDAEL)

mod_skeletal_animatevertices_sse.o: mod_skeletal_animatevertices_sse.c
	$(CHECKLEVEL2)
	$(DO_CC) $(CFLAGS_SSE)

dpsoftrast.o: dpsoftrast.c
	$(CHECKLEVEL2)
	$(DO_CC) $(CFLAGS_SSE2)

darkplaces.o: %.o : %.rc
	$(CHECKLEVEL2)
	$(WINDRES) -o $@ $<

nexuiz.o: %.o : %.rc
	$(CHECKLEVEL2)
	$(WINDRES) -o $@ $<

.c.o:
	$(CHECKLEVEL2)
	$(DO_CC)

$(EXE_CL): $(OBJ_CL) $(OBJ_ICON)
	$(CHECKLEVEL2)
	$(DO_LD)

$(EXE_SV): $(OBJ_SV) $(OBJ_ICON)
	$(CHECKLEVEL2)
	$(DO_LD)

$(EXE_SDL): $(OBJ_SDL) $(OBJ_ICON)
	$(CHECKLEVEL2)
	$(DO_LD)

$(EXE_CLNEXUIZ): $(OBJ_CL) $(OBJ_ICON_NEXUIZ)
	$(CHECKLEVEL2)
	$(DO_LD)

$(EXE_SVNEXUIZ): $(OBJ_SV) $(OBJ_ICON_NEXUIZ)
	$(CHECKLEVEL2)
	$(DO_LD)

$(EXE_SDLNEXUIZ): $(OBJ_SDL) $(OBJ_ICON_NEXUIZ)
	$(CHECKLEVEL2)
	$(DO_LD)

clean:
	-$(CMD_RM) $(EXE_CL)
	-$(CMD_RM) $(EXE_SV)
	-$(CMD_RM) $(EXE_SDL)
	-$(CMD_RM) $(EXE_CLNEXUIZ)
	-$(CMD_RM) $(EXE_SVNEXUIZ)
	-$(CMD_RM) $(EXE_SDLNEXUIZ)
	-$(CMD_RM) *.o
	-$(CMD_RM) *.d
	-$(CMD_RM) build-obj/

clean-profile: clean
	-$(CMD_RM) *.gcda
	-$(CMD_RM) *.gcno
