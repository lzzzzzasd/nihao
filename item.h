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
    //AttributeType��ʾ����

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
        std::cout << "��Ʒ: " << m_name << std::endl;
        std::cout << "����: " << (int)m_type << std::endl;
        std::cout << "����: " << m_description << std::endl;
        for (const auto& attr : m_attributes) {
            std::cout << "����(" << AttributeTypeToString(attr.first) << "): " << attr.second << std::endl;
        }
    }
};

#endif
