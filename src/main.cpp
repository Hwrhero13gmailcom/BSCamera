#include "config.hpp"

#include "scotland2/shared/modloader.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "custom-types/shared/register.hpp"
#include "paper2_scotland2/shared/logger.hpp"
#include "bsml/shared/BSML.hpp"
#include "HMUI/ViewController.hpp"

// bs-cordl headers
#include "GlobalNamespace/zzzz__MainCamera_def.hpp"
#include "GlobalNamespace/zzzz__MainCamera_impl.hpp"
#include "UnityEngine/zzzz__Camera_def.hpp"
#include "UnityEngine/zzzz__Camera_impl.hpp"
#include "UnityEngine/zzzz__Transform_def.hpp"
#include "UnityEngine/zzzz__Transform_impl.hpp"
#include "UnityEngine/zzzz__Vector3_def.hpp"

static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};
static CameraConfig config;

static auto logger = Paper::Logger::WithContext<MOD_ID>();

// ---- Settings UI ----

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (!firstActivation) return;
    // No BSML XML needed for a no-UI first build — settings via config file only
}

// ---- Camera hook ----

MAKE_HOOK_MATCH(MainCamera_Awake, &GlobalNamespace::MainCamera::Awake, void, GlobalNamespace::MainCamera* self) {
    MainCamera_Awake(self);

    auto* cam = self->GetComponent<UnityEngine::Camera*>();
    if (!cam) return;

    UnityEngine::Transform* transform = self->get_transform().ptr();
    if (!transform) return;

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
    logger.info("Camera Offset Mod setup called");
}

extern "C" __attribute__((visibility("default"))) void late_load() {
    il2cpp_functions::Init();
    config = LoadConfig();

    logger.info("Installing hooks...");
    INSTALL_HOOK(logger, MainCamera_Awake);

    BSML::Init();
    BSML::Register::RegisterSettingsMenu(MOD_ID, DidActivate, false);

    logger.info("Camera Offset Mod loaded");
}
