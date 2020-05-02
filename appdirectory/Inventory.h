#ifndef _WIN32 // TODO: get to compile

#pragma once

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

	std::vector<std::pair<Item, int>> items;
    std::vector<std::pair<Item, int>> _itemDefaults;
	int currentItemIndex;
};

#endif // _WIN32
