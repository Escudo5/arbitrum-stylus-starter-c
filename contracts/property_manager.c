#include "property_manager.h"
#include <stddef.h> // Para size_t
#include <stdio.h>  // Para printf (en pruebas)

// Mensajes asociados a cada código de estado
const char *messages[] = {
    "Success",
    "Invalid input",
    "No free slot available",
    "Output too small",
    "Insufficient funds",
    "Property not found"
};

// Función auxiliar para crear un resultado con un código de estado
ArbResultPedro create_result(int code) {
    ArbResultPedro result;
    result.code = code;
    
    if (code >= 0 && code < sizeof(messages) / sizeof(messages[0])) {
        result.message = messages[code];
    } else {
        result.message = "Unknown error";
    }
    
    return result;
}

// Función para agregar una propiedad
ArbResultPedro add_property(size_t *used_len) {
    if (!used_len) {
        return create_result(ARB_INVALID_INPUT);
    }

    size_t calculated_len = 42; // Ejemplo de cálculo
    if (calculated_len > 100) { // Simulación de un caso de error
        return create_result(ARB_OUTPUT_TOO_SMALL);
    }

    *used_len = calculated_len;
    return create_result(ARB_SUCCESS);
}

// Función para eliminar una propiedad
ArbResultPedro remove_property(int property_id) {
    if (property_id < 0) {
        return create_result(ARB_INVALID_INPUT);
    }

    if (!is_valid_property_id(property_id)) {
        return create_result(ARB_PROPERTY_NOT_FOUND);
    }

    return create_result(ARB_SUCCESS);
}

// Función para leer los datos de una propiedad
ArbResultPedro read_property_data(int property_id) {
    if (property_id < 0) {
        return create_result(ARB_INVALID_INPUT);
    }

    if (!is_valid_property_id(property_id)) {
        return create_result(ARB_PROPERTY_NOT_FOUND);
    }

    return create_result(ARB_SUCCESS);
}

// Función para transferir la propiedad a otro propietario
ArbResultPedro transfer_property(int property_id, int new_owner_id) {
    if (property_id < 0 || new_owner_id < 0) {
        return create_result(ARB_INVALID_INPUT);
    }

    if (!is_valid_property_id(property_id)) {
        return create_result(ARB_PROPERTY_NOT_FOUND);
    }

    return create_result(ARB_SUCCESS);
}

// Función para verificar si un ID de propiedad es válido
int is_valid_property_id(int property_id) {
    // Lógica de ejemplo: Si el ID es mayor que 100, no es válido
    return property_id <= 100;
}

// Main de prueba (se compila solo si se define TEST)
#ifdef TEST
int main() {
    size_t used_len;
    ArbResultPedro result = add_property(&used_len);

    if (result.code != ARB_SUCCESS) {
        printf("Error: %s\n", result.message);
    } else {
        printf("Property added successfully. Used length: %zu\n", used_len);
    }

    result = remove_property(200); // ID no válido
    if (result.code != ARB_SUCCESS) {
        printf("Error: %s\n", result.message);
    } else {
        printf("Property removed successfully.\n");
    }

    result = read_property_data(150); // ID no válido
    if (result.code != ARB_SUCCESS) {
        printf("Error reading property: %s\n", result.message);
    } else {
        printf("Property data read successfully.\n");
    }

    result = transfer_property(50, 10); // ID válido
    if (result.code != ARB_SUCCESS) {
        printf("Error transferring property: %s\n", result.message);
    } else {
        printf("Property transferred successfully.\n");
    }

    return 0;
}
#endif
