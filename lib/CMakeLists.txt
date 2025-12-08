add_library(game_lib
    Food.cpp
    Game.cpp
    Renderer.cpp
    Snake.cpp
)

target_include_directories(game_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SFML_INCLUDE_DIR})
target_link_libraries(game_lib PUBLIC sfml-graphics sfml-window sfml-system)
