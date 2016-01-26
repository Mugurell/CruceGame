cmake_minimum_required(VERSION 3.3)

project(CruceGame_clion)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -lncursesw -pthread -g")

set(CRUCE_SOURCE_FILES
        src/main.c
        src/cli/cli.c
        src/libCruceGame/deck.c src/libCruceGame/errors.c src/libCruceGame/game.c
        src/libCruceGame/round.c src/libCruceGame/team.c
        src/multiplayerCurses/ircHandlers.c src/multiplayerCurses/multiplayerCurses.c
        src/networking/network.c
        src/parser/parser.c
        src/singlePlayerCurses/singlePlayerCurses.c )

include_directories(${PROJECT_SOURCE_DIR}/src)



# Tell CLion to pass its $(PROJECT_BINARY_DIR) to make
# Comment the row below let CMake compile the project. Not recommended.
add_custom_target(CruceGame_clion COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_SOURCE_DIR}/out)



# Uncomment below rows to use CMake to build the roject. Not reccomended.
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)
#
#add_executable(CruceGame_exe ${CRUCE_SOURCE_FILES})
#
#target_link_libraries(CruceGame_exe "-lncursesw" )
