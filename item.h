#pragma once
#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <map>
#include "global.h"

class Item {
private:
    std::string m_name;
    std::string m_description;
    ItemType m_type;
    std::map<AttributeType, int> m_attributes;

public:
    Item(const std::string& name, const std::string& description, ItemType type)
        : m_name(name), m_description(description), m_type(type) {
    }

    std::string getName() const { return m_name; }
    std::string getDescription() const { return m_description; }
    ItemType getType() const { return m_type; }

    void setAttribute(AttributeType attr, int value) {
        m_attributes[attr] = value;
    }

    int getAttribute(AttributeType attr) const {
        auto it = m_attributes.find(attr);
        if (it != m_attributes.end()) {
            return it->second;
        }
        return 0;
    }

    void displayInfo() const {
        std::cout << "物品: " << m_name << std::endl;
        std::cout << "类型: " << (int)m_type << std::endl;
        std::cout << "描述: " << m_description << std::endl;
        for (const auto& attr : m_attributes) {
            std::cout << "属性(" << (int)attr.first << "): " << attr.second << std::endl;
        }
    }
};

#endif