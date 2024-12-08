export async function getProperty(propertyId: number) {
    try {
      return await publicClient.readContract({
        address: contractAddress,
        abi,
        functionName: 'get_property',
        args: [propertyId],
      });
    } catch (error) {
      console.error('Error al obtener la propiedad:', error);
      throw error;
    }
  }
  
  export async function buyProperty(propertyId: number) {
    try {
      return await walletClient.writeContract({
        address: contractAddress,
        abi,
        functionName: 'buy_property',
        args: [propertyId],
      });
    } catch (error) {
      console.error('Error al comprar la propiedad:', error);
      throw error;
    }
  }
  