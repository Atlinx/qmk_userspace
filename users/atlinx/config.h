#pragma once

#ifdef AUDIO_ENABLE
#define STARTUP_SONG SONG(PLANCK_SOUND)
#endif

// Our keymaps use more than 16 layers, so widen the layer state to 32-bit.
// Must live in a config.h so it takes effect before quantum.h is processed.
#define LAYER_STATE_32BIT
