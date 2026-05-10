#include "config.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML-Lite.hpp"
#include "custom-types/shared/register.hpp"
#include "GlobalNamespace/MainCamera.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"

static modloader::ModInfo modInfo{"CameraOffsetMod", "0.1.0", 0};
static Paper::ConstLoggerContext<15UL> const logger = Paper::Logger::WithContext<"CameraOffset">();

// ─── apply settings to camera ─────────────────────────────────────────────────

static void ApplyCameraSettings(UnityEngine::Camera* cam) {
    if (!cam) return;

    auto& cfg = getConfig();

    // FOV
    cam->set_fieldOfView(cfg.FOV);

    // Position offset (relative to parent transform)
    auto t = cam->get_transform();
    auto localPos = t->get_localPosition();
    t->set_localPosition({
        localPos.x + cfg.PosX,
        localPos.y + cfg.PosY,
        localPos.z + cfg.PosZ
    });

    // Rotation offset
    auto localRot = t->get_localEulerAngles();
    t->set_localEulerAngles({
        localRot.x + cfg.RotX,
        localRot.y + cfg.RotY,
        localRot.z + cfg.RotZ
    });
}

// ─── hooks ────────────────────────────────────────────────────────────────────

MAKE_AUTO_HOOK_MATCH(
    MainCamera_Awake,
    &GlobalNamespace::MainCamera::Awake,
    void,
    GlobalNamespace::MainCamera* self
) {
    MainCamera_Awake(self);
    ApplyCameraSettings(self->get_gameObject()->GetComponent<UnityEngine::Camera*>());
}

// ─── settings menu ────────────────────────────────────────────────────────────

static void BuildSettingsUI(UnityEngine::GameObject* parent) {
    auto& cfg = getConfig();

    BSML::Lite::CreateText(parent->get_transform(), "Camera Offset Settings");

    // FOV
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "FOV", 1.0f, cfg.FOV, 60.0f, 120.0f,
        [](float val) {
            getConfig().FOV = val;
            getConfig().Save();
        }
    );

    // Position
    BSML::Lite::CreateText(parent->get_transform(), "Position Offset");
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "X", 0.01f, cfg.PosX, -0.5f, 0.5f,
        [](float val) { getConfig().PosX = val; getConfig().Save(); }
    );
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "Y", 0.01f, cfg.PosY, -0.5f, 0.5f,
        [](float val) { getConfig().PosY = val; getConfig().Save(); }
    );
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "Z", 0.01f, cfg.PosZ, -0.5f, 0.5f,
        [](float val) { getConfig().PosZ = val; getConfig().Save(); }
    );

    // Rotation
    BSML::Lite::CreateText(parent->get_transform(), "Rotation Offset");
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "Pitch (X)", 1.0f, cfg.RotX, -45.0f, 45.0f,
        [](float val) { getConfig().RotX = val; getConfig().Save(); }
    );
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "Yaw (Y)", 1.0f, cfg.RotY, -45.0f, 45.0f,
        [](float val) { getConfig().RotY = val; getConfig().Save(); }
    );
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "Roll (Z)", 1.0f, cfg.RotZ, -45.0f, 45.0f,
        [](float val) { getConfig().RotZ = val; getConfig().Save(); }
    );

    // Reset button
    BSML::Lite::CreateUIButton(parent->get_transform(), "Reset to Defaults", []() {
        auto& cfg = getConfig();
        cfg.FOV  = 90.0f;
        cfg.PosX = cfg.PosY = cfg.PosZ = 0.0f;
        cfg.RotX = cfg.RotY = cfg.RotZ = 0.0f;
        cfg.Save();
    });
}

// ─── entry points ─────────────────────────────────────────────────────────────

extern "C" void setup(CModInfo* info) {
    *info = modInfo.to_c();
    getConfig().Init(modInfo);
    logger.info("CameraOffsetMod setup");
}

extern "C" void late_load() {
    il2cpp_functions::Init();

    INSTALL_HOOK(logger, MainCamera_Awake);

    BSML::Register::RegisterSettingsMenu("Camera Offset", BuildSettingsUI, false);

    logger.info("CameraOffsetMod loaded");
}
