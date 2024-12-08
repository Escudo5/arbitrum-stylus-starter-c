import Link from 'next/link';
import Navigation from '../Navigation';
//import { useState } from 'react';

export default function LandingPage() {
    //const [input, setInput] = useState('');

    return (
        <>
    <Navigation/>
    <div style={{ backgroundColor: '#1DA1F2', height: '100vh', display: 'flex', justifyContent: 'center', alignItems: 'center' }}>
      <Link href="https://17t.1da.myftpupload.com/">
        <button style={{ backgroundColor: 'white', opacity: 100, padding: '15px 30px', borderRadius: '50px', border: 'none', cursor: 'pointer' }}>
          Get VERIFY! Become a P.M.I. Member Today!
        </button>
      </Link>
    </div>
    </>
  );
}