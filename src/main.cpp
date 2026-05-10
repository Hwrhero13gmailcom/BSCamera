#include "config.hpp"

#include "scotland2/shared/modloader.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "paper2_scotland2/shared/logger.hpp"
#include "bsml/shared/BSML.hpp"
#include "HMUI/ViewController.hpp"

#include "GlobalNamespace/zzzz__MainCamera_def.hpp"
#include "GlobalNamespace/zzzz__MainCamera_impl.hpp"
#include "UnityEngine/zzzz__Camera_def.hpp"
#include "UnityEngine/zzzz__Camera_impl.hpp"
#include "UnityEngine/zzzz__Transform_def.hpp"
#include "UnityEngine/zzzz__Transform_impl.hpp"
#include "UnityEngine/zzzz__GameObject_def.hpp"
#include "UnityEngine/zzzz__GameObject_impl.hpp"
#include "UnityEngine/WaitForSeconds.hpp"

static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};
static CameraConfig config;
static auto logger = Paper::Logger::WithContext<MOD_ID>();

// Coroutine: wait for MainCamera then apply offset
custom_types::Helpers::Coroutine ApplyCameraOffset() {
    // Wait a couple frames for camera to be ready
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(
        UnityEngine::WaitForSeconds::New_ctor(1.0f)
    );

    UnityEngine::Camera* cam = UnityEngine::Camera::get_main().ptr();
    if (!cam) {
        logger.warn("Main camera not found");
        co_return;
    }

    UnityEngine::Transform* transform = cam->get_transform().ptr();
    if (!transform) co_return;

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

    logger.info("Camera offset applied");
    co_return;
}

// Settings UI (no-op for now, offsets set via config file)
void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
}

extern "C" __attribute__((visibility("default"))) void setup(CModInfo* info) {
    info->id           = MOD_ID;
    info->version      = VERSION;
    info->version_long = 0;
    logger.info("Camera Offset Mod setup");
}

extern "C" __attribute__((visibility("default"))) void late_load() {
    il2cpp_functions::Init();
    config = LoadConfig();

    logger.info("Starting camera offset coroutine");
    // Start coroutine via a temporary GameObject
    auto* go = UnityEngine::GameObject::New_ctor();
    UnityEngine::GameObject::DontDestroyOnLoad(go);
    auto* mb = go->AddComponent<UnityEngine::MonoBehaviour*>();
    mb->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(ApplyCameraOffset()));

    BSML::Init();
    BSML::Register::RegisterSettingsMenu(MOD_ID, DidActivate, false);

    logger.info("Camera Offset Mod loaded");
}
