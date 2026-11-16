// // src/main.cpp
// #include <iostream>
// #include <cmath>
// #include "raylib.h"
// #include "raymath.h"
// #include "Point.h"
// #include "Vector.h"
// #include "Room.h"
// #include "Source.h"

// // --- Conversión de nuestras clases a Raylib ---
// static Vector3 toV3(const Point& p)  { return Vector3{(float)p.x, (float)p.y, (float)p.z}; }
// static Vector3 toV3(const Vector& v) { return Vector3{(float)v.x, (float)v.y, (float)v.z}; }

// static void DrawArrow3D(Vector3 start, Vector3 end, Color color,
//                         float headLen = 0.18f, float headRadius = 0.06f,
//                         float bodyRadius = 0.0f) {
//     Vector3 dir = Vector3Subtract(end, start);
//     float len = Vector3Length(dir);
//     if (len < 1e-6f) return;

//     if (len <= headLen) {
//         DrawCylinderEx(start, end, headRadius, 0.0f, 12, color);
//         return;
//     }

//     Vector3 unit = Vector3Scale(dir, 1.0f / len);
//     Vector3 base = Vector3Subtract(end, Vector3Scale(unit, headLen));

//     if (bodyRadius > 0.0f) DrawCylinderEx(start, base, bodyRadius, bodyRadius, 8, color);
//     else                   DrawLine3D(start, base, color);
//     DrawCylinderEx(base, end, headRadius, 0.0f, 12, color);
// }

// static void drawRoomWire(Room& room, Color color) {
//     for (int f = 0; f < 6; f++) {
//         const Plane& pl = room.faces[f];
//         for (int i = 0; i < 4; i++) {
//             DrawLine3D(toV3(pl.v[i]), toV3(pl.v[(i + 1) % 4]), color);
//         }
//     }
// }

// // Normales unitarias desde el centro de cada cara hacia el interior
// // static void drawRoomNormals(Room& room, float len, Color color) {
// //     for (int f = 0; f < 6; f++) {
// //         Point  c = room.faces[f].center();
// //         Vector n = room.faces[f].unitNormal();
// //         Vector3 a = toV3(c);
// //         Vector3 b = { a.x + (float)n.x * len,
// //                       a.y + (float)n.y * len,
// //                       a.z + (float)n.z * len };
// //         DrawArrow3D(a, b, color);
// //     }
// // }


// static void drawRoomNormals(Room& room, float len, Color color) {
//     for (int f = 0; f < 6; f++) {
//         Point  c = room.faces[f].center();
//         Vector n = room.faces[f].unitNormal();   // vector de longitud 1
//         Vector3 a = toV3(c);
//         Vector3 b = { a.x + (float)n.x * len,
//                       a.y + (float)n.y * len,
//                       a.z + (float)n.z * len };
//         DrawArrow3D(a, b, color, 0.22f, 0.08f, 0.015f);   // cuerpo grueso + cono
//     }
// }




// // Ejes cartesianos gruesos cruzándose en (0,0,0)
// static void drawAxes(float length, float thickness) {
//     Vector3 o = {0.0f, 0.0f, 0.0f};
//     DrawCylinderEx(Vector3{-length, 0, 0}, Vector3{length, 0, 0}, thickness, thickness, 8, RED);    // X
//     DrawCylinderEx(Vector3{0, -length, 0}, Vector3{0, length, 0}, thickness, thickness, 8, GREEN);  // Y
//     DrawCylinderEx(Vector3{0, 0, -length}, Vector3{0, 0, length}, thickness, thickness, 8, BLUE);   // Z
//     DrawSphere(o, thickness * 1.8f, BLACK);   // marca del origen
// }

// int main() {
//     // --- Entorno de simulación ---
//     Room  room(4.0);                    // cubo de -2 a 2
//     Point receptor(-1.5, 0.0, -1.0);

//     Source src;
//     src.p = Point(1.5, 0.0, 1.0);
//     src.createRays(12);

//     // implementacion de los reflejos
//         // --- Precálculo de rutas: fuente -> impacto 1 -> ... (máx. 3 rebotes) ---
//     const int MAX_BOUNCES = 3;

//     struct RayPath {
//         Point pts[MAX_BOUNCES + 2];   // fuente + 1 impacto inicial + 3 rebotes = 5 puntos
//         int   n;                      // cantidad de puntos válidos
//     };

//     RayPath* paths = new RayPath[src.NRAYS];

//     for (int i = 0; i < src.NRAYS; i++) {
//         Point  p0 = src.p;                        // origen actual
//         Vector vi = src.Rays[i].normalized();     // dirección actual

//         paths[i].n = 0;
//         paths[i].pts[paths[i].n++] = p0;          // vértice 0: la fuente

//         int bounces = 0;
//         while (bounces <= MAX_BOUNCES) {
//             Point hit;
//             int   faceIdx = -1;
//             if (!room.firstHit(p0, vi, hit, &faceIdx)) break;

//             paths[i].pts[paths[i].n++] = hit;     // guardar punto de impacto

//             vi = vi.reflect(room.faces[faceIdx].normal);   // vi = vi - ni*2*(vi·ni)
//             p0 = hit;                                      // p0 = pi
//             bounces++;
//         }
//     }

//     // --- Verificación de normales por consola ---
//     const char* names[6] = {"Piso", "Techo", "Izquierda", "Derecha", "Fondo", "Frente"};
//     std::cout << "Normales de las caras (deben apuntar hacia el interior):\n";
//     for (int f = 0; f < 6; f++) {
//         const Vector& n = room.faces[f].normal;
//         std::cout << "  " << names[f] << ": (" << n.x << ", " << n.y << ", " << n.z << ")\n";
//     }
//     std::cout << std::endl;

//     // --- Ventana ---
//     // Añade esta línea JUSTO ANTES de InitWindow
//     SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI); 
//     InitWindow(1000, 700, "Simulacion - Sala, Fuente y Receptor");
//     SetTargetFPS(60);

//     // --- Fuente personalizada (debe cargarse después de InitWindow) ---
//     Font font = LoadFontEx("fuente.ttf", 24, 0, 0);
//     SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
//     bool customFont = (font.texture.id != 0);
//     if (customFont) SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
//     else            font = GetFontDefault();   // respaldo si no encuentra el .ttf

//     // --- Cámara orbital con mouse ---
//     float yaw   = 45.0f * DEG2RAD;
//     float pitch = 35.0f * DEG2RAD;
//     float dist  = 11.0f;

//     Camera3D camera = {};
//     camera.target     = Vector3{0.0f, 0.0f, 0.0f};
//     camera.up         = Vector3{0.0f, 1.0f, 0.0f};
//     camera.fovy       = 45.0f;
//     camera.projection = CAMERA_PERSPECTIVE;

//     // Longitud total de la ruta más larga (para el ciclo de la animación)
//     float max_path_length = 0.0f;
//     for (int i = 0; i < src.NRAYS; i++) {
//         float L = 0.0f;
//         for (int k = 0; k + 1 < paths[i].n; k++)
//             L += Vector3Distance(toV3(paths[i].pts[k]), toV3(paths[i].pts[k + 1]));
//         if (L > max_path_length) max_path_length = L;
//     }

//     while (!WindowShouldClose()) {
//         // Clic izquierdo + arrastrar = rotar; rueda = zoom
//         if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
//             Vector2 d = GetMouseDelta();
//             yaw   -= d.x * 0.005f;
//             pitch += d.y * 0.005f;
//             if (pitch >  1.5f) pitch =  1.5f;
//             if (pitch < -1.5f) pitch = -1.5f;
//         }
//         dist -= GetMouseWheelMove() * 0.5f;
//         if (dist < 3.0f)  dist = 3.0f;
//         if (dist > 30.0f) dist = 30.0f;

//         camera.position = Vector3{
//             dist * cosf(pitch) * sinf(yaw),
//             dist * sinf(pitch),
//             dist * cosf(pitch) * cosf(yaw)
//         };

//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//             BeginMode3D(camera);
            
//             // 1. Sistema de referencia (cuadrícula en el centro y ejes cortos)
//             DrawGrid(10, 1.0f);                 
//             drawAxes(0.5f, 0.02f);              

//             // 2. Dibujar la estructura del cubo y sus normales unitarias
//             drawRoomWire(room, BLACK);
//             drawRoomNormals(room, 1.0f, DARKGREEN);

//             // 3. Dibujar los puntos espaciales (Fuente y Receptor)
//             DrawSphere(toV3(src.p), 0.1f, RED);
//             DrawSphere(toV3(receptor), 0.1f, BLUE);

//             // // 4. Rayos incidentes: longitud fija (4 unidades), sin calcular colisiones por ahora
//             // const float RAY_LENGTH = 4.0f;
//             // Vector3 origin = toV3(src.p);
            
//             // for (int i = 0; i < src.NRAYS; i++) {
//             //     Vector d = src.Rays[i].normalized();
//             //     Vector3 end = { origin.x + (float)d.x * RAY_LENGTH,
//             //                     origin.y + (float)d.y * RAY_LENGTH,
//             //                     origin.z + (float)d.z * RAY_LENGTH };
//             //     DrawArrow3D(origin, end, ORANGE);
//             // }
                        
//             // 4. Rayos incidentes animados: pulsos que nacen en la fuente y crecen hasta 4 unidades
//             const float MAX_LENGTH = 4.0f;
//             const float SPEED      = 1.5f;                                  // unidades por segundo
//             float currentLength    = fmodf((float)GetTime() * SPEED, MAX_LENGTH);  // 0 -> 4, luego reinicia a 0

//             Vector3 origin = toV3(src.p);


//             // Este for dibuja los rayos sin rebote, atraviesan el cubo
//             // for (int i = 0; i < src.NRAYS; i++) {
//             //     Vector d = src.Rays[i].normalized();
//             //     Vector3 end = { origin.x + (float)d.x * currentLength,
//             //                     origin.y + (float)d.y * currentLength,
//             //                     origin.z + (float)d.z * currentLength };
//             //     DrawArrow3D(origin, end, ORANGE);
//             // }

//             // // 4. Rayos con rebotes precalculados, estático y sin animación
//             // for (int i = 0; i < src.NRAYS; i++) {
//             //     for (int k = 0; k + 1 < paths[i].n; k++) {
//             //         Vector3 a = toV3(paths[i].pts[k]);
//             //         Vector3 b = toV3(paths[i].pts[k + 1]);
//             //         Color   c = (k == 0) ? ORANGE : PURPLE;   // 1er trayecto naranja, rebotes morados

//             //         if (k + 2 == paths[i].n) DrawArrow3D(a, b, c);   // flecha solo en el último tramo
//             //         else                     DrawLine3D(a, b, c);
//             //     }
//             // }

            
//             // 4. Animación d = v·t: los rayos recorren sus rutas precalculadas
//             // con animacion y rebotes
//             double t     = GetTime();
//             float  speed = 8.0f;                                          // unidades por segundo
//             float  total_dist_global = fmodf((float)t * speed, max_path_length);

//             for (int i = 0; i < src.NRAYS; i++) {
//                 float total_dist = total_dist_global;                     // distancia restante para este rayo

//                 for (int k = 0; k + 1 < paths[i].n; k++) {
//                     Vector3 a = toV3(paths[i].pts[k]);
//                     Vector3 b = toV3(paths[i].pts[k + 1]);
//                     Color   c = (k == 0) ? ORANGE : PURPLE;               // 1er tramo naranja, rebotes morados

//                     float segLen = Vector3Distance(a, b);
//                     if (segLen < 1e-6f) continue;

//                     if (total_dist > segLen) {
//                         DrawArrow3D(a, b, c);                             // segmento completo
//                         total_dist -= segLen;
//                     } else {
//                         Vector3 partial = Vector3Lerp(a, b, total_dist / segLen);
//                         DrawArrow3D(a, partial, c);                       // segmento parcial
//                         break;                                            // fin de este rayo
//                     }
//                 }
//             }


//         EndMode3D();


//         DrawTextEx(font, "Rojo: fuente | Azul: receptor | Verde: normales",
//                    Vector2{10, 10}, 24, 1, DARKGRAY);
//         DrawTextEx(font, "Naranja: rayos incidentes | Morado: rebotes",
//                    Vector2{10, 38}, 24, 1, BLACK);
//         DrawTextEx(font, TextFormat("Rayos: %d", src.NRAYS),
//                    Vector2{10, 66}, 24, 1, DARKGRAY);
//         DrawTextEx(font, "Clic izq + arrastrar: rotar | Rueda: zoom",
//                    Vector2{10, 94}, 20, 1, GRAY);
//         EndDrawing();
//     }

//     if (customFont) UnloadFont(font);
//     delete[] paths;
//     CloseWindow();
//     return 0;
// }



// src/main.cpp
#include <iostream>
#include <cmath>
#include "raylib.h"
#include "raymath.h"
#include "Point.h"
#include "Vector.h"
#include "Room.h"
#include "Source.h"
#include "Config.h" // <-- Tu nuevo archivo de configuración

// --- Conversión de nuestras clases a Raylib ---
static Vector3 toV3(const Point& p)  { return Vector3{(float)p.x, (float)p.y, (float)p.z}; }
static Vector3 toV3(const Vector& v) { return Vector3{(float)v.x, (float)v.y, (float)v.z}; }

static void DrawArrow3D(Vector3 start, Vector3 end, Color color,
                        float headLen = 0.18f, float headRadius = 0.06f,
                        float bodyRadius = 0.0f) {
    Vector3 dir = Vector3Subtract(end, start);
    float len = Vector3Length(dir);
    if (len < 1e-6f) return;

    if (len <= headLen) {
        DrawCylinderEx(start, end, headRadius, 0.0f, 12, color);
        return;
    }

    Vector3 unit = Vector3Scale(dir, 1.0f / len);
    Vector3 base = Vector3Subtract(end, Vector3Scale(unit, headLen));

    if (bodyRadius > 0.0f) DrawCylinderEx(start, base, bodyRadius, bodyRadius, 8, color);
    else                   DrawLine3D(start, base, color);
    DrawCylinderEx(base, end, headRadius, 0.0f, 12, color);
}

static void drawRoomWire(Room& room, Color color) {
    for (int f = 0; f < 6; f++) {
        const Plane& pl = room.faces[f];
        for (int i = 0; i < 4; i++) {
            DrawLine3D(toV3(pl.v[i]), toV3(pl.v[(i + 1) % 4]), color);
        }
    }
}

// Normales unitarias desde el centro de cada cara hacia el interior
static void drawRoomNormals(Room& room, float len, Color color) {
    for (int f = 0; f < 6; f++) {
        Point  c = room.faces[f].center();
        Vector n = room.faces[f].unitNormal();   // vector de longitud 1
        Vector3 a = toV3(c);
        Vector3 b = { a.x + (float)n.x * len,
                      a.y + (float)n.y * len,
                      a.z + (float)n.z * len };
        DrawArrow3D(a, b, color, 0.22f, 0.08f, 0.015f);   // cuerpo grueso + cono
    }
}

// Ejes cartesianos gruesos cruzándose en (0,0,0)
static void drawAxes(float length, float thickness) {
    Vector3 o = {0.0f, 0.0f, 0.0f};
    DrawCylinderEx(Vector3{-length, 0, 0}, Vector3{length, 0, 0}, thickness, thickness, 8, RED);    // X
    DrawCylinderEx(Vector3{0, -length, 0}, Vector3{0, length, 0}, thickness, thickness, 8, GREEN);  // Y
    DrawCylinderEx(Vector3{0, 0, -length}, Vector3{0, 0, length}, thickness, thickness, 8, BLUE);   // Z
    DrawSphere(o, thickness * 1.8f, BLACK);   // marca del origen
}

int main() {
    // --- Entorno de simulación (Usando Config.h) ---
    Room  room(Config::ROOM_SIZE);                    
    Point receptor(Config::RECEPTOR_X, Config::RECEPTOR_Y, Config::RECEPTOR_Z);

    Source src;
    src.p = Point(Config::SOURCE_X, Config::SOURCE_Y, Config::SOURCE_Z);
    src.createRays(Config::NUM_RAYS);

    // --- Precálculo de rutas con choques internos ---
    struct RayPath {
        Point pts[Config::MAX_BOUNCES + 2];   // fuente + 1 impacto inicial + rebotes
        int   n;                              // cantidad de puntos válidos
    };

    RayPath* paths = new RayPath[src.NRAYS];

    for (int i = 0; i < src.NRAYS; i++) {
        Point  p0 = src.p;                        
        Vector vi = src.Rays[i].normalized();     

        paths[i].n = 0;
        paths[i].pts[paths[i].n++] = p0;          

        int bounces = 0;
        while (bounces <= Config::MAX_BOUNCES) {
            Point hit;
            int   faceIdx = -1;
            if (!room.firstHit(p0, vi, hit, &faceIdx)) break;

            paths[i].pts[paths[i].n++] = hit;     

            vi = vi.reflect(room.faces[faceIdx].normal);   
            p0 = hit;                                      
            bounces++;
        }
    }

    // --- Verificación de normales por consola ---
    const char* names[6] = {"Piso", "Techo", "Izquierda", "Derecha", "Fondo", "Frente"};
    std::cout << "Normales de las caras (deben apuntar hacia el interior):\n";
    for (int f = 0; f < 6; f++) {
        const Vector& n = room.faces[f].normal;
        std::cout << "  " << names[f] << ": (" << n.x << ", " << n.y << ", " << n.z << ")\n";
    }
    std::cout << std::endl;

    // --- Ventana ---
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI); 
    InitWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, "Simulacion - Sala, Fuente y Receptor");
    SetTargetFPS(Config::TARGET_FPS);

    // --- Fuente personalizada ---
    Font font = LoadFontEx("fuente.ttf", 24, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    bool customFont = (font.texture.id != 0);
    if (!customFont) font = GetFontDefault();

    // --- Cámara orbital con mouse ---
    float yaw   = 45.0f * DEG2RAD;
    float pitch = 35.0f * DEG2RAD;
    float dist  = 11.0f;

    Camera3D camera = {};
    camera.target     = Vector3{0.0f, 0.0f, 0.0f};
    camera.up         = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy       = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // --- Longitud total de la ruta más larga ---
    float max_path_length = 0.0f;
    for (int i = 0; i < src.NRAYS; i++) {
        float L = 0.0f;
        for (int k = 0; k + 1 < paths[i].n; k++)
            L += Vector3Distance(toV3(paths[i].pts[k]), toV3(paths[i].pts[k + 1]));
        if (L > max_path_length) max_path_length = L;
    }

    while (!WindowShouldClose()) {
        // Clic izquierdo + arrastrar = rotar; rueda = zoom
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 d = GetMouseDelta();
            yaw   -= d.x * Config::CAMERA_SPEED;
            pitch += d.y * Config::CAMERA_SPEED;
            if (pitch >  1.5f) pitch =  1.5f;
            if (pitch < -1.5f) pitch = -1.5f;
        }
        dist -= GetMouseWheelMove() * 0.5f;
        if (dist < 3.0f)  dist = 3.0f;
        if (dist > 30.0f) dist = 30.0f;

        camera.position = Vector3{
            dist * cosf(pitch) * sinf(yaw),
            dist * sinf(pitch),
            dist * cosf(pitch) * cosf(yaw)
        };

        BeginDrawing();
        ClearBackground(RAYWHITE);

            BeginMode3D(camera);
            
            DrawGrid(10, 1.0f);                 
            drawAxes(0.5f, 0.02f);              

            drawRoomWire(room, BLACK);
            drawRoomNormals(room, 1.0f, DARKGREEN);

            DrawSphere(toV3(src.p), 0.1f, RED);
            DrawSphere(toV3(receptor), 0.1f, BLUE);
            
            // Animación de los rayos recorriendo sus rutas precalculadas
            double t = GetTime();
            float  total_dist_global = fmodf((float)t * Config::RAY_SPEED, max_path_length);

            for (int i = 0; i < src.NRAYS; i++) {
                float total_dist = total_dist_global;

                for (int k = 0; k + 1 < paths[i].n; k++) {
                    Vector3 a = toV3(paths[i].pts[k]);
                    Vector3 b = toV3(paths[i].pts[k + 1]);
                    Color   c = (k == 0) ? ORANGE : PURPLE;

                    float segLen = Vector3Distance(a, b);
                    if (segLen < 1e-6f) continue;

                    if (total_dist > segLen) {
                        DrawArrow3D(a, b, c);
                        total_dist -= segLen;
                    } else {
                        Vector3 partial = Vector3Lerp(a, b, total_dist / segLen);
                        DrawArrow3D(a, partial, c);
                        break;
                    }
                }
            }

        EndMode3D();

        // --- Indicadores del Sistema de Referencia (+X, +Y, +Z) ---
        // Se proyectan un poco más allá de la longitud de los ejes (0.5f -> 0.7f) para que no se superpongan
        Vector2 labelX = GetWorldToScreen(Vector3{0.7f, 0.0f, 0.0f}, camera);
        Vector2 labelY = GetWorldToScreen(Vector3{0.0f, 0.7f, 0.0f}, camera);
        Vector2 labelZ = GetWorldToScreen(Vector3{0.0f, 0.0f, 0.7f}, camera);

        DrawTextEx(font, "+X", labelX, 14, 0, RED);
        DrawTextEx(font, "+Y", labelY, 14, 0, DARKGREEN); 
        DrawTextEx(font, "+Z", labelZ, 14, 0, BLUE);
        // ----------------------------------------------------------

        DrawTextEx(font, "Rojo: fuente | Azul: receptor | Verde: normales",
                   Vector2{10, 10}, 24, 1, BLACK);
        DrawTextEx(font, "Naranja: rayos incidentes | Morado: reflejos",
                   Vector2{10, 38}, 24, 1, BLACK);
        DrawTextEx(font, TextFormat("Rayos: %d | Rebotes: %d", src.NRAYS, Config::MAX_BOUNCES),
                   Vector2{10, 66}, 24, 1, BLACK);
        DrawTextEx(font, "Clic izq + arrastrar: rotar | Rueda: zoom",
                   Vector2{10, 94}, 20, 1, BLACK);
        EndDrawing();
    }

    if (customFont) UnloadFont(font);
    delete[] paths;
    CloseWindow();
    return 0;
}
