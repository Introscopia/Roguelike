CC = gcc

OBJS = main_RL.c map.c fundamentals.c SDL2_gfx/SDL2_gfxPrimitives.c SDL2_gfx/SDL2_framerate.c SDL2_gfx/SDL2_rotozoom.c

# OFILES = obj/space.o obj/fundamentals.o obj/vec2d.o SDL2_gfx/SDL2_gfxPrimitives.o SDL2_gfx/SDL2_framerate.o SDL2_gfx/SDL2_rotozoom.o
# $(OFILES)

INCLUDE_PATHS = -IC:/SDL/SDL2-2.0.10/i686-w64-mingw32/include/SDL2
INCLUDE_PATHS += -IC:/SDL/SDL2_image-2.0.5/i686-w64-mingw32/include/SDL2
INCLUDE_PATHS += -IC:/SDL/SDL2_ttf-2.0.15/i686-w64-mingw32/include/SDL2
LIBRARY_PATHS = -LC:/SDL/SDL2-2.0.10/i686-w64-mingw32/lib
LIBRARY_PATHS += -LC:/SDL/SDL2_image-2.0.5/i686-w64-mingw32/lib
LIBRARY_PATHS += -LC:/SDL/SDL2_ttf-2.0.15/i686-w64-mingw32/lib

# -w (suppresses all warnings)
# -Wl,-subsystem (windows gets rid of the console window)
# COMPILER_FLAGS = -w -Wl,-subsystem,windows
# COMPILER_FLAGS = -Wall -Wextra -Werror -O2 -std=c99 -pedantic
# COMPILER_FLAGS = -w
COMPILER_FLAGS = -Wall -g

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

OBJ_NAME = Roguelike

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)