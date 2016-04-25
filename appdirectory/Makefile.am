noinst_HEADERS = BaseGame.h Character.h GameManager.h GUISystem.h HUD.h HUDListener.h Ninja.h RenderingEngine.h SingleplayerGame.h ParticleType.h Plane.h Player.h PlayerBank.h PlayerInfo.h GameObject.h SoundBank.h SoundController.h

bin_PROGRAMS = Battle
Battle_SOURCES = BaseGame.cpp Character.cpp GameManager.cpp GameObject.cpp GUISystem.cpp HUD.cpp Main.cpp Ninja.cpp RenderingEngine.cpp SingleplayerGame.cpp Plane.cpp Player.cpp PlayerBank.cpp SoundBank.cpp SoundController.cpp

Battle_CPPFLAGS = -I$(top_srcdir) -std=c++11 -Wall -Werror -Wfatal-errors 
Battle_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(shell sdl-config --cflags) $(CEGUI_CFLAGS)
Battle_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS)
Battle_LDFLAGS = -lOgreOverlay -lboost_system -lboost_serialization -lSDL -lSDL_mixer -lSDL_net -R/lusr/lib/cegui-0.8

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
