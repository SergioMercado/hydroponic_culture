import React from 'react';

export default function Footer() {
  const year = new Date().getFullYear();
  return <footer className='footer'>© {year} Hydra Analytic.</footer>;
}
