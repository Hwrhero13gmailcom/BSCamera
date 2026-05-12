#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>

struct CameraOffsetConfig {
    float FOV  = 90.0f;
    float PosX = 0.0f;
    float PosY = 0.0f;
    float PosZ = 0.0f;
    float RotX = 0.0f;
    float RotY = 0.0f;
    float RotZ = 0.0f;
};

inline CameraOffsetConfig& getConfig() {
    static CameraOffsetConfig config;
    return config;
}

inline std::string GetConfigPath() {
    return "/sdcard/ModData/com.beatgames.beatsaber/Mods/CameraOffsetMod/config.txt";
}

inline void LoadConfig() {
    std::ifstream file(GetConfigPath());
    if (!file.is_open()) return;
    auto& cfg = getConfig();
    std::string line;
    while (std::getline(file, line)) {
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        auto key = line.substr(0, eq);
        float val = std::stof(line.substr(eq + 1));
        if (key == "FOV")  cfg.FOV  = val;
        if (key == "PosX") cfg.PosX = val;
        if (key == "PosY") cfg.PosY = val;
        if (key == "PosZ") cfg.PosZ = val;
        if (key == "RotX") cfg.RotX = val;
        if (key == "RotY") cfg.RotY = val;
        if (key == "RotZ") cfg.RotZ = val;
    }
}

inline void SaveConfig() {
    auto path = GetConfigPath();
    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    std::ofstream file(path);
    auto& cfg = getConfig();
    file << "FOV="  << cfg.FOV  << "\n";
    file << "PosX=" << cfg.PosX << "\n";
    file << "PosY=" << cfg.PosY << "\n";
    file << "PosZ=" << cfg.PosZ << "\n";
    file << "RotX=" << cfg.RotX << "\n";
    file << "RotY=" << cfg.RotY << "\n";
    file << "RotZ=" << cfg.RotZ << "\n";
}
