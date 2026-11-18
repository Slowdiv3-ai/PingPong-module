// effect/pingpong_effect.cpp
#include <effect_api.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Unique UUID for our effect (Generate your own later if needed)
const effect_uuid_t my_uuid = { 0x12345678, 0x1234, 0x1234, 0x1234, { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc } };

const effect_descriptor_t my_descriptor = {
    { 0x12345678, 0x1234, 0x1234, 0x1234, { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc } }, // uuid
    { 0x87654321, 0x4321, 0x4321, 0x4321, { 0x21, 0x43, 0x65, 0x87, 0xbc, 0x9a } }, // type (preprocessing/postprocessing)
    "PingPong DSP", // name
    "PingPong Inc", // implementor
    "PingPong Global Effect", // connect name
    EFFECT_FLAG_TYPE_POST_PROC | EFFECT_FLAG_DEVICE_IND | EFFECT_FLAG_HW_ACC_TUNNEL, // flags
    1, // cpuLoad
    1, // memoryUsage
    0, // reserved
};

typedef struct {
    effect_config_t config;
    float speed; // Our tweakable parameter
} pingpong_context_t;

static pingpong_context_t* context = NULL;

// --- Core Effect API Functions ---

int32_t EffectCreate(const effect_uuid_t *uuid, int32_t sessionId, int32_t ioId, effect_handle_t *pHandle) {
    context = (pingpong_context_t*)calloc(1, sizeof(pingpong_context_t));
    context->speed = 1.0f; // Default speed
    *pHandle = (effect_handle_t)context;
    return 0;
}

int32_t EffectRelease(effect_handle_t handle) {
    free(handle);
    return 0;
}

int32_t EffectProcess(effect_handle_t handle, audio_buffer_t *inBuffer, audio_buffer_t *outBuffer) {
    if (!context) return -1;
    
    // TODO: Add the actual PingPong DSP math here later!
    // For now, just copy input to output to prove it compiles and runs.
    memcpy(outBuffer->s16, inBuffer->s16, inBuffer->frameCount * 4); // 4 bytes per frame (stereo 16bit)
    
    return 0;
}

int32_t EffectCommand(effect_handle_t handle, uint32_t cmdCode, uint32_t cmdSize, void *pCmdData, uint32_t *replySize, void *pReplyData) {
    if (!context) return -1;

    switch (cmdCode) {
        case EFFECT_CMD_ENABLE:
        case EFFECT_CMD_DISABLE:
            return 0;
            
        // CUSTOM COMMAND: Set Speed
        // App sends: cmdCode = 0x1000, cmdData = float (speed)
        case 0x1000: 
            if (cmdSize == sizeof(float) && pCmdData != NULL) {
                context->speed = *((float*)pCmdData);
                // Log or apply speed change here
            }
            return 0;

        default:
            return -1;
    }
}

int32_t EffectGetDescriptor(const effect_uuid_t *uuid, effect_descriptor_t *pDescriptor) {
    *pDescriptor = my_descriptor;
    return 0;
}

// --- Library Entry Point ---
extern "C" {
    __attribute__ ((visibility ("default")))
    effect_library_t AUDIO_EFFECT_LIBRARY_INFO_SYM = {
        tag: EFFECT_LIBRARY_API_VERSION,
        name: "PingPong DSP Library",
        implementor: "PingPong Inc",
        get_descriptor: EffectGetDescriptor,
        create_effect: EffectCreate,
        release_effect: EffectRelease
    };
}
