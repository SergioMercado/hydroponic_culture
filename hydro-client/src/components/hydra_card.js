import React from 'react';

export default function HydraCard({ title, children }) {
  return (
    <div className='col-md-12 col-xl-12 px-0'>
      <div className='card m-b-30'>
        <div className='card-body'>
          <h4 className='mt-0 header-title'>{title}</h4>
          {children}
        </div>
      </div>
    </div>
  );
}
