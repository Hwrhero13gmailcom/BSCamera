#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(CameraOffsetConfig,
    CONFIG_VALUE(FOV, float, "FOV", 90.0f);
    CONFIG_VALUE(PosX, float, "PosX", 0.0f);
    CONFIG_VALUE(PosY, float, "PosY", 0.0f);
    CONFIG_VALUE(PosZ, float, "PosZ", 0.0f);
    CONFIG_VALUE(RotX, float, "RotX", 0.0f);
    CONFIG_VALUE(RotY, float, "RotY", 0.0f);
    CONFIG_VALUE(RotZ, float, "RotZ", 0.0f);
)
