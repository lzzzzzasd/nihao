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
            std::cout << "===== …ÃµÍ =====" << std::endl;
            std::cout << "ƒ„µƒΩ±“: " << player.GetGold() << std::endl;
            std::cout << "1. «ø–ß“©ÀÆ (5Ω±“) - ª÷∏¥50…˙√¸÷µ" << std::endl;
            std::cout << "2. ∞Ÿ¡∂∏÷Ω£ (40Ω±“) - π•ª˜+15" << std::endl;
            std::cout << "3. ∫ÆÃ˙÷ÿΩ£ (35Ω±“) - π•ª˜+12 √ÙΩ›-3" << std::endl;
            std::cout << "4.  ÷ÂÛ (30Ω±“) - π•ª˜+8 √ÙΩ›+5" << std::endl;
            std::cout << "5. ∑˚ŒƒÂÛ (45Ω±“) - π•ª˜+12 √ÙΩ›+8" << std::endl;
            std::cout << "6. À¯◊”º◊ (25Ω±“) - ∑¿”˘+8" << std::endl;
            std::cout << "7. æ´÷¬∞Âº◊ (35Ω±“) - ∑¿”˘+12" << std::endl;
            std::cout << "8. ¿Îø™…ÃµÍ" << std::endl;
            std::cout << "«Î—°‘Ò: ";

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
            case 8: inStore = false; std::cout << "¿Îø™…ÃµÍ°£" << std::endl; break;
            default: std::cout << "Œﬁ–ß—°‘Ò" << std::endl;
            }

            if (inStore) system("pause");
        }
    }

private:
    void BuyPotion(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 5) {
            player.AddGold(-5);
            auto potion = std::make_shared<Item>("«ø–ß“©ÀÆ", "ª÷∏¥50µ„…˙√¸÷µ", ItemType::POTION);
            potion->setAttribute(AttributeType::HP, 50);
            inventory.storeItem(potion);
            std::cout << "π∫¬Ú¡À«ø–ß“©ÀÆ£°" << std::endl;
        }
        else {
            std::cout << "Ω±“≤ª◊„£°" << std::endl;
        }
    }

    void BuyRefinedSteelSword(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 40) {
            player.AddGold(-40);
            auto meleeWeapon = std::make_shared<Item>("∞Ÿ¡∂∏÷Ω£", "π•ª˜+15", ItemType::WEAPON);
            meleeWeapon->setAttribute(AttributeType::ATTACK, 15);
            inventory.storeItem(meleeWeapon);
            std::cout << "π∫¬Ú¡À∞Ÿ¡∂∏÷Ω££°" << std::endl;
        }
        else {
            std::cout << "Ω±“≤ª◊„£°" << std::endl;
        }
    }

    void BuyColdIronGreatsword(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 35) {
            player.AddGold(-35);
            auto meleeWeapon = std::make_shared<Item>("∫ÆÃ˙÷ÿΩ£", "π•ª˜+12 √ÙΩ›-3", ItemType::WEAPON);
            meleeWeapon->setAttribute(AttributeType::ATTACK, 12);
            meleeWeapon->setAttribute(AttributeType::AGILITY, -3); // ∏∫÷µ±Ì æºı…Ÿ√ÙΩ›
            inventory.storeItem(meleeWeapon);
            std::cout << "π∫¬Ú¡À∫ÆÃ˙÷ÿΩ££°" << std::endl;
        }
        else {
            std::cout << "Ω±“≤ª◊„£°" << std::endl;
        }
    }

    void BuyHandCrossbow(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 30) {
            player.AddGold(-30);
            auto rangedWeapon = std::make_shared<Item>(" ÷ÂÛ", "π•ª˜+8 √ÙΩ›+5", ItemType::WEAPON);
            rangedWeapon->setAttribute(AttributeType::ATTACK, 8);
            rangedWeapon->setAttribute(AttributeType::AGILITY, 5);
            inventory.storeItem(rangedWeapon);
            std::cout << "π∫¬Ú¡À ÷ÂÛ£°" << std::endl;
        }
        else {
            std::cout << "Ω±“≤ª◊„£°" << std::endl;
        }
    }

    void BuyRuneCrossbow(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 45) {
            player.AddGold(-45);
            auto rangedWeapon = std::make_shared<Item>("∑˚ŒƒÂÛ", "π•ª˜+12 √ÙΩ›+8", ItemType::WEAPON);
            rangedWeapon->setAttribute(AttributeType::ATTACK, 12);
            rangedWeapon->setAttribute(AttributeType::AGILITY, 8);
            inventory.storeItem(rangedWeapon);
            std::cout << "π∫¬Ú¡À∑˚ŒƒÂÛ£°" << std::endl;
        }
        else {
            std::cout << "Ω±“≤ª◊„£°" << std::endl;
        }
    }

    void BuyChainmail(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 25) {
            player.AddGold(-25);
            auto armor = std::make_shared<Item>("À¯◊”º◊", "∑¿”˘+8", ItemType::ARMOR);
            armor->setAttribute(AttributeType::DEFENSE, 8);
            inventory.storeItem(armor);
            std::cout << "π∫¬Ú¡ÀÀ¯◊”º◊£°" << std::endl;
        }
        else {
            std::cout << "Ω±“≤ª◊„£°" << std::endl;
        }
    }

    void BuyFinePlateArmor(CPlayer& player, Storage& inventory) {
        if (player.GetGold() >= 35) {
            player.AddGold(-35);
            auto armor = std::make_shared<Item>("æ´÷¬∞Âº◊", "∑¿”˘+12", ItemType::ARMOR);
            armor->setAttribute(AttributeType::DEFENSE, 12);
            inventory.storeItem(armor);
            std::cout << "π∫¬Ú¡Àæ´÷¬∞Âº◊£°" << std::endl;
        }
        else {
            std::cout << "Ω±“≤ª◊„£°" << std::endl;
        }
    }
};

#endif