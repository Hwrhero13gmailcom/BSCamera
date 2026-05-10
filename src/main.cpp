#include "main.hpp"
#include "config.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "custom-types/shared/register.hpp"

#include "GlobalNamespace/MainCamera.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "HMUI/ViewController.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/RectTransform.hpp"

#include <string>

static ModInfo modInfo;
static CameraConfig config;

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// ---- Settings UI ----

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (!firstActivation) return;

    auto container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());

    // X Offset
    QuestUI::BeatSaberUI::CreateSliderSetting(
        container->get_transform(),
        "X Offset", 0.01f, config.xOffset, -2.0f, 2.0f,
        [](float val) {
            config.xOffset = val;
            SaveConfig(config);
        }
    );

    // Y Offset
    QuestUI::BeatSaberUI::CreateSliderSetting(
        container->get_transform(),
        "Y Offset", 0.01f, config.yOffset, -2.0f, 2.0f,
        [](float val) {
            config.yOffset = val;
            SaveConfig(config);
        }
    );

    // Z Offset
    QuestUI::BeatSaberUI::CreateSliderSetting(
        container->get_transform(),
        "Z Offset", 0.01f, config.zOffset, -2.0f, 2.0f,
        [](float val) {
            config.zOffset = val;
            SaveConfig(config);
        }
    );

    // X Rotation
    QuestUI::BeatSaberUI::CreateSliderSetting(
        container->get_transform(),
        "X Rotation", 1.0f, config.xRotation, -180.0f, 180.0f,
        [](float val) {
            config.xRotation = val;
            SaveConfig(config);
        }
    );

    // Y Rotation
    QuestUI::BeatSaberUI::CreateSliderSetting(
        container->get_transform(),
        "Y Rotation", 1.0f, config.yRotation, -180.0f, 180.0f,
        [](float val) {
            config.yRotation = val;
            SaveConfig(config);
        }
    );

    // Z Rotation
    QuestUI::BeatSaberUI::CreateSliderSetting(
        container->get_transform(),
        "Z Rotation", 1.0f, config.zRotation, -180.0f, 180.0f,
        [](float val) {
            config.zRotation = val;
            SaveConfig(config);
        }
    );
}

// ---- Camera hook ----

MAKE_HOOK_MATCH(MainCamera_Awake, &GlobalNamespace::MainCamera::Awake, void, GlobalNamespace::MainCamera* self) {
    MainCamera_Awake(self);

    auto* cam = self->GetComponent<UnityEngine::Camera*>();
    if (!cam) return;

    auto* transform = cam->get_transform();

    UnityEngine::Vector3 pos = transform->get_localPosition();
    pos.x += config.xOffset;
    pos.y += config.yOffset;
    pos.z += config.zOffset;
    transform->set_localPosition(pos);

    UnityEngine::Vector3 rot = transform->get_localEulerAngles();
    rot.x += config.xRotation;
    rot.y += config.yRotation;
    rot.z += config.zRotation;
    transform->set_localEulerAngles(rot);
}

// ---- Entry point ----

extern "C" void setup(ModInfo& info) {
    info.id      = "CameraOffsetMod";
    info.version = "0.1.0";
    modInfo      = info;
    getLogger().info("Camera Offset Mod setup called");
}

extern "C" void load() {
    il2cpp_functions::Init();
    config = LoadConfig();

    getLogger().info("Installing Camera Offset Mod hooks...");
    INSTALL_HOOK(getLogger(), MainCamera_Awake);

    QuestUI::Init();
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);

    getLogger().info("Camera Offset Mod loaded successfully");
}
