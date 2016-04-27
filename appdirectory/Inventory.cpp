#include "Inventory.h"

Inventory::Inventory(const std::string& _fn) : currentItemIndex(0) {
	// based on xml parser found here http://www.boost.org/doc/libs/1_42_0/doc/html/boost_propertytree/tutorial.html

    using boost::property_tree::ptree;
    ptree pt;
    read_xml(_fn, pt);

    BOOST_FOREACH(ptree::value_type &v, pt.get_child("inventoryDatabase")) {
        std::string name = v.second.get<std::string>("item.name");
        std::string type = v.second.get<std::string>("item.type");
        int deltaHealth = v.second.get<int>("item.delta-health");
        int deltaArmor = v.second.get<int>("item.delta-armor");
        int deltaDamage = v.second.get<int>("item.delta-damage");
        int deltaSpecial = v.second.get<int>("item.delta-special");
        float deltaAccuracy = v.second.get<float>("item.delta-accuracy");
        int quantity = v.second.get<int>("quantity");
        Item item(name, type, deltaHealth, deltaArmor, deltaDamage, deltaSpecial, deltaAccuracy);
        items.push_back(std::pair<Item,int>(item,quantity));
    }

    for (auto pair : items) {
        std::string info;
        info += pair.first.name + " (";
        info += std::to_string(pair.second) + ") | ";
        info += pair.first.type + " | ";
        std::cout << info << std::endl;
    }
}

const Item& Inventory::getCurrentItem() {
    return items.at(currentItemIndex).first;
}

std::string Inventory::getCurrentItemMenuTitle() {
    return items.at(currentItemIndex).first.name + " (" + std::to_string(items.at(currentItemIndex).second) + ")";
}

int Inventory::getCurrentItemCount() {
    return items.at(currentItemIndex).second;
}

void Inventory::cycleInventoryForward() {
    currentItemIndex = (currentItemIndex + 1) % items.size();
}

void Inventory::cycleInventoryBackward() {
    int ind = currentItemIndex - 1;
    currentItemIndex = ind == -1 ? items.size() - 1 : ind;
}

void Inventory::useItem(Player& target) {
    
    auto& item = items.at(currentItemIndex);
    item.first.use(target);

    item.second--;
    if (item.second == 0) {
        items.erase(items.begin() + currentItemIndex);
        currentItemIndex = std::min(currentItemIndex, (int)items.size() - 1);
    }
}