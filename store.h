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
            std::cout << "1. ǿЧҩˮ (5���) - �ָ�50����ֵ" << std::endl;
            std::cout << "2. �����ֽ� (40���) - ����+15" << std::endl;
            std::cout << "3. �����ؽ� (35���) - ����+12 ����-3" << std::endl;
            std::cout << "4. ���� (30���) - ����+8 ����+5" << std::endl;
            std::cout << "5. ������ (45���) - ����+12 ����+8" << std::endl;
            std::cout << "6. ���Ӽ� (25���) - ����+8" << std::endl;
            std::cout << "7. ���°�� (35���) - ����+12" << std::endl;
            std::cout << "8. �뿪�̵�" << std::endl;
            std::cout << "��ѡ��: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
            case 1: BuyPotion(player, inventory); break;
            case 2: BuyRefinedSteelSword(player, inventory); break;
            case 3: BuyColdIronGreatsword(player, inventory); break;
            case 4: BuyHandCrossbow(player, inventory); break;
            case 5: BuyRuneCrossbow(player, inventory); break;
            case 6: BuyChainmail(player, inventory); break;
            case 7: BuyFinePlateArmor(player, inventory); break;
            case 8: inStore = false; std::cout << "�뿪�̵ꡣ" << std::endl; break;
            default: std::cout << "��Чѡ��" << std::endl;
            }

            if (inStore) system("pause");
        }
    }

private:
    void BuyPotion(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 5) {
            player.AddGold(-5);
            auto potion = std::make_shared<Item>("ǿЧҩˮ", "�ָ�50������ֵ", ItemType::POTION);
            potion->setAttribute(AttributeType::HP, 50);
            inventory.storeItem(potion);
            std::cout << "������ǿЧҩˮ��" << std::endl;
        }
        else {
            std::cout << "��Ҳ��㣡" << std::endl;
        }
    }

    void BuyRefinedSteelSword(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 40) {
            player.AddGold(-40);
            auto meleeWeapon = std::make_shared<Item>("�����ֽ�", "����+15", ItemType::WEAPON);
            meleeWeapon->setAttribute(AttributeType::ATTACK, 15);
            inventory.storeItem(meleeWeapon);
            std::cout << "�����˰����ֽ���" << std::endl;
        }
        else {
            std::cout << "��Ҳ��㣡" << std::endl;
        }
    }

    void BuyColdIronGreatsword(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 35) {
            player.AddGold(-35);
            auto meleeWeapon = std::make_shared<Item>("�����ؽ�", "����+12 ����-3", ItemType::WEAPON);
            meleeWeapon->setAttribute(AttributeType::ATTACK, 12);
            meleeWeapon->setAttribute(AttributeType::AGILITY, -3); // ��ֵ��ʾ��������
            inventory.storeItem(meleeWeapon);
            std::cout << "�����˺����ؽ���" << std::endl;
        }
        else {
            std::cout << "��Ҳ��㣡" << std::endl;
        }
    }

    void BuyHandCrossbow(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 30) {
            player.AddGold(-30);
            auto rangedWeapon = std::make_shared<Item>("����", "����+8 ����+5", ItemType::WEAPON);
            rangedWeapon->setAttribute(AttributeType::ATTACK, 8);
            rangedWeapon->setAttribute(AttributeType::AGILITY, 5);
            inventory.storeItem(rangedWeapon);
            std::cout << "����������" << std::endl;
        }
        else {
            std::cout << "��Ҳ��㣡" << std::endl;
        }
    }

    void BuyRuneCrossbow(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 45) {
            player.AddGold(-45);
            auto rangedWeapon = std::make_shared<Item>("������", "����+12 ����+8", ItemType::WEAPON);
            rangedWeapon->setAttribute(AttributeType::ATTACK, 12);
            rangedWeapon->setAttribute(AttributeType::AGILITY, 8);
            inventory.storeItem(rangedWeapon);
            std::cout << "�����˷�����" << std::endl;
        }
        else {
            std::cout << "��Ҳ��㣡" << std::endl;
        }
    }

    void BuyChainmail(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 25) {
            player.AddGold(-25);
            auto armor = std::make_shared<Item>("���Ӽ�", "����+8", ItemType::ARMOR);
            armor->setAttribute(AttributeType::DEFENSE, 8);
            inventory.storeItem(armor);
            std::cout << "���������Ӽף�" << std::endl;
        }
        else {
            std::cout << "��Ҳ��㣡" << std::endl;
        }
    }

    void BuyFinePlateArmor(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 35) {
            player.AddGold(-35);
            auto armor = std::make_shared<Item>("���°��", "����+12", ItemType::ARMOR);
            armor->setAttribute(AttributeType::DEFENSE, 12);
            inventory.storeItem(armor);
            std::cout << "�����˾��°�ף�" << std::endl;
        }
        else {
            std::cout << "��Ҳ��㣡" << std::endl;
        }
    }
};

#endif