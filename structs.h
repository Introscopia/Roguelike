#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include "fundamentals.h"

typedef enum { miscellaneous, armor, melee_weapon, ranged_weapon, ammo, consumable } item_type;

typedef enum { head, torso, right_arm, left_arm, right_leg, left_leg } body_part;

// Main structs:

typedef struct item{
	char *name;
	int sprite_ID;
	int x, y;
	int weight;
	item_type type;
	void *data;
} Item;

typedef struct entity_struct{
	char *name;
	int sprite_ID;
	int x, y;

	int body_part_HP[6];
	int body_part_max_HP[6];
	int stamina;
	int carry_capacity;

	Item right_hand, left_hand;
	Item equipped_armor [6];
	Item *inventory;
	int inventory_size;
} Entity;


// o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o o 
// Specific structs for each item type:

typedef struct armor_struct{
	int defense;
	// this value just gets subtracted from incoming damage.
} Armor;

typedef struct melee_struct{
	int damage;
} Melee_weapon;

typedef struct ammo_struct{
	int damage;
	char ID;
} Ammo;
	
typedef struct {
	int base_damage;
	float range;
	char ammo_ID;
	int loaded_ammo;
} Ranged_weapon;

typedef struct consumable_struct{
	int delta_hp;
	int delta_stamina;
} Consumable;

#endif