#include "config.hpp"

#include "scotland2/shared/modloader.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "custom-types/shared/register.hpp"

// bs-cordl generated headers for BS 1.40
#include "GlobalNamespace/zzzz__MainCamera_def.hpp"
#include "UnityEngine/zzzz__Camera_def.hpp"
#include "UnityEngine/zzzz__Transform_def.hpp"
#include "UnityEngine/zzzz__Vector3_def.hpp"

// BSML for settings UI
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Components/Settings/SliderSetting.hpp"

#include "paper2_scotland2/shared/logger.hpp"

static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};
static CameraConfig config;

Paper::ConstLoggerContext<18ULL> static const& getLogger() {
    static auto ctx = Paper::Logger::WithContext<MOD_ID>();
    return ctx;
}

// ---- Settings UI ----

#include "HMUI/ViewController.hpp"

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (!firstActivation) return;

    BSML::parse_and_inject(self->get_transform(), R"(
        <bg xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'
            xsi:noNamespaceSchemaLocation='https://raw.githubusercontent.com/nicoco007/BeatSaberMarkupLanguage/main/Schema/bsml-schema.xsd'>
            <vertical child-control-height='false'>
                <slider-setting text='X Offset' value='xOffset' min='-2' max='2' increment='0.01' apply-on-change='true'/>
                <slider-setting text='Y Offset' value='yOffset' min='-2' max='2' increment='0.01' apply-on-change='true'/>
                <slider-setting text='Z Offset' value='zOffset' min='-2' max='2' increment='0.01' apply-on-change='true'/>
                <slider-setting text='X Rotation' value='xRotation' min='-180' max='180' increment='1' apply-on-change='true'/>
                <slider-setting text='Y Rotation' value='yRotation' min='-180' max='180' increment='1' apply-on-change='true'/>
                <slider-setting text='Z Rotation' value='zRotation' min='-180' max='180' increment='1' apply-on-change='true'/>
            </vertical>
        </bg>
    )", nullptr);
}

// ---- Camera hook ----

MAKE_HOOK_MATCH(MainCamera_Awake, &GlobalNamespace::MainCamera::Awake, void, GlobalNamespace::MainCamera* self) {
    MainCamera_Awake(self);

    auto* cam = self->GetComponent<UnityEngine::Camera*>();
    if (!cam) return;

    auto* transform = cam->get_transform();

    auto pos = transform->get_localPosition();
    pos.x += config.xOffset;
    pos.y += config.yOffset;
    pos.z += config.zOffset;
    transform->set_localPosition(pos);

    auto rot = transform->get_localEulerAngles();
    rot.x += config.xRotation;
    rot.y += config.yRotation;
    rot.z += config.zRotation;
    transform->set_localEulerAngles(rot);
}

// ---- Entry point ----

extern "C" __attribute__((visibility("default"))) void setup(CModInfo* info) {
    info->id      = MOD_ID;
    info->version = VERSION;
    info->version_long = 0;
    getLogger().info("Camera Offset Mod setup called");
}

extern "C" __attribute__((visibility("default"))) void late_load() {
    il2cpp_functions::Init();
    config = LoadConfig();

    getLogger().info("Installing Camera Offset Mod hooks...");
    INSTALL_HOOK(getLogger(), MainCamera_Awake);

    BSML::Init();
    BSML::Register::RegisterSettingsMenu(modInfo, DidActivate);

    getLogger().info("Camera Offset Mod loaded successfully");
}
