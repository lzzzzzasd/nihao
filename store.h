#pragma once
#ifndef STORE_H
#define STORE_H

#include <memory>
#include "player.h"
#include "storage.h"

class Store {
public:
    Store() = default;
    ~Store() = default;

    void ShowMenu(CPlayer& player, Storage& inventory) {
        bool inStore = true;
        while (inStore) {
            system("cls");
            std::cout << "===== 商店 =====" << std::endl;
            std::cout << "你的金币: " << player.GetGold() << std::endl;
            std::cout << "1. 药水 (50金币)" << std::endl;
            std::cout << "2. 枪 (100金币)" << std::endl;
            std::cout << "3. 护甲 (80金币)" << std::endl;
            std::cout << "4. 刀 (70金币)" << std::endl;
            std::cout << "5. 离开商店" << std::endl;
            std::cout << "请选择: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
            case 1: BuyPotion(player, inventory); break;
            case 2: BuyGun(player, inventory); break;
            case 3: BuyArmor(player, inventory); break;
            case 4: BuyKnife(player, inventory); break;
            case 5: inStore = false; std::cout << "感谢光临！" << std::endl; break;
            default: std::cout << "无效选项" << std::endl;
            }

            if (inStore) system("pause");
        }
    }

private:
    void BuyPotion(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 1) {
            player.AddGold(-1);
            auto potion = std::make_shared<Item>("强效药水", "恢复50生命值", ItemType::POTION);
            potion->setAttribute(AttributeType::HP, 50);
            inventory.storeItem(potion);
            std::cout << "获得强效药水！" << std::endl;
        }
        else {
            std::cout << "金币不足！" << std::endl;
        }
    }
    void BuyGun(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 1) {
            player.AddGold(-1);
            auto gun = std::make_shared<Item>("手枪", "攻击+10", ItemType::WEAPON);
            gun->setAttribute(AttributeType::ATTACK, 10);
            inventory.storeItem(gun);
            std::cout << "获得手枪！" << std::endl;
        }
        else {
            std::cout << "金币不足！" << std::endl;
        }
    }
    void BuyArmor(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 1) {
            player.AddGold(-1);
            auto armor = std::make_shared<Item>("护甲", "防御+5", ItemType::ARMOR);
            armor->setAttribute(AttributeType::DEFENSE, 5);
            inventory.storeItem(armor);
            std::cout << "获得护甲！" << std::endl;
        }
        else {
            std::cout << "金币不足！" << std::endl;
        }
    }
    void BuyKnife(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 1) {
            player.AddGold(-1);
            auto knife = std::make_shared<Item>("匕首", "攻击+3 敏捷+5", ItemType::WEAPON);
            knife->setAttribute(AttributeType::ATTACK, 3);
            knife->setAttribute(AttributeType::AGILITY, 5);
            inventory.storeItem(knife);
            std::cout << "获得匕首！" << std::endl;
        }
        else {
            std::cout << "金币不足！" << std::endl;
        }
    }
};

#endif