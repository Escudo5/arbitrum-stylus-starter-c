#include "../include/stylus_sdk.h"
#include "../stylus-sdk-c/include/stylus_utils.h"
#include "../stylus-sdk-c/include/storage.h"
#include "../stylus-sdk-c/include/string.h"
#include "property_manager.h"

#define STORAGE_SLOT__properties 0x0 // Base slot for property storage
#define STORAGE_SLOT__balance 0x10   // Slot for storing balance

// Main handler
int handler(size_t argc)
{
    uint8_t argv[argc];
    read_args(argv);

    FunctionRegistry registry[] = {
        {to_function_selector("register_property(bytes,uint256)"), register_property},
        {to_function_selector("list_properties()"), list_properties},
        {to_function_selector("buy_property(bytes,uint256)"), buy_property},
    };

    uint32_t signature = *((uint32_t *)argv);

    ArbResult res = call_function(registry,
                                  sizeof(registry) / sizeof(registry[0]),
                                  signature, argv + 4, argc - 4);
    return (write_result(res.output, res.output_len), res.status);
}

ENTRYPOINT(handler)
