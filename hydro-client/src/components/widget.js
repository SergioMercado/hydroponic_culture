import React from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
export default function Widget({ title, total, icon, bgColor = 'bg-danger' }) {
  return (
    <div className='col-sm-12 col-md-6 col-xl-3'>
      <div className={`card ${bgColor} m-b-30`}>
        <div className='card-body'>
          <div className='d-flex row'>
            <div className='col-3 align-self-center'>
              <div className='round'>
                <FontAwesomeIcon className='m-r-5 text-white' icon={icon} />
              </div>
            </div>
            <div className='col-8 ml-auto align-self-center text-center'>
              <div className='m-l-10 text-white float-right'>
                <h5 className='mt-0 round-inner'>{total}</h5>
                <p className='mb-0 '>{title}</p>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
