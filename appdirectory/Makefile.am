noinst_HEADERS = AnimationController.h AnimationSpec.h BaseGame.h Character.h DamageIndicatorController.h GameManager.h GUISystem.h HUD.h HUDListener.h Item.h Inventory.h MeshSpec.h MeshSpecBank.h RenderingEngine.h SingleplayerGame.h ParticleController.h ParticleType.h Plane.h Player.h PlayerBank.h PlayerInfo.h GameObject.h SoundBank.h OgreScreenRecorder.h

bin_PROGRAMS = Battle
Battle_SOURCES = AnimationController.cpp BaseGame.cpp Character.cpp DamageIndicatorController.cpp GameManager.cpp GameObject.cpp GUISystem.cpp HUD.cpp Item.cpp Inventory.cpp Main.cpp MeshSpecBank.cpp RenderingEngine.cpp SingleplayerGame.cpp ParticleController.cpp Plane.cpp Player.cpp PlayerBank.cpp SoundBank.cpp OgreScreenRecorder.cpp

Battle_CPPFLAGS = -I$(top_srcdir) -std=c++11 -Wall -Werror -Wfatal-errors 
Battle_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(shell sdl-config --cflags) $(CEGUI_CFLAGS)
Battle_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS)
Battle_LDFLAGS = -lOgreOverlay -lboost_system -lboost_serialization -lSDL -lSDL_mixer -lSDL_net -R/lusr/lib/cegui-0.8

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
