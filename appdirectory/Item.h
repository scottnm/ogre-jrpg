#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>

class Item {
public:
	Item(std::string name, std::string type, std::string description);

	std::string name;
	std::string type;
	std::string description;
};

#endif // __INVENTORY_H__