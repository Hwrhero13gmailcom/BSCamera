#pragma once
#include <string>
#include <fstream>

struct CameraConfig {
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    float zOffset = 0.0f;
    float xRotation = 0.0f;
    float yRotation = 0.0f;
    float zRotation = 0.0f;
    float fov = 0.0f;
};

static const std::string CONFIG_PATH = "/sdcard/ModData/com.beatgames.beatsaber/Mods/CameraOffsetMod/config.txt";

inline void EnsureDir() {
    system("mkdir -p /sdcard/ModData/com.beatgames.beatsaber/Mods/CameraOffsetMod");
}

inline CameraConfig LoadConfig() {
    CameraConfig cfg;
    EnsureDir();
    std::ifstream f(CONFIG_PATH);
    if (!f.is_open()) return cfg;
    std::string line;
    while (std::getline(f, line)) {
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = line.substr(0, eq);
        float val = std::stof(line.substr(eq + 1));
        if (key == "xOffset")   cfg.xOffset   = val;
        if (key == "yOffset")   cfg.yOffset   = val;
        if (key == "zOffset")   cfg.zOffset   = val;
        if (key == "xRotation") cfg.xRotation = val;
        if (key == "yRotation") cfg.yRotation = val;
        if (key == "zRotation") cfg.zRotation = val;
        if (key == "fov")       cfg.fov       = val;
    }
    return cfg;
}

inline void SaveConfig(const CameraConfig& cfg) {
    EnsureDir();
    std::ofstream f(CONFIG_PATH, std::ios::trunc);
    f << "xOffset="   << cfg.xOffset   << "\n";
    f << "yOffset="   << cfg.yOffset   << "\n";
    f << "zOffset="   << cfg.zOffset   << "\n";
    f << "xRotation=" << cfg.xRotation << "\n";
    f << "yRotation=" << cfg.yRotation << "\n";
    f << "zRotation=" << cfg.zRotation << "\n";
    f << "fov="       << cfg.fov       << "\n";
}
