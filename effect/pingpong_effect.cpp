// effect/pingpong_effect.cpp
#include "effect_api.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Unique UUID for our effect: 12345678-1234-1234-1234-123456789abc
const effect_uuid_t my_uuid = { 
    0x12345678, 0x1234, 0x1234, 0x1234, { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc } 
};

const effect_descriptor_t my_descriptor = {
    { 0x12345678, 0x1234, 0x1234, 0x1234, { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc } }, // type
    { 0x12345678, 0x1234, 0x1234, 0x1234, { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc } }, // uuid
    1, // apiVersion
    EFFECT_FLAG_TYPE_POST_PROC | EFFECT_FLAG_DEVICE_IND, // flags
    1, // cpuLoad
    1, // memoryUsage
    "PingPong DSP", // name
    "PingPong Inc", // implementor
    "PingPong Global Effect" // connect name
};

typedef struct {
    effect_config_t config;
    float speed; // Our tweakable parameter
    bool enabled;
} pingpong_context_t;

static pingpong_context_t* context = NULL;

// --- Core Effect API Functions ---

int32_t EffectCreate(const effect_uuid_t *uuid, int32_t sessionId, int32_t ioId, effect_handle_t *pHandle) {
    context = (pingpong_context_t*)calloc(1, sizeof(pingpong_context_t));
    context->speed = 1.0f; // Default speed
    context->enabled = false;
    *pHandle = (effect_handle_t)context;
    return 0;
}

int32_t EffectRelease(effect_handle_t handle) {
    if (handle) {
        free(handle);
    }
    return 0;
}

int32_t EffectProcess(effect_handle_t handle, audio_buffer_t *inBuffer, audio_buffer_t *outBuffer) {
    if (!context || !context->enabled) {
        // If disabled, just pass audio through unchanged
        if (inBuffer && outBuffer && inBuffer->s16 && outBuffer->s16) {
            memcpy(outBuffer->s16, inBuffer->s16, inBuffer->frameCount * 4); // 4 bytes per frame (stereo 16-bit)
        }
        return 0;
    }
    
    // TODO: Add the actual PingPong DSP math here later!
    // For now, copy input to output to prove it compiles and runs.
    if (inBuffer && outBuffer && inBuffer->s16 && outBuffer->s16) {
        memcpy(outBuffer->s16, inBuffer->s16, inBuffer->frameCount * 4);
    }
    
    return 0;
}

int32_t EffectCommand(effect_handle_t handle, uint32_t cmdCode, uint32_t cmdSize, void *pCmdData, uint32_t *replySize, void *pReplyData) {
    if (!context) return -1;

    switch (cmdCode) {
        case EFFECT_CMD_ENABLE:
            context->enabled = true;
            return 0;
        case EFFECT_CMD_DISABLE:
            context->enabled = false;
            return 0;
            
        // CUSTOM COMMAND: Set Speed
        // App sends: cmdCode = 0x1000, cmdData = float (speed)
        case 0x1000: 
            if (cmdSize == sizeof(float) && pCmdData != NULL) {
                context->speed = *((float*)pCmdData);
            }
            return 0;

        default:
            return -1; // Command not handled
    }
}

int32_t EffectGetDescriptor(const effect_uuid_t *uuid, effect_descriptor_t *pDescriptor) {
    if (pDescriptor) {
        *pDescriptor = my_descriptor;
    }
    return 0;
}

// --- Library Entry Point ---
extern "C" {
    __attribute__ ((visibility ("default")))
    effect_library_t AUDIO_EFFECT_LIBRARY_INFO_SYM = {
        EFFECT_LIBRARY_API_VERSION,
        "PingPong DSP Library",
        "PingPong Inc",
        EffectGetDescriptor,
        EffectCreate,
        EffectRelease
    };
}
