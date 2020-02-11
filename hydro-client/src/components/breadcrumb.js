import React from 'react';
import { Link } from 'react-router-dom';

const BreadcrumbItem = ({ title, path }) => (
  <li className='breadcrumb-item'>
    <Link to={path}>{title}</Link>
  </li>
);

export default function Breadcrumb({ breadcrumbLinks }) {
  return (
    <div className='btn-group float-right'>
      <ol className='breadcrumb hide-phone p-0 m-0'>
        <BreadcrumbItem title='Dashboard' />
      </ol>
    </div>
  );
}
