import { createPublicClient, createWalletClient, http } from 'viem';
import { arbitrum } from 'viem/chains';

export const contractAddress = '0x35359e90606838bc2dbc0da9a462d7401ba795ff';
export const abi = [
  {
    name: 'buy_property',
    type: 'function',
    stateMutability: 'nonpayable',
    inputs: [{ type: 'uint256', name: 'property_id' }],
    outputs: [],
  },
  {
    name: 'get_property',
    type: 'function',
    stateMutability: 'view',
    inputs: [{ type: 'uint256', name: 'property_id' }],
    outputs: [
      {
        type: 'tuple',
        components: [
          { name: 'owner', type: 'address' },
          { name: 'price', type: 'uint256' },
        ],
      },
    ],
  },
];

export const publicClient = createPublicClient({
  chain: arbitrum,
  transport: http(),
});

export const walletClient = createWalletClient({
  chain: arbitrum,
  transport: http(),
});
