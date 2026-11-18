// effect/effect_api.h
#ifndef ANDROID_EFFECT_API_H
#define ANDROID_EFFECT_API_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct effect_uuid_s {
    uint32_t timeLow;
    uint16_t timeMid;
    uint16_t timeHiAndVersion;
    uint16_t clockSeq;
    uint8_t node[6];
} effect_uuid_t;

typedef struct effect_descriptor_s {
    effect_uuid_t type;
    effect_uuid_t uuid;
    uint16_t apiVersion; // Must be 0x0300 for Android 13+
    uint32_t flags;
    uint16_t cpuLoad;
    uint16_t memoryUsage;
    const char* name;
    const char* implementor;
    const char* connectName;
} effect_descriptor_t;

typedef struct effect_config_s {
    uint32_t inputConfig;
    uint32_t outputConfig;
} effect_config_t;

typedef struct audio_buffer_s {
    uint32_t frameCount;
    union {
        int16_t* s16;
        int32_t* s32;
        float* f32;
        void* raw;
    };
} audio_buffer_t;

typedef void* effect_handle_t;

#define EFFECT_CMD_ENABLE 0
#define EFFECT_CMD_DISABLE 1

#define EFFECT_FLAG_TYPE_PRE_PROC 0x00000001
#define EFFECT_FLAG_TYPE_POST_PROC 0x00000002
#define EFFECT_FLAG_DEVICE_IND 0x00000004

// CRITICAL: Android 13+ requires API version 3.0
#define EFFECT_LIBRARY_API_VERSION 0x0300

typedef struct effect_library_s {
    uint32_t tag; // Must be 0x0300
    const char* name;
    const char* implementor;
    int32_t (*get_descriptor)(const effect_uuid_t *uuid, effect_descriptor_t *pDescriptor);
    int32_t (*create_effect)(const effect_uuid_t *uuid, int32_t sessionId, int32_t ioId, effect_handle_t *pHandle);
    int32_t (*release_effect)(effect_handle_t handle);
} effect_library_t;

#define AUDIO_EFFECT_LIBRARY_INFO_SYM effect_lib_info

#ifdef __cplusplus
}
#endif

#endif // ANDROID_EFFECT_API_H
