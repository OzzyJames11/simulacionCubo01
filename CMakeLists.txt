cmake_minimum_required(VERSION 3.14)
project(SimulacionCubo CXX)

# Forzar el uso de C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Descargar Raylib automáticamente en tiempo de configuración
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master 
)
FetchContent_MakeAvailable(raylib)

# Incluir tu carpeta de cabeceras
include_directories(include)

# Crear el ejecutable con TODAS tus clases
add_executable(SimulacionCubo 
    src/main.cpp 
    src/Vector.cpp 
    src/Point.cpp 
    src/Plane.cpp 
    src/Room.cpp 
    src/Source.cpp 
    src/Intersect.cpp
)

# Vincular Raylib a tu ejecutable
target_link_libraries(SimulacionCubo PRIVATE raylib)

# Configuración específica para macOS
if (APPLE)
    target_link_libraries(SimulacionCubo PRIVATE "-framework IOKit")
    target_link_libraries(SimulacionCubo PRIVATE "-framework Cocoa")
    target_link_libraries(SimulacionCubo PRIVATE "-framework OpenGL")
endif()
