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
    Storage() {}
    ~Storage() {}

    bool storeItem(std::shared_ptr<Item> item) {
        if (item) {
            // 检查传入的智能指针是否指向一个真实的Item对象
            // 将物品添加到对应名字的vector中
            m_items[item->getName()].push_back(item);
            return true;
        }
        // 如果传入的是空指针，存储失败
        return false;
    }

    std::shared_ptr<Item> takeItem(const std::string& name) {
        // 1. 查找：在map中查找是否有这个键（物品名）
        auto it = m_items.find(name);
        // 2. 检查：如果找到了且对应的vector不为空
        if (it != m_items.end() && !it->second.empty()) {
            // 3. 取出：获取vector中的最后一个物品
            std::shared_ptr<Item> item = it->second.back();
            // 4. 删除：从vector中移除最后一个物品
            it->second.pop_back();
            // 5. 清理：如果这个vector空了，就把这个键值对从map中完全删除，节省空间
            if (it->second.empty()) {
                m_items.erase(it);
            }
            // 6. 返回：将这个物品的智能指针返回给调用者（例如，玩家要使用它）
            return item;
        }
        // 如果没找到或vector为空，返回一个空指针表示取出失败
        return nullptr;
    }

    void listAllItems() const {
        if (m_items.empty()) {
            std::cout << "背包是空的。" << std::endl;
            return;
        }
        // 遍历map中的每一个键值对
        for (const auto& entry : m_items) {
            // entry.first 是键（物品名），entry.second 是值（vector）
            // 打印：物品名 x数量（例如：血药 x3）
            std::cout << entry.first << " x" << entry.second.size() << std::endl;
            // 显示第一个物品的详细信息作为代表
            entry.second[0]->displayInfo();
            std::cout << "-------------------------" << std::endl;
        }
    }

    // 检查仓库中是否有某个物品（只关心有无，不取出）
    bool hasItem(const std::string& name) const {
        auto it = m_items.find(name);
        // 如果找到了键，并且对应的vector不为空，返回true，否则返回false
        return (it != m_items.end() && !it->second.empty());
    }

    const std::map<std::string, std::vector<std::shared_ptr<Item>>>& GetAllItems() const {
        return m_items;
    }
};



#endif