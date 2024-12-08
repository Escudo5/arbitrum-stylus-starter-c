import { createPublicClient, createWalletClient, http, parseAbi } from "viem";
import { arbitrumSepolia } from "viem/chains";
import { privateKeyToAccount } from "viem/accounts";
import "dotenv/config";

// ABI del contrato
const ABI = parseAbi([
  "function buy_property(uint256 property_id) public",
  "function get_property(uint256 property_id) view returns (address owner, bool is_sold)"
]);

// Configuración de la cuenta y cliente
const account = privateKeyToAccount(process.env.PRIVATE_KEY);

const client = createWalletClient({
  chain: arbitrumSepolia,
  transport: http(),
  account,
});

const publicClient = createPublicClient({
  chain: arbitrumSepolia,
  transport: http(),
});

// Dirección del contrato desplegado
const CONTRACT_ADDRESS = "0x35359e90606838bc2dbc0da9a462d7401ba795ff"; // Actualiza con tu dirección

// Función para comprar una propiedad
async function buyProperty(propertyId) {
  try {
    // Enviar la transacción
    const result = await client.writeContract({
      abi: ABI,
      address: CONTRACT_ADDRESS,
      functionName: "buy_property",
      args: [BigInt(propertyId)],
    });
    
    console.debug(`Transaction hash: ${result.hash}`);
    
    // Esperar confirmación de la transacción
    const receipt = await client.waitForTransactionReceipt(result.hash);
    
    if (receipt.status === 'success') {
      console.log(`Property ${propertyId} bought successfully!`);
    } else {
      console.error(`Transaction failed: ${receipt.status}`);
    }
  } catch (error) {
    console.error("Error buying property:", error);
  }
}

// Función para obtener detalles de una propiedad
async function getProperty(propertyId) {
  try {
    const result = await publicClient.readContract({
      abi: ABI,
      address: CONTRACT_ADDRESS,
      functionName: "get_property",
      args: [BigInt(propertyId)],
    });
    console.debug(`Property Details: ${JSON.stringify(result)}`);
  } catch (error) {
    console.error("Error fetching property details:", error);
  }
}

// Ejecución de pruebas (Descomenta según lo que quieras probar)
(async () => {
   await buyProperty(1); // Cambia "1" por el ID de la propiedad que quieres comprar
   await getProperty(1); // Cambia "1" por el ID de la propiedad que quieres consultar
})();
