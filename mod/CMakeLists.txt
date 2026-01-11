# Requiere una versión moderna de CMake
cmake_minimum_required(VERSION 3.16...3.31)

# Nombre del proyecto (usa el mismo que tu mod)
project(AutoSave)

# Configuración estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# GEODE_SDK_ROOT debe pasarse desde CI o configurarse localmente
if(NOT DEFINED GEODE_SDK_ROOT)
  message(FATAL_ERROR "GEODE_SDK_ROOT is not defined. Pass -DGEODE_SDK_ROOT=/path/to/geode-sdk")
endif()

# Incluir el helper oficial del SDK (fallará si GEODE_SDK_ROOT es incorrecto)
include("${GEODE_SDK_ROOT}/cmake/Geode.cmake")

# Definir el mod con fuentes y cabeceras
geode_add_mod(AutoSave
    SOURCES
        ${CMAKE_SOURCE_DIR}/main.cpp
        src/AutoSave.cpp
        src/SettingsLayer.cpp
    HEADERS
        src/AutoSave.hpp
        src/SettingsLayer.hpp
    RESOURCES
        mod.json
        assets/icon.png       # añade aquí tus recursos reales
        assets/strings.json   # ejemplo de archivo de recursos
)

# Forzar nombre de salida conocido (opcional)
set_target_properties(AutoSave PROPERTIES OUTPUT_NAME "AutoSave")
