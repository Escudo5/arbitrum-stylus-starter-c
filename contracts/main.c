#include "../include/stylus_sdk.h"
#include "../stylus-sdk-c/include/stylus_utils.h"
#include "../stylus-sdk-c/include/storage.h"
#include "../stylus-sdk-c/include/string.h"

#define MAX_PROPERTIES 10
#define STORAGE_SLOT__properties 0x0 // Base slot for property storage
#define STORAGE_SLOT__balance 0x10   // Slot for storing balance

typedef struct
{
  uint8_t id[20];       // Property ID
  uint8_t owner[32];    // Owner address
  uint64_t price;       // Property price
  uint8_t is_sold;      // 0 = available, 1 = sold
} Property;

// Temporary buffers to avoid malloc
Property properties[MAX_PROPERTIES];
uint8_t buf_out[32]; // Used for output

// Helper to return success with a message
ArbResult inline _return_success_msg(const char *msg)
{
  ArbResult res = {Success, (uint8_t *)msg, strlen(msg)};
  return res;
}

// Helper to save properties to storage
void save_properties()
{
  storage_cache_bytes32((uint8_t *)STORAGE_SLOT__properties, (uint8_t *)properties);
  storage_flush_cache(false);
}

// Helper to load properties from storage
void load_properties()
{
  storage_load_bytes32((uint8_t *)STORAGE_SLOT__properties, (uint8_t *)properties);
}

// Function to register a property
ArbResult register_property(uint8_t *input, size_t len)
{
  if (len < 32 + 8)
  {
    return _return_short_string(Failure, "InvalidInput");
  }

  load_properties();

  // Find a free slot
  for (int i = 0; i < MAX_PROPERTIES; i++)
  {
    if (properties[i].id[0] == 0) // Empty slot
    {
      memcpy(properties[i].id, input, 20);
      memcpy(properties[i].owner, input + 20, 32);
      properties[i].price = *((uint64_t *)(input + 52));
      properties[i].is_sold = 0;
      save_properties();
      return _return_success_msg("PropertyRegistered");
    }
  }

  return _return_short_string(Failure, "NoFreeSlots");
}

// Function to list available properties
ArbResult list_properties(uint8_t *input, size_t len)
{
  load_properties();

  uint8_t *ptr = buf_out;
  for (int i = 0; i < MAX_PROPERTIES; i++)
  {
    if (!properties[i].is_sold && properties[i].id[0] != 0)
    {
      memcpy(ptr, properties[i].id, 20);
      ptr += 20;
      *((uint64_t *)ptr) = properties[i].price;
      ptr += 8;
    }
  }

  return (ArbResult){Success, buf_out, ptr - buf_out};
}

// Function to buy a property
ArbResult buy_property(uint8_t *input, size_t len)
{
  if (len < 20 + 8)
  {
    return _return_short_string(Failure, "InvalidInput");
  }

  load_properties();

  uint8_t *property_id = input;
  uint64_t payment = *((uint64_t *)(input + 20));

  for (int i = 0; i < MAX_PROPERTIES; i++)
  {
    if (memcmp(properties[i].id, property_id, 20) == 0 && !properties[i].is_sold)
    {
      if (payment >= properties[i].price)
      {
        properties[i].is_sold = 1;
        save_properties();

        // Update balance
        uint64_t balance;
        storage_load_bytes32((uint8_t *)STORAGE_SLOT__balance, (uint8_t *)&balance);
        balance += payment;
        storage_cache_bytes32((uint8_t *)STORAGE_SLOT__balance, (uint8_t *)&balance);
        storage_flush_cache(false);

        return _return_success_msg("PurchaseSuccessful");
      }
      else
      {
        return _return_short_string(Failure, "InsufficientPayment");
      }
    }
  }

  return _return_short_string(Failure, "PropertyNotFound");
}

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
