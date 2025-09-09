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
            // ��鴫�������ָ���Ƿ�ָ��һ����ʵ��Item����
            // ����Ʒ��ӵ���Ӧ���ֵ�vector��
            m_items[item->getName()].push_back(item);
            return true;
        }
        // ���������ǿ�ָ�룬�洢ʧ��
        return false;
    }

    std::shared_ptr<Item> takeItem(const std::string& name) {
        // 1. ���ң���map�в����Ƿ������������Ʒ����
        auto it = m_items.find(name);
        // 2. ��飺����ҵ����Ҷ�Ӧ��vector��Ϊ��
        if (it != m_items.end() && !it->second.empty()) {
            // 3. ȡ������ȡvector�е����һ����Ʒ
            std::shared_ptr<Item> item = it->second.back();
            // 4. ɾ������vector���Ƴ����һ����Ʒ
            it->second.pop_back();
            // 5. ����������vector���ˣ��Ͱ������ֵ�Դ�map����ȫɾ������ʡ�ռ�
            if (it->second.empty()) {
                m_items.erase(it);
            }
            // 6. ���أ��������Ʒ������ָ�뷵�ظ������ߣ����磬���Ҫʹ������
            return item;
        }
        // ���û�ҵ���vectorΪ�գ�����һ����ָ���ʾȡ��ʧ��
        return nullptr;
    }

    void listAllItems() const {
        if (m_items.empty()) {
            std::cout << "�����ǿյġ�" << std::endl;
            return;
        }
        // ����map�е�ÿһ����ֵ��
        for (const auto& entry : m_items) {
            // entry.first �Ǽ�����Ʒ������entry.second ��ֵ��vector��
            // ��ӡ����Ʒ�� x���������磺Ѫҩ x3��
            std::cout << entry.first << " x" << entry.second.size() << std::endl;
            // ��ʾ��һ����Ʒ����ϸ��Ϣ��Ϊ����
            entry.second[0]->displayInfo();
            std::cout << "-------------------------" << std::endl;
        }
    }

    // ���ֿ����Ƿ���ĳ����Ʒ��ֻ�������ޣ���ȡ����
    bool hasItem(const std::string& name) const {
        auto it = m_items.find(name);
        // ����ҵ��˼������Ҷ�Ӧ��vector��Ϊ�գ�����true�����򷵻�false
        return (it != m_items.end() && !it->second.empty());
    }

    const std::map<std::string, std::vector<std::shared_ptr<Item>>>& GetAllItems() const {
        return m_items;
    }
};



#endif