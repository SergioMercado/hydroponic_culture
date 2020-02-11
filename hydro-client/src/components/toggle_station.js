import React from 'react';
export default function ToggleStation({ status, changeStatusHandler }) {
  return (
    <div className='custom-control custom-switch'>
      <input
        type='checkbox'
        className='custom-control-input'
        id='customSwitch1'
        checked={status}
        onChange={changeStatusHandler}
      />
      <label className='custom-control-label' htmlFor='customSwitch1'>
        {status ? 'On' : 'Off'}
      </label>
    </div>
  );
}
