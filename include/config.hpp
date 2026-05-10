#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_JSON_STRUCT(CameraOffsetConfig) {
    VALUE_DEFAULT(float, FOV, 90.0f);
    VALUE_DEFAULT(float, PosX, 0.0f);
    VALUE_DEFAULT(float, PosY, 0.0f);
    VALUE_DEFAULT(float, PosZ, 0.0f);
    VALUE_DEFAULT(float, RotX, 0.0f);
    VALUE_DEFAULT(float, RotY, 0.0f);
    VALUE_DEFAULT(float, RotZ, 0.0f);
};

inline CameraOffsetConfig& getConfig() {
    static CameraOffsetConfig config;
    return config;
}
