noinst_HEADERS = BaseGame.h GameManager.h GUISystem.h RenderingEngine.h SingleplayerGame.h Plane.h Player.h GameObject.h SoundBank.h Character.h

bin_PROGRAMS = Battle
Battle_SOURCES = BaseGame.cpp GameManager.cpp GUISystem.cpp Main.cpp RenderingEngine.cpp SingleplayerGame.cpp Plane.cpp Player.cpp GameObject.cpp SoundBank.cpp Character.cpp

Battle_CPPFLAGS = -I$(top_srcdir) -std=c++11 -Wall
Battle_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(shell sdl-config --cflags) $(CEGUI_CFLAGS)
Battle_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS)
Battle_LDFLAGS = -lOgreOverlay -lboost_system -lboost_serialization -lSDL -lSDL_mixer -lSDL_net -R/lusr/lib/cegui-0.8

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
