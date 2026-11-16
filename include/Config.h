// include/Config.h
#pragma once

namespace Config {
    // --- Configuración de Ventana ---
    constexpr int   WINDOW_WIDTH  = 1000;
    constexpr int   WINDOW_HEIGHT = 700;
    constexpr int   TARGET_FPS    = 60;
    constexpr float CAMERA_SPEED  = 0.005f; // Sensibilidad del mouse

    // --- Configuración del Entorno (Cubo) ---
    constexpr double ROOM_SIZE = 4.0; 

    // --- Posiciones Espaciales ---
    constexpr double SOURCE_X = 1.5;
    constexpr double SOURCE_Y = 0.0;
    constexpr double SOURCE_Z = 1.0;

    constexpr double RECEPTOR_X = -1.5;
    constexpr double RECEPTOR_Y = 0.0;
    constexpr double RECEPTOR_Z = -1.0;

    // --- Física y Acústica ---
    constexpr int   NUM_RAYS    = 12;   // Rayos omnidireccionales
    constexpr int   MAX_BOUNCES = 4;    // Límite de choques por rayo
    constexpr float RAY_SPEED   = 4.0f; // Velocidad de la animación (unidades por segundo)
}
