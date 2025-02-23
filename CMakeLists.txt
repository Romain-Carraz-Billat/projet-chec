cmake_minimum_required(VERSION 3.29)
project(ProjetEchec)

set(CMAKE_CXX_STANDARD 20)

# Trouver SFML via la commande find_package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Ajouter les fichiers sources
set(SOURCES
        src/main.cpp
)

# Ajouter l'exécutable
add_executable(ProjetEchec ${SOURCES})

# Lier SFML
target_link_libraries(ProjetEchec sfml-graphics sfml-window sfml-system)

