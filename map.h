#pragma once
#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <memory>

class Map {
private:
    std::string m_name;
    std::vector<std::string> m_connectedMaps;
    bool m_hasBoss;

public:
    Map(const std::string& name, bool hasBoss = false)
        : m_name(name), m_hasBoss(hasBoss) {
    }

    std::string GetName() const { return m_name; }
    bool HasBoss() const { return m_hasBoss; }

    void AddConnectedMap(const std::string& mapName) {
        m_connectedMaps.push_back(mapName);
    }

    const std::vector<std::string>& GetConnectedMaps() const {
        return m_connectedMaps;
    }

    void ShowConnectedMaps() const {
        std::cout << "¿ÉÇ°ÍùµØÍ¼:\n";
        for (size_t i = 0; i < m_connectedMaps.size(); ++i) {
            std::cout << (i + 1) << ". " << m_connectedMaps[i] << std::endl;
        }
    }
};

#endif