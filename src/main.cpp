#include "config.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML-Lite.hpp"
#include "custom-types/shared/register.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "HMUI/ViewController.hpp"

static modloader::ModInfo modInfo{"CameraOffsetMod", "0.1.0", 0};
static auto logger = Paper::Logger::WithContext<"CameraOffset">();

// ─── apply settings ───────────────────────────────────────────────────────────

static void ApplyCameraSettings() {
    auto cam = UnityEngine::Camera::get_main();
    if (!cam) return;

    cam->set_fieldOfView(getConfig().FOV.GetValue());

    auto t = cam->get_transform();
    auto pos = t->get_localPosition();
    t->set_localPosition({
        pos.x + getConfig().PosX.GetValue(),
        pos.y + getConfig().PosY.GetValue(),
        pos.z + getConfig().PosZ.GetValue()
    });

    auto rot = t->get_localEulerAngles();
    t->set_localEulerAngles({
        rot.x + getConfig().RotX.GetValue(),
        rot.y + getConfig().RotY.GetValue(),
        rot.z + getConfig().RotZ.GetValue()
    });
}

// ─── settings menu ────────────────────────────────────────────────────────────

static void BuildSettingsUI(HMUI::ViewController* vc, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (!firstActivation) return;

    auto parent = vc->get_gameObject();

    BSML::Lite::CreateText(parent->get_transform(), "Camera Offset Settings");

    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "FOV", 1.0f, getConfig().FOV.GetValue(), 60.0f, 120.0f,
        [](float val) { getConfig().FOV.SetValue(val); }
    );

    BSML::Lite::CreateText(parent->get_transform(), "Position Offset");
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "X", 0.01f, getConfig().PosX.GetValue(), -0.5f, 0.5f,
        [](float val) { getConfig().PosX.SetValue(val); }
    );
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "Y", 0.01f, getConfig().PosY.GetValue(), -0.5f, 0.5f,
        [](float val) { getConfig().PosY.SetValue(val); }
    );
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "Z", 0.01f, getConfig().PosZ.GetValue(), -0.5f, 0.5f,
        [](float val) { getConfig().PosZ.SetValue(val); }
    );

    BSML::Lite::CreateText(parent->get_transform(), "Rotation Offset");
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "Pitch (X)", 1.0f, getConfig().RotX.GetValue(), -45.0f, 45.0f,
        [](float val) { getConfig().RotX.SetValue(val); }
    );
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "Yaw (Y)", 1.0f, getConfig().RotY.GetValue(), -45.0f, 45.0f,
        [](float val) { getConfig().RotY.SetValue(val); }
    );
    BSML::Lite::CreateSliderSetting(
        parent->get_transform(), "Roll (Z)", 1.0f, getConfig().RotZ.GetValue(), -45.0f, 45.0f,
        [](float val) { getConfig().RotZ.SetValue(val); }
    );

    BSML::Lite::CreateUIButton(parent->get_transform(), "Reset to Defaults", []() {
        getConfig().FOV.SetValue(90.0f);
        getConfig().PosX.SetValue(0.0f);
        getConfig().PosY.SetValue(0.0f);
        getConfig().PosZ.SetValue(0.0f);
        getConfig().RotX.SetValue(0.0f);
        getConfig().RotY.SetValue(0.0f);
        getConfig().RotZ.SetValue(0.0f);
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

    BSML::Register::RegisterSettingsMenu(
        "Camera Offset",
        BuildSettingsUI,
        false
    );

    logger.info("CameraOffsetMod loaded");
}
