import Link from 'next/link';
import Navigation from '../Navigation';
import { useState } from 'react';
import { getProperty, buyProperty } from '../utils/blockchain';

export default function LandingPage() {
  const [propertyId, setPropertyId] = useState<number>(0);
  const [propertyInfo, setPropertyInfo] = useState<any>(null);

  const fetchProperty = async () => {
    try {
      const data = await getProperty(propertyId);
      setPropertyInfo(data);
    } catch (error) {
      console.error('Error fetching property:', error);
    }
  };

  const purchaseProperty = async () => {
    try {
      await buyProperty(propertyId);
      alert('Property purchased successfully!');
    } catch (error) {
      console.error('Error buying property:', error);
    }
  };

  return (
    <>
      <Navigation />
      <div style={{ backgroundColor: '#1DA1F2', minHeight: '100vh', padding: '20px' }}>
        <div style={{ marginBottom: '20px' }}>
          <label style={{ color: 'white', fontSize: '18px', marginRight: '10px' }}>
            Enter Property ID:
          </label>
          <input
            type="number"
            value={propertyId}
            onChange={(e) => setPropertyId(Number(e.target.value))}
            style={{ padding: '5px', borderRadius: '5px' }}
          />
        </div>
        <button onClick={fetchProperty} style={{ marginRight: '10px', padding: '10px', borderRadius: '5px' }}>
          Get Property Info
        </button>
        <button onClick={purchaseProperty} style={{ padding: '10px', borderRadius: '5px' }}>
          Buy Property
        </button>
        {propertyInfo && (
          <div style={{ marginTop: '20px', color: 'white' }}>
            <h3>Property Info:</h3>
            <p>Owner: {propertyInfo[0]}</p>
            <p>Price: {propertyInfo[1]}</p>
            <p>Description: {propertyInfo[2]}</p>
          </div>
        )}
      </div>
    </>
  );
}
