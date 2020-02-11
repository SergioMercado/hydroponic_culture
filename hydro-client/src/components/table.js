import React from 'react';
import uuid from 'uuid/v4';

export default function Table({ header, children }) {
  return (
    <div className='table-responsive'>
      <table className='table table-bordered'>
        <thead>
          <tr>
            {header.map(value => (
              <th key={uuid()}>{value}</th>
            ))}
          </tr>
        </thead>

        <tbody>{children}</tbody>
      </table>
    </div>
  );
}
