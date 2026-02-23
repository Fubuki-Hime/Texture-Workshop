#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class TWSPack {
    public:
        std::string TPName, DownloadURL, IconURL, TPDescription, TPCreator, TPVersion, GDVersion;
        int ID, downloads;
        bool featured;

        static TWSPack* create(std::string name, std::string downloadURL, std::string iconURL, std::string description, std::string creator, std::string version, std::string gdVersion, bool isFeatured, int downloads) {
            auto tp = new TWSPack();

            tp->TPName = name;
            tp->DownloadURL = downloadURL;
            tp->IconURL = iconURL;
            tp->TPDescription = description;
            tp->TPCreator = creator;
            tp->TPVersion = version;
            tp->GDVersion = gdVersion;
            tp->featured = isFeatured;
            tp->downloads = downloads;

            return tp;
        };

        void downloadTP();
        bool isDownloading();
};