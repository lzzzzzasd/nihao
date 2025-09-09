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
    EquipmentType m_equipmentType; // ��ȷװ������
    std::map<AttributeType, int> m_attributes;

public:
    Item(const std::string& name, const std::string& description,
        ItemType type, EquipmentType equipType = EquipmentType::MELEE_WEAPON)
        : m_name(name), m_description(description),
        m_type(type), m_equipmentType(equipType) {
    }

    std::string getName() const { return m_name; }
    std::string getDescription() const { return m_description; }
    ItemType getType() const { return m_type; }
    EquipmentType getEquipmentType() const { return m_equipmentType; } // ����

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
        std::cout << "����: " << m_name << std::endl;
        std::cout << "����: " << m_description << std::endl;
        if (m_type == ItemType::WEAPON || m_type == ItemType::ARMOR) {
            std::cout << "װ������: ";
            switch (m_equipmentType) {
            case EquipmentType::MELEE_WEAPON: std::cout << "��ս����"; break;
            case EquipmentType::RANGED_WEAPON: std::cout << "Զ������"; break;
            case EquipmentType::ARMOR: std::cout << "����"; break;
            }
            std::cout << std::endl;
        }
        for (const auto& attr : m_attributes) {
            std::cout << "����(" << AttributeTypeToString(attr.first) << "): "
                << (attr.second > 0 ? "+" : "") << attr.second << std::endl;
        }
    }

    std::map<AttributeType, int> getAllAttributes() const {
        return m_attributes;
    }
};

#endif