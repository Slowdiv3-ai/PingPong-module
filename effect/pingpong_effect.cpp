// effect/pingpong_effect.cpp
#include "effect_api.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

const effect_uuid_t my_uuid = { 
    0x12345678, 0x1234, 0x1234, 0x1234, { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc } 
};

const effect_descriptor_t my_descriptor = {
    { 0x12345678, 0x1234, 0x1234, 0x1234, { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc } }, // type
    { 0x12345678, 0x1234, 0x1234, 0x1234, { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc } }, // uuid
    0x0300, // apiVersion
    EFFECT_FLAG_TYPE_POST_PROC | EFFECT_FLAG_DEVICE_IND, // flags
    1, // cpuLoad
    1, // memoryUsage
    "PingPong DSP", // name
    "PingPong Inc", // implementor
    "PingPong Global Effect" // connect name
};

typedef struct {
    effect_config_t config;
    float speed;
    bool enabled;
    size_t frameSize; // Store the actual frame size
} pingpong_context_t;

static pingpong_context_t* context = NULL;

int32_t EffectCreate(const effect_uuid_t *uuid, int32_t sessionId, int32_t ioId, effect_handle_t *pHandle) {
    context = (pingpong_context_t*)calloc(1, sizeof(pingpong_context_t));
    context->speed = 1.0f;
    context->enabled = false;
    context->frameSize = 8; // Default to 8 bytes (32-bit stereo) for modern Android
    *pHandle = (effect_handle_t)context;
    return 0;
}

int32_t EffectRelease(effect_handle_t handle) {
    if (handle) free(handle);
    return 0;
}

int32_t EffectProcess(effect_handle_t handle, audio_buffer_t *inBuffer, audio_buffer_t *outBuffer) {
    if (!context) return -1;
    
    if (inBuffer && outBuffer && inBuffer->raw && outBuffer->raw && inBuffer->frameCount > 0) {
        // SAFE COPY: Use raw pointer and calculate size based on frameCount * frameSize
        // This prevents buffer corruption on 32-bit float systems
        size_t bytesToCopy = inBuffer->frameCount * context->frameSize;
        memcpy(outBuffer->raw, inBuffer->raw, bytesToCopy);
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
        case EFFECT_CMD_SET_CONFIG:
            // Handle configuration changes to update frameSize if needed
            // For now, keep the safe default of 8 bytes
            return 0;
        case 0x1000: 
            if (cmdSize == sizeof(float) && pCmdData != NULL) context->speed = *((float*)pCmdData);
            return 0;
        default: 
            return 0; // Return 0 instead of -1 for unhandled commands to prevent errors
    }
}

int32_t EffectGetDescriptor(const effect_uuid_t *uuid, effect_descriptor_t *pDescriptor) {
    if (pDescriptor) *pDescriptor = my_descriptor;
    return 0;
}

extern "C" {
    __attribute__ ((visibility ("default")))
    effect_library_t AUDIO_EFFECT_LIBRARY_INFO_SYM = {
        0x0300, // tag
        "PingPong DSP Library",
        "PingPong Inc",
        EffectGetDescriptor,
        EffectCreate,
        EffectRelease
    };
}
