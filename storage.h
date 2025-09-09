#pragma once
#ifndef STORAGE_H
#define STORAGE_H

#include <map>
#include <vector>
#include <memory>
#include <string>
#include "item.h"

class Storage {
private:
    std::map<std::string, std::vector<std::shared_ptr<Item>>> m_items;

public:
    Storage() = default;
    ~Storage() = default;

    bool storeItem(std::shared_ptr<Item> item) {
        if (item) {
            m_items[item->getName()].push_back(item);
            return true;
        }
        return false;
    }

    std::shared_ptr<Item> takeItem(const std::string& name) {
        auto it = m_items.find(name);
        if (it != m_items.end() && !it->second.empty()) {
            std::shared_ptr<Item> item = it->second.back();
            it->second.pop_back();
            if (it->second.empty()) {
                m_items.erase(it);
            }
            return item;
        }
        return nullptr;
    }

    void listAllItems() const {
        if (m_items.empty()) {
            std::cout << "背包是空的。" << std::endl;
            return;
        }
        for (const auto& entry : m_items) {
            std::cout << entry.first << " x" << entry.second.size() << std::endl;
            entry.second[0]->displayInfo();
            std::cout << "-------------------------" << std::endl;
        }
    }


    bool hasItem(const std::string& name) const {
        auto it = m_items.find(name);
        return (it != m_items.end() && !it->second.empty());
    }

    // 新增：获取所有物品，用于存档功能
    const std::map<std::string, std::vector<std::shared_ptr<Item>>>& getAllItems() const {
        return m_items;
    }
};

#endif
