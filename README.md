# PPG Wave Clone — Fase 9: Pruebas

Implementación original de un instrumento virtual VST3 de síntesis wavetable
inspirado conceptualmente en el PPG Wave 3.3. No contiene ROMs, muestras,
gráficos ni código propietario de terceros.

## Estado de esta entrega

**Fases 1-8**: listo y confirmado sonando en hardware real (arquitectura,
osciladores wavetable, filtro + envolventes, LFOs + matriz de modulación,
efectos, gestión de presets, interfaz profesional, carácter vintage).

**Fase 9** (esta entrega): validación general. No agrega funcionalidad
nueva — ver `TESTING.md` para el checklist completo a correr en Ableton
Live (sample rates, tamaños de buffer, polifonía máxima, automatización,
cambio rápido de presets, estado del proyecto).

Antes de escribir el checklist, revisé el código buscando puntos frágiles
bajo las condiciones que pide esta fase y corregí uno real:

- **`Drive::prepare` usaba un tamaño de buffer fijo (512 muestras) en vez
  del tamaño real que reporta el host.** Como el filtro de tono de Drive
  se procesa muestra por muestra (no por bloque), esto probablemente no
  causaba un bug audible en la práctica — pero era incorrecto y podía
  volverse un problema real si el host usa buffers grandes (1024, 2048...)
  y una fase futura cambia ese filtro a procesamiento por bloque. Ya
  recibe el tamaño real de `PluginProcessor::prepareToPlay`, igual que
  Chorus, Delay y Reverb.

Durante las pruebas del usuario aparecieron tres problemas reales más,
también corregidos:

- **Ruido de fondo constante, independiente de lo que se tocara.** Cada
  una de las 8 voces procesaba audio en cada bloque incluso sin estar
  sonando. Con la mayoría de las formas de onda eso da silencio real, pero
  PWM tiene un valor distinto de cero justo al inicio de su ciclo — así
  que las voces en reposo filtraban un valor bajo pero constante todo el
  tiempo. `SynthVoice::renderNextBlock` ahora sale de inmediato si la voz
  no está activa (`isVoiceActive()`), lo cual además reduce el uso de CPU
  al no procesar voces silenciosas.
- **El visualizador ADSR solo reflejaba el nivel de Sustain.** El ancho de
  los segmentos Attack/Decay/Release era fijo a propósito, así que mover
  esos tres knobs no cambiaba el dibujo — quedó como una limitación mal
  pensada, no como el comportamiento esperado de un visualizador ADSR.
  Ahora el ancho de cada segmento es proporcional (en escala logarítmica)
  al tiempo real del parámetro.
- **El botón "Random" randomiza los parámetros del patch actual**, no
  carga un preset guardado al azar como antes (eso seguía la sección 17
  del documento original al pie de la letra, pero no es lo que se
  necesitaba en la práctica). Deja Master Volume intacto para no pegar un
  salto de volumen. `PresetManager::loadRandom()` (random *preset*) sigue
  existiendo en el código por si se necesita después, solo que ya no está
  conectado a ningún botón.

Ver `TESTING.md` para un límite de memoria conocido (no un bug) en el
Delay sincronizado a divisiones largas con tempos muy lentos.

## Compilación sin instalar nada (recomendado)

Este proyecto incluye `.github/workflows/build.yml`: compila el VST3 en un
Mac con Xcode ya instalado, en la nube, gratis, vía GitHub Actions. Sube la
carpeta a un repositorio (con GitHub Desktop, para no perder `.github` por
ser una carpeta oculta), espera la palomita verde en la pestaña "Actions", y
descarga el `.vst3` desde "Artifacts".

## Compilación local (requiere CMake + compilador C++20)

1. Clona JUCE dentro de esta carpeta:
   ```
   git clone --depth 1 --branch 7.0.12 https://github.com/juce-framework/JUCE.git
   ```
   Debe quedar como `PPGWaveClone/JUCE/`.

2. Configura y compila:

   **macOS** (Xcode instalado):
   ```
   cmake -B build -G Xcode
   cmake --build build --config Release
   ```
   El `.vst3` queda en `build/PPGWaveClone_artefacts/Release/VST3/`.
   Cópialo a `~/Library/Audio/Plug-Ins/VST3/` (tu usuario) o
   `/Library/Audio/Plug-Ins/VST3/` (todo el sistema — recomendado si Ableton
   corre en otra sesión de usuario). Si lo bajaste de un navegador:
   ```
   xattr -dr com.apple.quarantine "/ruta/al/PPG Wave Clone.vst3"
   ```

   **Windows** (Visual Studio 2022):
   ```
   cmake -B build -G "Visual Studio 17 2022"
   cmake --build build --config Release
   ```
   El `.vst3` queda en `build/PPGWaveClone_artefacts/Release/VST3/`.
   Cópialo a `C:\Program Files\Common Files\VST3\`.

3. Rescanea plugins en Ableton Live (Preferences → Plug-ins → Rescan).

## Estructura

Sin cambios de fondo esta fase (solo la corrección en
`Source/DSP/Effects/Drive.h/.cpp` y `EffectsChain.cpp`). Ver el README de
la Fase 8 para el árbol completo del proyecto.

## Qué sigue

Con la Fase 9 corrida y confirmada, el instrumento cumple los 19 criterios
de éxito de la sección 28 del documento de diseño original: compila,
genera VST3, Ableton lo detecta, recibe MIDI, genera audio, osciladores
wavetable funcionando (con modulación de posición), filtro, envolventes,
LFOs, matriz de modulación, presets (guardar/cargar), estado persistente
en el proyecto, parámetros automatizables, interfaz estable, biblioteca
inicial de presets, documentación, y código modular y mantenible.

Corre el checklist de `TESTING.md` y dime qué encuentras.
