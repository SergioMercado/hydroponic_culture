import React from 'react';
import { HydraCard, Table, UserForm } from '../components';

export default function Culture() {
  return (
    <>
      <div className='row'>
        <UserForm />

        <HydraCard title='Culture List'>
          <Table />
        </HydraCard>
      </div>
    </>
  );
}
