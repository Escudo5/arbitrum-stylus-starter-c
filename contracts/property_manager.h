#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <stdint.h>
#include <stddef.h>
#include "arb_result.h"

#define MAX_PROPERTIES 100


typedef char Address[42]; // Ejemplo, ajusta según el formato real de una dirección.


typedef uint64_t uint256_t; // Temporal, mientras ajustamos la lógica.



// Definición de la estructura Property
typedef struct
{
    uint8_t id[20];       // Property ID
    uint8_t owner[32];    // Owner address
    uint64_t price;       // Property price
    uint8_t is_sold;      // 0 = available, 1 = sold
    char hash[64];        // Hash of the property
} Property;

// Declaración externa de la variable properties
extern Property properties[MAX_PROPERTIES];

extern uint8_t buf_out[32]; // Buffer for output

// Funciones para la gestión de propiedades
ArbResult register_property(uint8_t *input, size_t size);
ArbResult list_properties(uint8_t *input, size_t len);
void save_property_to_storage(Property *property, const uint8_t *hash, uint8_t slot);
void read_purchase_data(uint8_t *input, uint8_t *property_id, uint256_t *offer_price, uint8_t *buyer);
ArbResult buy_property(uint8_t *input, size_t data_len);
const uint8_t *retrieve_property_from_storage(const uint8_t *property_id, Property *property, uint8_t slot);
const char *get_property_hash(int index);
int set_property_hash(int index, const char *hash, uint64_t owner);
void generate_property_hash(Property *property, uint8_t *output_hash);
int get_user_reputation(uint8_t *user_address);
void update_user_reputation(uint8_t *user_address, int change);
void emit_sale_event(uint8_t *buyer, uint8_t *seller, uint8_t *property_id, uint64_t price, const uint8_t *hash);
void fail_transaction(const char *message);
ArbResult update_balance(uint8_t *owner, uint256_t amount, uint8_t *storage_slot);
void transfer_property(Property *property, Address new_owner, uint8_t *storage_slot);
void list_all_properties(uint8_t slot);
int read_property_data(uint8_t *input, Property *property, uint256_t *price);




#endif
