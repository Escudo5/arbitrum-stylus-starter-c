#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <stdint.h>
#include <stddef.h>

#define MAX_PROPERTIES 10

typedef struct
{
    uint8_t id[20];       // Property ID
    uint8_t owner[32];    // Owner address
    uint64_t price;       // Property price
    uint8_t is_sold;      // 0 = available, 1 = sold
    char hash[64];        // Hash of the property
} Property;

extern Property properties[MAX_PROPERTIES];
extern uint8_t buf_out[32]; // Buffer for output

// Function to register a property
int register_property(uint8_t *input, size_t len);

// Function to list available properties
int list_properties(uint8_t *input, size_t len);

// Function to buy a property
int buy_property(uint8_t *input, size_t len);

// Function to get the hash of a property
const char *get_property_hash(int index);

// Function to set the hash of a property
int set_property_hash(int index, const char *hash, uint64_t owner);
#endif
