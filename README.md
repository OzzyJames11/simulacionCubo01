# Simulación de trazado de rayos en una sala cúbica (C++ / Raylib)

Entorno geométrico para acústica de rayos: una sala cúbica de 4×4×4 centrada en el origen, una fuente que emite 12 rayos omnidireccionales y un receptor. Está escrito con programación orientada a objetos (`Point`, `Vector`, `Plane`, `Room`, `Source`) y se visualiza con Raylib.

---

## 1. Qué es del profesor y qué se agregó

| Componente | Origen | Estado |
|---|---|---|
| `Source` (generación de rayos con icosaedro) | **Profesor** | Lógica matemática intacta; solo se corrigió sintaxis y tipos |
| `Vector` (punto `*`, cruz `/`) | Tuyo | Se amplió (escalar, suma, resta, `length`, `normalized`, `reflect`) |
| `Point` (`Point - Point = Vector`) | Tuyo | Se agregó constructor de 3 parámetros |
| `Plane` | Nuevo | 4 vértices + normal por producto cruz con `operator/` |
| `Room` | Nuevo | 6 caras con normales hacia el interior + `firstHit` |
| `Intersect` (rayo–plano) | Nuevo | Implementado, **sin uso en el render actual** |
| `Vector::reflect` | Nuevo | Implementado, **sin uso en el render actual** |
| `main.cpp` (Raylib) | Nuevo | Cámara orbital, ejes, flechas, normales, rayos, UI |

---

## 2. Estructura de archivos

```
include/                src/
  Vector.h                Vector.cpp
  Point.h                 Point.cpp
  Plane.h                 Plane.cpp
  Room.h                  Room.cpp
  Source.h                Source.cpp      <- código del profesor adaptado
  Intersect.h             Intersect.cpp
                          main.cpp
```

Regla de dependencias: `Vector` no depende de nadie. `Point` incluye `Vector` porque `Point::operator-` devuelve un `Vector`. `Plane` incluye ambos. `Room` incluye `Plane` e `Intersect`. `Source` incluye `Point` y `Vector`.

---

## 3. `Source`: el código del profesor

### 3.1 Qué hace

Genera direcciones unitarias distribuidas casi uniformemente sobre una esfera, partiendo de un **icosaedro** (12 vértices, 30 aristas, 20 caras triangulares) y, si se piden más rayos, **subdividiendo** aristas y caras.

`createRays(NumberOfRays)` calcula la frecuencia de subdivisión `n`:

```
n     = round( sqrt((NumberOfRays - 2) / 10) )
NRAYS = 2 + 10·n²
```

Para `createRays(12)`: `n = 1`, `NRAYS = 12`. Con `n = 1` los bucles de aristas y caras no agregan ningún punto, así que los 12 rayos son exactamente los 12 vértices del icosaedro.

El arreglo `Rays` se llena en este orden:

```
[0..11]                    vértices del icosaedro
[12 .. 12+30(n-1)-1]       puntos interiores de las 30 aristas (n-1 por arista)
[resto]                    puntos interiores de las 20 caras
```

El total cuadra: `12 + 30(n-1) + 10(n-1)(n-2) = 2 + 10n²`.

### 3.2 Vértices del icosaedro

Todos son vectores unitarios. Con `S = 2/√5 ≈ 0.894` y `R = (5-√5)/5`:

| Índice | Coordenadas | Descripción |
|---|---|---|
| 0 | `(0, 0, 1)` | Polo norte |
| 1–5 | `(S·cos 72°i, S·sin 72°i, 1-R)` | Anillo superior, `z = 1/√5 ≈ 0.447` |
| 6–10 | `(S·cos(72°i+36°), S·sin(72°i+36°), R-1)` | Anillo inferior, `z ≈ -0.447`, girado 36° |
| 11 | `(0, 0, -1)` | Polo sur |

Comprobación de que son unitarios: `S² + (1-R)² = 4/5 + 1/5 = 1`.

### 3.3 Tablas `A` (aristas) y `T` (triángulos)

- `A[30][3]`: cada fila es `{vértice1, vértice2, posición en Rays}`. La tercera columna se rellena durante la ejecución con el índice donde empiezan los puntos interiores de esa arista.
- `T[20][3]`: cada fila es `{arista1, arista2, orientación}`. Orientación `0` = triángulo "en pie" (V), `-1` = invertido. Define el tamaño `m` de cada fila de subdivisión de la cara.

### 3.4 La fórmula de subdivisión (los `c[0..7]`)

Dados dos vectores unitarios `B` y `C` con ángulo `θ = acos(B·C)`, el código calcula los puntos del arco de círculo máximo entre ambos, repartidos en ángulos iguales:

```
P(i) = [ (B - C·cosθ)·cos(iθ/n) + (C - B·cosθ)·cos((n-i)θ/n) ] / sin²θ
```

Correspondencia con el código:

- `c[0] = sin²θ` (es `|B × C|²` expandido).
- `c[1] = θ`.
- `c[2], c[4], c[6]` = componentes x, y, z de `B - C·cosθ`.
- `c[3], c[5], c[7]` = componentes x, y, z de `C - B·cosθ`.

Cada `P(i)` cumple `P·B = cos(iθ/n)` y `P·C = cos((n-i)θ/n)`. Es decir, está a `iθ/n` de `B` y a `(n-i)θ/n` de `C`, y por eso queda sobre el arco y es unitario.

- **Aristas:** para cada arista, `B` y `C` son sus dos vértices y se generan `i = 1..n-1`.
- **Caras:** para cada triángulo y cada `j = 1..n-1`, `B` y `C` son el j-ésimo punto de la primera y de la segunda arista. Se subdivide ese arco en `m` partes (`m = j` si está en pie, `m = n-j` si está invertido) y se generan `m-1` puntos interiores.

### 3.5 Cambios que se hicieron al código del profesor

| Problema original | Corrección |
|---|---|
| `class source{` anidada dos veces y un `};` sobrante | Una sola clase |
| `point` y `vector` en minúscula | `Point` y `Vector` (tus clases) |
| `p = 0.0;` (tu `Point` no se construye desde `double`) | Eliminado; el constructor por defecto ya deja `x=y=z=0` |
| `PI` no definida (y `raylib.h` define su propia macro `PI`) | Constante local `PI_D` en `Source.cpp`, que no incluye raylib |
| `Rays = NULL` | `nullptr` |
| `new Vector[NRAYS]` sin liberar al destruir | Se agregó `~Source() { delete[] Rays; }` |
| Copiar un `Source` duplicaría el puntero y causaría `delete[]` doble | Copia prohibida (`= delete`) |
| `source` en minúscula | `Source` (coincide con `Source.h`) |
| Todo en una clase inline | Separado en `Source.h` / `Source.cpp` |

No se modificó ninguna fórmula, tabla ni orden de cálculo.

### 3.6 Memoria

`Rays` es un `Vector*` creado con `new Vector[NRAYS]`. Cada `Vector` ocupa 3 `double` (24 bytes) y el constructor por defecto los inicializa a 0. Si `createRays` se llama otra vez, primero hace `delete[]` del arreglo previo.

---

## 4. Clases geométricas

### 4.1 `Vector`

- `operator*(Vector)`: **producto punto** `a·b = ax·bx + ay·by + az·bz`.
- `operator/(Vector)`: **producto cruz** (regla de la mano derecha):
  ```
  r.x = y·v.z - z·v.y
  r.y = z·v.x - x·v.z
  r.z = x·v.y - y·v.x
  ```
- `operator*(double)`, `operator+`, `operator-`: operaciones componente a componente.
- `length()`: `√(x²+y²+z²)`.
- `normalized()`: divide entre la longitud; devuelve `(0,0,0)` si `|v| < 1e-12` para evitar dividir entre cero.
- `reflect(n)`: ver sección 7.

### 4.2 `Point`

Tres `double` y `operator-(Point)` que devuelve un `Vector` (`P - Q` = vector de `Q` a `P`). Un constructor de 3 parámetros permite crear vértices directamente.

### 4.3 `Plane`

Guarda 4 vértices `v[0..3]` y la `normal`. El constructor calcula:

```cpp
Vector e1 = b - a;
Vector e2 = d - a;
normal = e1 / e2;      // producto cruz con el operador /
```

La normal **no está normalizada**: para la sala de lado 4 mide 16 (`|e1|·|e2| = 4·4`). Esto no afecta a la intersección ni a la dirección. `unitNormal()` devuelve la versión de longitud 1 y `center()` el promedio de los 4 vértices.

`Plane` no corrige el orden de los vértices: quien lo construye (`Room`) debe pasarlos ya ordenados.

---

## 5. `Room`: normales hacia el interior

Con la regla de la mano derecha, si los vértices se recorren en sentido **antihorario visto desde el interior**, `(b-a) / (d-a)` apunta hacia el interior. Para el semi-lado `h = 2`:

| Cara | Plano | `e1` | `e2` | Normal resultante |
|---|---|---|---|---|
| Piso | y = -h | +Z | +X | (0, +16, 0) |
| Techo | y = +h | +X | +Z | (0, -16, 0) |
| Izquierda | x = -h | +Y | +Z | (+16, 0, 0) |
| Derecha | x = +h | +Z | +Y | (-16, 0, 0) |
| Fondo | z = -h | +X | +Y | (0, 0, +16) |
| Frente | z = +h | +Y | +X | (0, 0, -16) |

Ejemplo (cara derecha): `e1 = (0,0,4)`, `e2 = (0,4,0)` → `x = 0·0 - 4·4 = -16`, `y = 0`, `z = 0` → `(-16,0,0)`, hacia adentro.

`Room::firstHit(o, d, hit, &faceIdx)` recorre las 6 caras, llama a la intersección rayo–plano y se queda con la de menor `t > 0` (la sala es convexa y la fuente está dentro).

---

## 6. Intersección rayo–plano (`Intersect`)

El rayo es `P(t) = O + t·d` y el plano cumple `n · (X - v0) = 0`. Al sustituir y despejar:

```
t = n · (v0 - O) / (n · d)
```

- Si `|n·d| < 1e-9` el rayo es paralelo al plano: no hay intersección.
- Si `t < 1e-9` el plano está detrás (o sobre) el origen: se descarta.
- Si no, `P_int = O + t·d`.

No se necesita normalizar `n`, porque su magnitud se cancela entre numerador y denominador. Las funciones reciben `Point`, `Vector` y `Plane` por valor porque los operadores no están marcados `const`.

**Estado actual:** el código existe y fue verificado, pero el render ya no lo usa; los rayos se dibujan con longitud fija.

---

## 7. Reflexión (`Vector::reflect`)

Se descompone el vector incidente `I` en una parte paralela a la normal y otra tangente a la pared. Al rebotar, la tangente se conserva y la paralela invierte el signo:

```
R = I - 2 (I · N̂) N̂        con N̂ = N / |N|
```

Propiedades: `|R| = |I|`, y como `I` apunta hacia la pared y `N` hacia adentro, `I·N̂ < 0`, por lo tanto `R·N̂ > 0` (el rayo vuelve al interior).

**Estado actual:** implementado y probado, pero retirado del render a petición. Queda listo para el siguiente paso.

---

## 8. Render con Raylib (`main.cpp`)

### 8.1 Frontera entre mis clases y Raylib

Raylib usa `Vector3` con `float`; las clases propias usan `double`. Las funciones `toV3(Point)` y `toV3(Vector)` hacen la conversión `double → float` solo al momento de dibujar. Todo el cálculo geométrico queda en `double`.

### 8.2 Cámara orbital con mouse

La cámara apunta siempre al origen. Su posición se calcula en coordenadas esféricas:

```
x = dist · cos(pitch) · sin(yaw)
y = dist · sin(pitch)
z = dist · cos(pitch) · cos(yaw)
```

- Clic izquierdo + arrastrar: `yaw -= Δx·0.005`, `pitch += Δy·0.005` (con `pitch` limitado a ±1.5 rad para evitar el volteo en los polos).
- Rueda: `dist` entre 3 y 30.

### 8.3 Elementos dibujados

- **Cuadrícula:** `DrawGrid(10, 1.0f)` en `y = 0`.
- **Ejes:** cilindros delgados en X (rojo), Y (verde) y Z (azul) cruzando en `(0,0,0)`, más una esfera negra en el origen. Su longitud es 0.5 para no tapar las normales.
- **Sala:** wireframe; se dibujan las 4 aristas de cada una de las 6 caras con `DrawLine3D` usando los vértices de cada `Plane`. Cada arista del cubo se dibuja dos veces, lo que es irrelevante visualmente.
- **Normales:** 6 flechas `DARKGREEN` desde `center()` de cada cara, con `unitNormal()` de longitud 1, apuntando hacia adentro.
- **Fuente / receptor:** esferas roja en `(1.5, 0, 1)` y azul en `(-1.5, 0, -1)`.
- **Rayos:** 12 flechas naranjas desde la fuente en la dirección `Rays[i].normalized()` con longitud fija 4. Atraviesan las paredes porque aún no se usan colisiones.
- **UI:** `LoadFontEx("fuente.ttf", 24, 0, 0)` con filtro bilineal y `DrawTextEx`. Si no se encuentra la fuente, se usa la de Raylib. Flags `FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI` antes de `InitWindow`.

### 8.4 `DrawArrow3D`

Raylib no trae flechas. La función calcula:

```
unit = (end - start) / |end - start|
base = end - unit · headLen
```

Dibuja el cuerpo de `start` a `base` (con `DrawLine3D`, o con un cilindro si se pide grosor) y la punta como un cono: `DrawCylinderEx(base, end, headRadius, 0, 12, color)`, es decir, radio inferior `headRadius` y radio superior 0. La punta termina exactamente en `end`.

---

## 9. Verificación en consola

Antes del bucle de Raylib se imprimen las 6 normales con `std::cout`. Salida esperada:

```
Piso: (0, 16, 0)        Techo: (0, -16, 0)
Izquierda: (16, 0, 0)   Derecha: (-16, 0, 0)
Fondo: (0, 0, 16)       Frente: (0, 0, -16)
```

---

## 10. Estado y pendientes

**Funcionando:** icosaedro del profesor adaptado, sala con normales interiores, intersección rayo–plano, reflexión, visualización completa.

**Implementado pero desactivado en el render:** `Room::firstHit`, `intersectRayPlane`, `Vector::reflect`.

**Pendiente:** colisiones en el render, rebotes sucesivos, detección del receptor y energía por rayo (`Source::eF` existe pero aún no se usa).
