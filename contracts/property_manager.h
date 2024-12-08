#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <stddef.h> // Para size_t

// Códigos de estado para los resultados
#define ARB_INVALID_INPUT 1
#define ARB_SUCCESS 0
#define ARB_NO_FREE_SLOT 2
#define ARB_OUTPUT_TOO_SMALL 3
#define ARB_INSUFFICIENT_FUNDS 4
#define ARB_PROPERTY_NOT_FOUND 5

// Estructura para devolver los resultados
typedef struct {
    int code;
    const char *message;
} ArbResultPedro;

// Declaración de funciones
ArbResultPedro create_result(int code);
ArbResultPedro add_property(size_t *used_len);
ArbResultPedro remove_property(int property_id);
ArbResultPedro read_property_data(int property_id);
ArbResultPedro transfer_property(int property_id, int new_owner_id);

// Función para verificar si un ID de propiedad es válido
int is_valid_property_id(int property_id);

#endif // PROPERTY_MANAGER_H
