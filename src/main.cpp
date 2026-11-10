#include "raylib.h"

int main() {
    InitWindow(800, 600, "Entorno 3D - Simulacion");
    SetTargetFPS(60);

    // Configuración básica de cámara 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(camera);
            DrawGrid(10, 1.0f); // Dibuja una grilla base
            DrawCube((Vector3){0, 0, 0}, 2.0f, 2.0f, 2.0f, RED); // Cubo de prueba
            DrawCubeWires((Vector3){0, 0, 0}, 2.0f, 2.0f, 2.0f, MAROON);
        EndMode3D();

        DrawText("Entorno configurado correctamente", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

