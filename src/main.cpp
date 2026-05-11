#include "config.hpp"

#include "scotland2/shared/modloader.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/register.hpp"
#include "paper2_scotland2/shared/logger.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML-Lite.hpp"
#include "HMUI/ViewController.hpp"

#include "GlobalNamespace/zzzz__MainCamera_def.hpp"
#include "GlobalNamespace/zzzz__MainCamera_impl.hpp"
#include "UnityEngine/zzzz__Camera_def.hpp"
#include "UnityEngine/zzzz__Camera_impl.hpp"
#include "UnityEngine/zzzz__Transform_def.hpp"
#include "UnityEngine/zzzz__Transform_impl.hpp"
#include "UnityEngine/zzzz__GameObject_def.hpp"
#include "UnityEngine/zzzz__GameObject_impl.hpp"
#include "UnityEngine/zzzz__MonoBehaviour_def.hpp"
#include "UnityEngine/zzzz__MonoBehaviour_impl.hpp"
#include "UnityEngine/WaitForSeconds.hpp"

static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};
static CameraConfig config;
static auto logger = Paper::Logger::WithContext<MOD_ID>();

static void ApplyOffset() {
    UnityEngine::Camera* cam = UnityEngine::Camera::get_main().ptr();
    if (!cam) return;
    UnityEngine::Transform* t = cam->get_transform().ptr();
    if (!t) return;
    auto pos = t->get_localPosition();
    pos.x += config.xOffset; pos.y += config.yOffset; pos.z += config.zOffset;
    t->set_localPosition(pos);
    auto rot = t->get_localEulerAngles();
    rot.x += config.xRotation; rot.y += config.yRotation; rot.z += config.zRotation;
    t->set_localEulerAngles(rot);
    logger.info("Camera offset applied");
}

custom_types::Helpers::Coroutine ApplyCameraOffsetCoro() {
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(
        UnityEngine::WaitForSeconds::New_ctor(1.5f)
    );
    ApplyOffset();
    co_return;
}

// ---- Settings UI ----

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (!firstActivation) return;
    auto* container = BSML::Lite::CreateScrollableSettingsContainer(self->get_transform());
    UnityEngine::Transform* t = container->get_transform().ptr();

    BSML::Lite::CreateSliderSetting(t, "X Offset",    0.01f, config.xOffset,   -2.0f, 2.0f,   0.0f, true, {}, [](float v){ config.xOffset   = v; SaveConfig(config); });
    BSML::Lite::CreateSliderSetting(t, "Y Offset",    0.01f, config.yOffset,   -2.0f, 2.0f,   0.0f, true, {}, [](float v){ config.yOffset   = v; SaveConfig(config); });
    BSML::Lite::CreateSliderSetting(t, "Z Offset",    0.01f, config.zOffset,   -2.0f, 2.0f,   0.0f, true, {}, [](float v){ config.zOffset   = v; SaveConfig(config); });
    BSML::Lite::CreateSliderSetting(t, "X Rotation",  1.0f,  config.xRotation, -180.0f, 180.0f, 0.0f, true, {}, [](float v){ config.xRotation = v; SaveConfig(config); });
    BSML::Lite::CreateSliderSetting(t, "Y Rotation",  1.0f,  config.yRotation, -180.0f, 180.0f, 0.0f, true, {}, [](float v){ config.yRotation = v; SaveConfig(config); });
    BSML::Lite::CreateSliderSetting(t, "Z Rotation",  1.0f,  config.zRotation, -180.0f, 180.0f, 0.0f, true, {}, [](float v){ config.zRotation = v; SaveConfig(config); });

    BSML::Lite::CreateUIButton(t, "Apply Now", [self](){
        ApplyOffset();
    });
}

// ---- Entry point ----

extern "C" __attribute__((visibility("default"))) void setup(CModInfo* info) {
    info->id = MOD_ID; info->version = VERSION; info->version_long = 0;
    logger.info("Camera Offset Mod setup");
}

extern "C" __attribute__((visibility("default"))) void late_load() {
    il2cpp_functions::Init();
    config = LoadConfig();
    logger.info("Camera Offset Mod late_load");

    auto* go = UnityEngine::GameObject::New_ctor();
    UnityEngine::GameObject::DontDestroyOnLoad(go);
    auto* mb = go->AddComponent<UnityEngine::MonoBehaviour*>();
    mb->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(ApplyCameraOffsetCoro()));

    BSML::Init();
    BSML::Register::RegisterSettingsMenu("Camera Offset", DidActivate, false);

    logger.info("Camera Offset Mod loaded");
}
