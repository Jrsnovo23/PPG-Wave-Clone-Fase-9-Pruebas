# PPG Wave Clone — Fase 9: Checklist de Pruebas

Esta fase no agrega funcionalidad nueva. Es la validación final antes de
considerar el instrumento "terminado" según los criterios de éxito del
documento de diseño original (sección 28). No puedo correr un DAW real
desde donde trabajo, así que esta fase la ejecutas tú en Ableton Live y me
cuentas qué encuentras — para cada ítem que falle, dime cuál es y lo
corregimos antes de dar la fase por cerrada.

## 1. Sample rates

Cambia el sample rate del proyecto en Ableton Live (Preferencias → Audio)
y confirma que el plugin sigue sonando afinado y sin artefactos raros en
cada uno:

- [ ] 44.1 kHz
- [ ] 48 kHz
- [ ] 88.2 kHz
- [ ] 96 kHz

**Caso límite conocido**: si tienes el Delay sincronizado ("Sync" activado
vía automatización, ya que no tiene knob en la GUI) en la división "4/1" y
el tempo del proyecto es muy lento (por debajo de ~90 BPM aprox., varía
según el sample rate), el tiempo de delay real sonará más corto que el
musicalmente esperado — el buffer interno tiene un máximo de ~4.3s a 44.1
kHz (~2s a 96 kHz) y se recorta a ese límite en vez de fallar. No es un
crash, es una limitación de memoria conocida. Repórtame si te topas con
esto en la práctica; si te importa lo ampliamos.

## 2. Tamaños de buffer

Cambia el tamaño de buffer del audio driver (Ableton Live → Preferencias →
Audio → Buffer Size) y confirma que no hay clicks, dropouts ni cambios de
comportamiento:

- [ ] Buffer pequeño (64 o 128 samples) — mayor exigencia de CPU
- [ ] Buffer mediano (256 o 512 samples) — el uso típico
- [ ] Buffer grande (1024 o 2048 samples) — menor exigencia de CPU, más
      latencia

## 3. Polifonía máxima

- [ ] Toca un acorde de 8 notas simultáneas (todas las voces disponibles)
      y confirma que suenan todas sin distorsión inesperada ni cuelgues.
- [ ] Con las 8 voces sonando, toca una 9na nota y confirma que el plugin
      le "roba" la voz a la nota más vieja en vez de trabarse o no sonar
      (voice stealing automático de JUCE).
- [ ] Suelta todas las notas a la vez (o con el pedal de sustain) y
      confirma que el release de todas las voces se escucha limpio, sin
      clicks al llegar a silencio.

## 4. Automatización

- [ ] Dibuja una curva de automatización en Ableton sobre algún parámetro
      que cambie rápido (por ejemplo Filter Cutoff o el Wave Position de
      un oscilador) y confirma que se escucha suave, sin "escalones"
      audibles ni clicks.
- [ ] Automatiza Master Volume de silencio a volumen normal y viceversa;
      confirma que no hay clicks en los extremos.
- [ ] Cambia un parámetro tipo interruptor (por ejemplo Drive Enabled)
      por automatización mientras suena una nota; confirma que no hay un
      "pop" audible al activarse/desactivarse.

## 5. Cambio rápido de presets

- [ ] Con una nota sonando, cambia de preset (botones `<` `>` o el
      selector) varias veces seguidas y rápido. Es normal que el timbre
      cambie de golpe (es lo que hace un preset) — lo que no debería pasar
      es un click fuerte, silencio inesperado, o que el plugin deje de
      responder.
- [ ] Usa "Random" varias veces seguidas, rápido.
- [ ] Guarda un preset de usuario, ciérralo y ábrelo (recarga el proyecto
      de Ableton Live o reabre el plugin) y confirma que tu preset sigue
      ahí y suena igual que cuando lo guardaste.

## 6. Estado del proyecto de Ableton Live

- [ ] Ajusta varios parámetros a mano, guarda el proyecto de Ableton Live,
      ciérralo y vuelve a abrirlo — confirma que el plugin recuerda
      exactamente cómo lo dejaste (esto usa `getStateInformation` /
      `setStateInformation`, independiente del sistema de presets).
- [ ] Duplica la pista con el plugin cargado — confirma que la copia tiene
      el mismo estado que el original, y que ambas instancias son
      independientes entre sí (cambiar una no afecta a la otra).

## 7. General

- [ ] Deja el plugin sonando (o con notas entrando y saliendo) por varios
      minutos seguidos — confirma que no hay fugas de memoria evidentes
      (el uso de RAM de Ableton no debería crecer sin parar) ni que el uso
      de CPU se dispare con el tiempo.
- [ ] Prueba con Vintage/Modern en distintos valores (0, medio, máximo)
      combinado con varios presets, para confirmar que no introduce
      clicks ni comportamiento inestable en ningún punto del rango.

---

Repórtame los resultados — no hace falta que sea una lista formal, con que
me digas "todo bien excepto X" alcanza. Si algo falla, lo corregimos ahí
mismo antes de seguir.
