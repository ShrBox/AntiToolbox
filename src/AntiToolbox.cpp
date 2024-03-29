﻿#include <MC/ServerPlayer.hpp>
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/ConnectionRequest.hpp>
#include <vector>
#include <rapidjson/document.h>
#include "base64.h"
#include <MC/Level.hpp>
#include <PlayerInfoAPI.h>
#include <MC/NetworkIdentifier.hpp>
#include <filesystem>
#include "Version.h"
#include <ServerAPI.h>

std::string Kick_message = "§cDon't use toolbox!";
std::vector<std::string> Array;
bool FakeNameDetection = true;
std::vector<std::string> CmdArray;
bool EnableCustomCmd = false;
Logger logger(PLUGIN_NAME);

inline void CheckProtocolVersion() {
#ifdef TARGET_BDS_PROTOCOL_VERSION
    auto currentProtocol = ll::getServerProtocolVersion();
    if (TARGET_BDS_PROTOCOL_VERSION != currentProtocol)
    {
        logger.warn("Protocol version not match, target version: {}, current version: {}.",
            TARGET_BDS_PROTOCOL_VERSION, currentProtocol);
        logger.warn("This will most likely crash the server, please use the Plugin that matches the BDS version!");
    }
#endif // TARGET_BDS_PROTOCOL_VERSION
}

void loadConfig() {
    std::string config_file = "plugins/AntiToolbox/config.json";
    std::ifstream fs;
    fs.open(config_file, std::ios::in);
    if (!fs) {
        logger.warn("{} not found, creating configuration file", config_file);
        std::filesystem::create_directory("plugins/AntiToolbox");
        std::ofstream of(config_file);
        if (of) {
            of
                    << "{\n  \"KickMessage\": \"§cDon't use toolbox!\",\n  \"WhiteList\": [\"Notch\", \"Jeb_\"],\n  \"FakeNameDetection\": true,\n  \"EnableCustomCmd\": false,\n  \"CustomCmd\": [\"ban ban %player%\", \"say Toolbox Detected: %player%\"]\n}";
        } else {
            logger.error("Configuration file creation failed");
        }
    } else {
        std::string json;
        char buf[1024];
        while (fs.getline(buf, 1024)) {
            json.append(buf);
        }
        rapidjson::Document document;
        document.Parse(json.c_str());
        if (document.HasMember("KickMessage")) {
            Kick_message = document["KickMessage"].GetString();
        } else {
            logger.warn("Config KickMessage not found");
        }
        if (document.HasMember("WhiteList")) {
            auto arraylist = document["WhiteList"].GetArray();
            for (rapidjson::Value::ConstValueIterator itr = arraylist.Begin(); itr != arraylist.End(); ++itr) {
                Array.emplace_back(itr->GetString());
            }
        } else {
            logger.warn("Config WhiteList not found");
        }
        if (document.HasMember("FakeNameDetection")) {
            FakeNameDetection = document["FakeNameDetection"].GetBool();
        } else {
            logger.warn("Config FakeNameDetection not found");
        }
        if (document.HasMember("EnableCustomCmd")) {
            EnableCustomCmd = document["EnableCustomCmd"].GetBool();
        } else {
            logger.warn("Config EnableCustomCmd not found");
        }
        if (document.HasMember("CustomCmd")) {
            auto arraylist = document["CustomCmd"].GetArray();
            for (rapidjson::Value::ConstValueIterator itr = arraylist.Begin(); itr != arraylist.End(); ++itr) {
                CmdArray.emplace_back(itr->GetString());
            }
        } else {
            logger.warn("Config CmdArray not found");
        }
    }
}

void customCmdExe(std::string player_name) {
    if (!CmdArray.empty()) {
        auto spacePos = player_name.find(' ');
        if (spacePos != std::string::npos) {
            player_name = "\"" + player_name + "\"";
        }
    }
    for (std::string cmd: CmdArray) {
        auto position = cmd.find("%player%");
        if (position != std::string::npos) {
            cmd.replace(position, 9, player_name);
        }
        //std::cout << cmd << "\n";
        Level::runcmdEx(cmd);
    }
}

bool onPlayerLogin(Event::PlayerJoinEvent ev) {
    if (FakeNameDetection) {
        std::string real_name = ev.mPlayer->getRealName();
        std::string player_name = ev.mPlayer->getNameTag();
        if (real_name != player_name) {
            logger.info("Fake Nametag detected: {} RealName: {}", player_name, real_name);
            if (!EnableCustomCmd) {
                ev.mPlayer->kick(Kick_message);
            } else {
                customCmdExe(real_name);
            }
        }
    }
    return true;
}

void PluginInit() {
    CheckProtocolVersion();
    logger.setFile("logs/AntiToolbox.log", true);
    loadConfig();
    Event::PlayerJoinEvent::subscribe(onPlayerLogin);
    logger.info("Loaded");
}

std::vector<string> split(const string &str, const char pattern) {
    std::vector<string> res;
    std::stringstream input(str);
    string temp;
    while (getline(input, temp, pattern)) {
        res.push_back(temp);
    }
    return res;
}

THook(void,
      "?sendLoginMessageLocal@ServerNetworkHandler@@QEAAXAEBVNetworkIdentifier@@AEBVConnectionRequest@@AEAVServerPlayer@@@Z",
      ServerNetworkHandler *thi, NetworkIdentifier *networkId, ConnectionRequest *con_req, ServerPlayer *sp) {
    BuildPlatform device_os = con_req->getDeviceOS();
    if (device_os == BuildPlatform::Android) {
        std::string pkt = base64_decode(con_req->mRawToken->mData);
        for (const std::string& pl_name: Array) { // WhiteList detecting
            std::string xuid = PlayerInfo::getXuid(pl_name);
            if (xuid == sp->getXuid()) { // WhiteList detected
                return original(thi, networkId, con_req, sp);
            }
        }
        rapidjson::Document document;
        document.Parse(pkt.c_str());
        std::string device_model = document["DeviceModel"].GetString();
        std::string player_name = sp->getRealName();

        if (device_model.empty()) {
            logger.info("Unknown model detected: {}, using Horion client?", player_name);
            if (!EnableCustomCmd) {
                sp->kick("Unknown model");
            } else {
                customCmdExe(player_name);
            }
            return original(thi, networkId, con_req, sp);
        }
        const char *spl = " ";
        std::vector device_model_ = split(device_model, *spl);
        std::string device_company = device_model_[0];
        std::string device_company_ori = device_company;
        std::transform(device_company.begin(), device_company.end(), device_company.begin(), ::toupper);
        //std::cout << device_company << ":" << device_company_ori << "\n";
        if (device_company_ori != device_company) { //Toolbox detected
            logger.info("Toolbox detected: {}", player_name);
            if (!EnableCustomCmd) {
                sp->kick(Kick_message);
            } else {
                customCmdExe(player_name);
            }
            return original(thi, networkId, con_req, sp);
        }
    }
    return original(thi, networkId, con_req, sp);
}