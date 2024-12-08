import { createPublicClient, createWalletClient, http } from 'viem';
import { mainnet, arbitrumSepolia } from 'viem/chains';

// Dirección del nodo RPC (asegúrate de usar una URL válida)
const rpcUrl = 'https://arb-goerli.g.alchemy.com/v2/YOUR_ALCHEMY_API_KEY'; // Reemplaza con tu clave de API

// Cliente público para lecturas (no requiere firma)
export const publicClient = createPublicClient({
  chain: arbitrumSepolia,
  transport: http(rpcUrl),
});

// Cliente de billetera para escrituras (requiere firma)
export const walletClient = createWalletClient({
  chain: arbitrumSepolia,
  transport: http(rpcUrl),
});
