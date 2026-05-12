#include "config.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML-Lite.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "HMUI/ViewController.hpp"

static modloader::ModInfo modInfo{"CameraOffsetMod", "0.1.0", 0};
static auto logger = Paper::Logger::WithContext<"CameraOffset">();

// ─── apply settings ───────────────────────────────────────────────────────────

static void ApplyCameraSettings() {
    auto cam = UnityEngine::Camera::get_main();
    if (!cam) return;

    auto& cfg = getConfig();

    cam->set_fieldOfView(cfg.FOV);

    auto t = cam->get_transform();
    t->set_localPosition({cfg.PosX, cfg.PosY, cfg.PosZ});
    t->set_localEulerAngles({cfg.RotX, cfg.RotY, cfg.RotZ});
}

// ─── settings menu ────────────────────────────────────────────────────────────

static void BuildSettingsUI(HMUI::ViewController* vc, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (!firstActivation) return;

    auto parent = vc->get_gameObject()->get_transform();
    auto& cfg = getConfig();

    BSML::Lite::CreateText(parent, "FOV & Camera Offset");

    BSML::Lite::CreateSliderSetting(parent, "FOV", 1.0f, cfg.FOV, 60.0f, 120.0f,
        [](float val) { getConfig().FOV = val; SaveConfig(); ApplyCameraSettings(); }
    );

    BSML::Lite::CreateText(parent, "Position");
    BSML::Lite::CreateSliderSetting(parent, "X", 0.01f, cfg.PosX, -1.0f, 1.0f,
        [](float val) { getConfig().PosX = val; SaveConfig(); ApplyCameraSettings(); }
    );
    BSML::Lite::CreateSliderSetting(parent, "Y", 0.01f, cfg.PosY, -1.0f, 1.0f,
        [](float val) { getConfig().PosY = val; SaveConfig(); ApplyCameraSettings(); }
    );
    BSML::Lite::CreateSliderSetting(parent, "Z", 0.01f, cfg.PosZ, -1.0f, 1.0f,
        [](float val) { getConfig().PosZ = val; SaveConfig(); ApplyCameraSettings(); }
    );

    BSML::Lite::CreateText(parent, "Rotation");
    BSML::Lite::CreateSliderSetting(parent, "Pitch (X)", 1.0f, cfg.RotX, -45.0f, 45.0f,
        [](float val) { getConfig().RotX = val; SaveConfig(); ApplyCameraSettings(); }
    );
    BSML::Lite::CreateSliderSetting(parent, "Yaw (Y)", 1.0f, cfg.RotY, -45.0f, 45.0f,
        [](float val) { getConfig().RotY = val; SaveConfig(); ApplyCameraSettings(); }
    );
    BSML::Lite::CreateSliderSetting(parent, "Roll (Z)", 1.0f, cfg.RotZ, -45.0f, 45.0f,
        [](float val) { getConfig().RotZ = val; SaveConfig(); ApplyCameraSettings(); }
    );

    BSML::Lite::CreateUIButton(parent, "Reset to Defaults", []() {
        auto& cfg = getConfig();
        cfg.FOV  = 90.0f;
        cfg.PosX = cfg.PosY = cfg.PosZ = 0.0f;
        cfg.RotX = cfg.RotY = cfg.RotZ = 0.0f;
        SaveConfig();
        ApplyCameraSettings();
    });
}

// ─── entry points ─────────────────────────────────────────────────────────────

extern "C" void setup(CModInfo* info) {
    *info = modInfo.to_c();
    logger.info("CameraOffsetMod setup");
}

extern "C" void late_load() {
    LoadConfig();
    ApplyCameraSettings();

    BSML::Register::RegisterSettingsMenu(
        "Camera Offset",
        BuildSettingsUI,
        false
    );

    logger.info("CameraOffsetMod loaded");
}
