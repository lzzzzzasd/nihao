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
            std::cout << "===== �̵� =====" << std::endl;
            std::cout << "��Ľ��: " << player.GetGold() << std::endl;
            std::cout << "1. ҩˮ (50���)" << std::endl;
            std::cout << "2. ǹ (100���)" << std::endl;
            std::cout << "3. ���� (80���)" << std::endl;
            std::cout << "4. �� (70���)" << std::endl;
            std::cout << "5. �뿪�̵�" << std::endl;
            std::cout << "��ѡ��: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
            case 1: BuyPotion(player, inventory); break;
            case 2: BuyGun(player, inventory); break;
            case 3: BuyArmor(player, inventory); break;
            case 4: BuyKnife(player, inventory); break;
            case 5: inStore = false; std::cout << "��л���٣�" << std::endl; break;
            default: std::cout << "��Чѡ��" << std::endl;
            }

            if (inStore) system("pause");
        }
    }

private:
    void BuyPotion(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 1) {
            player.AddGold(-1);
            auto potion = std::make_shared<Item>("ǿЧҩˮ", "�ָ�50����ֵ", ItemType::POTION);
            potion->setAttribute(AttributeType::HP, 50);
            inventory.storeItem(potion);
            std::cout << "���ǿЧҩˮ��" << std::endl;
        }
        else {
            std::cout << "��Ҳ��㣡" << std::endl;
        }
    }
    void BuyGun(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 1) {
            player.AddGold(-1);
            auto gun = std::make_shared<Item>("��ǹ", "����+10", ItemType::WEAPON);
            gun->setAttribute(AttributeType::ATTACK, 10);
            inventory.storeItem(gun);
            std::cout << "�����ǹ��" << std::endl;
        }
        else {
            std::cout << "��Ҳ��㣡" << std::endl;
        }
    }
    void BuyArmor(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 1) {
            player.AddGold(-1);
            auto armor = std::make_shared<Item>("����", "����+5", ItemType::ARMOR);
            armor->setAttribute(AttributeType::DEFENSE, 5);
            inventory.storeItem(armor);
            std::cout << "��û��ף�" << std::endl;
        }
        else {
            std::cout << "��Ҳ��㣡" << std::endl;
        }
    }
    void BuyKnife(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 1) {
            player.AddGold(-1);
            auto knife = std::make_shared<Item>("ذ��", "����+3 ����+5", ItemType::WEAPON);
            knife->setAttribute(AttributeType::ATTACK, 3);
            knife->setAttribute(AttributeType::AGILITY, 5);
            inventory.storeItem(knife);
            std::cout << "���ذ�ף�" << std::endl;
        }
        else {
            std::cout << "��Ҳ��㣡" << std::endl;
        }
    }
};

#endif