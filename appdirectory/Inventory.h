#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include "Item.h"

class Inventory {
public:
	Inventory(const std::string& fn);

	const Item& getCurrentItem();
	std::string getCurrentItemName();
	std::string getCurrentItemDescription();
	int getCurrentItemCount();
	void cycleInventoryForward();
	void cycleInventoryBackward();
	void useItem(Player& target);
    void reset(void);

	/**
     * load the sounds specified by the xml audio database file
     * @param xmlDbFilename - the name of the xml audio database file
     */
    //void loadInventoryDatabase(const std::string& xmlDbFilename);
	
	std::vector<std::pair<Item, int>> items;
    std::vector<std::pair<Item, int>> _itemDefaults;
	int currentItemIndex;
};

#endif // __INVENTORY_H__
