import React from 'react';
import { HydraCard, Table, UserForm } from '../components';

export default function Users() {
  return (
    <>
      <div className='row'>
        <UserForm />

        <HydraCard title='User List'>
          <Table />
        </HydraCard>
      </div>
    </>
  );
}
