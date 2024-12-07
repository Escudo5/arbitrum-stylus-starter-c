#include "property_manager.h"
#include <string.h>
#include <keccak256.h>
//#include <stdio.h>

Property properties[MAX_PROPERTIES];
uint8_t buf_out[32]; // Buffer for output

// Implementación personalizada de my_memcmp
int my_memcmp(const void *ptr1, const void *ptr2, size_t num)
{
    const unsigned char *p1 = ptr1;
    const unsigned char *p2 = ptr2;

    for (size_t i = 0; i < num; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }

    return 0; // Las cadenas son iguales
}

const char *get_property_hash(int index)
{
    if (index < 0 || index >= MAX_PROPERTIES) {
        return NULL;
    }
    return properties[index].hash;
}

int set_property_hash(int index, const char *hash, uint64_t owner)
{
    if (index < 0 || index >= MAX_PROPERTIES || !hash) {
        return 0; // Invalid index or hash
    }
    
    // Store the hash for the property at the given index
    strncpy(properties[index].hash, hash, sizeof(properties[index].hash) - 1);
    properties[index].hash[sizeof(properties[index].hash) - 1] = '\0'; // Ensure null-termination
    
    // Optionally, associate the owner to the hash (here you can customize as needed)
    memcpy(properties[index].owner, &owner, sizeof(properties[index].owner));

    return 1; // Success
}


// Función para generar un hash único basado en la propiedad
const char *generate_property_hash(Property *property) {
    static char hash[64];
    // Concatenamos los datos de la propiedad para generar un hash único
    char data[200];
    snprintf(data, sizeof(data), "%s%s%llu", property->id, property->owner, property->price);
    
    // Llamamos a la función keccak256 para generar el hash
    keccak256((uint8_t *)data, strlen(data), (uint8_t *)hash);
    return hash;
}

int register_property(uint8_t *input, size_t len)
{
    if (len < 32 + 8)
    {
        return 0; // Invalid input
    }

    // Find a free slot
    for (int i = 0; i < MAX_PROPERTIES; i++)
    {
        if (properties[i].id[0] == 0) // Empty slot
        {
            memcpy(properties[i].id, input, 20);
            memcpy(properties[i].owner, input + 20, 32);
            properties[i].price = *((uint64_t *)(input + 52));
            properties[i].is_sold = 0;

            // Register the property hash (using some example hash)
            const char *hash = "abcd1234abcd1234abcd1234abcd1234abcd1234abcd1234abcd1234abcd1234";
            if (!set_property_hash(i, hash, *((uint64_t *)(input + 20)))) {
                return 0; // Hash registration failed
            }

            return 1; // Success
        }
    }

    return 0; // No free slots
}

void list_properties(uint8_t *input, size_t len)
{
    uint8_t *ptr = buf_out;

    for (int i = 0; i < MAX_PROPERTIES; i++)
    {
        if (!properties[i].is_sold && properties[i].id[0] != 0)
        {
            memcpy(ptr, properties[i].id, 20);
            ptr += 20;
            *((uint64_t *)ptr) = properties[i].price;
            ptr += 8;

            // Add hash to the listing
            const char *hash = get_property_hash(i);
            if (hash) {
                memcpy(ptr, hash, strlen(hash));
                ptr += strlen(hash);
            }
        }
    }

    return ptr - buf_out; // Return the number of bytes used in buf_out
}

int buy_property(uint8_t *input, size_t len)
{
    if (len < 20 + 8)
    {
        return 0; // Invalid input
    }

    uint8_t *property_id = input;
    uint64_t payment = *((uint64_t *)(input + 20));

    for (int i = 0; i < MAX_PROPERTIES; i++)
    {
        if (my_memcmp(properties[i].id, property_id, 20) == 0 && !properties[i].is_sold)
        {
            if (payment >= properties[i].price)
            {
                properties[i].is_sold = 1;

                // Save properties logic (assumed saved elsewhere)
                return 1; // Success
            }
            else
            {
                return 0; // Insufficient payment
            }
        }
    }

    return 0; // Property not found
}

// Registro global de eventos de venta
SaleEvent sale_events[MAX_PROPERTIES];
int sale_event_count = 0;

// Función para emitir un evento de compra/venta
void emit_sale_event(uint8_t *buyer, uint8_t *seller, uint8_t *property_id, uint64_t price, const char *hash) {
    if (sale_event_count < MAX_PROPERTIES) {
        memcpy(sale_events[sale_event_count].buyer, buyer, 32);
        memcpy(sale_events[sale_event_count].seller, seller, 32);
        memcpy(sale_events[sale_event_count].property_id, property_id, 20);
        sale_events[sale_event_count].price = price;
        strncpy(sale_events[sale_event_count].hash, hash, 64);
        sale_event_count++;
    }
}


// Estructura para almacenar reputación de usuarios
typedef struct {
    uint8_t address[32];  // Dirección del usuario
    int reputation;       // Reputación del usuario
} UserReputation;

UserReputation user_reputations[MAX_USERS];  // Registro global de reputación

// Obtiene la reputación actual de un usuario
int get_user_reputation(uint8_t *user_address) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (my_memcmp(user_reputations[i].address, user_address, 32) == 0) {
            return user_reputations[i].reputation;
        }
    }
    return 0;  // Si el usuario no existe, retorna reputación 0
}

// Actualiza la reputación de un usuario
void update_user_reputation(uint8_t *user_address, int change) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (my_memcmp(user_reputations[i].address, user_address, 32) == 0) {
            user_reputations[i].reputation += change;
            return;
        }
    }
    // Si el usuario no existe, añade uno nuevo
    for (int i = 0; i < MAX_USERS; i++) {
        if (user_reputations[i].reputation == 0) {
            memcpy(user_reputations[i].address, user_address, 32);
            user_reputations[i].reputation = change;
            return;
        }
    }
}