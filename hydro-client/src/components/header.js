import React from 'react';
import { Link } from 'react-router-dom';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import uuid from 'uuid/v4';
import configRoutes from '../routes';

export const TopBarMenu = () => (
  <div className='topbar'>
    <nav className='navbar-custom'>
      <ul className='list-inline float-right mb-0 mr-3'>
        <li className='list-inline-item dropdown notification-list'>
          <span
            className='nav-link dropdown-toggle arrow-none waves-effect nav-user'
            data-toggle='dropdown'
            role='button'
            aria-haspopup='false'
            aria-expanded='true'
          >
            <img
              src='assets/images/users/avatar-1.jpg'
              alt='user'
              className='rounded-circle img-thumbnail'
            />
          </span>
          <div className='dropdown-menu dropdown-menu-right profile-dropdown '>
            <div className='dropdown-item noti-title'>
              <h5>Welcome</h5>
            </div>

            <span className='dropdown-item pointer'>
              <FontAwesomeIcon
                className='m-r-5 text-muted'
                icon='sign-out-alt'
              />{' '}
              Logout
            </span>
          </div>
        </li>
      </ul>

      <ul className='list-inline menu-left mb-0'>
        <li className='float-left'>
          <button className='button-menu-mobile open-left waves-light waves-effect'>
            <i className='mdi mdi-menu'></i>
          </button>
        </li>
      </ul>

      <div className='clearfix'></div>
    </nav>
  </div>
);
const Logo = ({ imgPath, alt, to = '/dashboard' }) => (
  <div className='topbar-left'>
    <div 
    className='text-center'
     style={{
       display: 'flex',
       alignItems: 'center',
       justifyContent: 'center'
    }}>
      <Link to={to} className='logo'>
        {/* <img src={imgPath} alt={alt} className='logo-large' /> */}
        <h4 className='text-center text-white'>Hydra Analytic</h4>
      </Link>
    </div>
  </div>
);
const NavigationItem = ({ label, to, icon }) => (
  <li>
    <Link to={to} className='waves-effect'>
      <FontAwesomeIcon className='m-r-5 text-muted' icon={icon} />
      <span>{label}</span>
    </Link>
  </li>
);

const NavigationItems = () => (
  <ul>
    <li className='menu-title'>Analytic</li>
    {configRoutes.map(route => (
      <NavigationItem {...route} key={uuid()} />
    ))}
  </ul>
);

export default function LeftMenuSidebar() {
  return (
    <div className='left side-menu position-fixed'>
      <button
        type='button'
        className='button-menu-mobile button-menu-mobile-topbar open-left waves-effect'
      >
        <i className='mdi mdi-close' />
      </button>

      <Logo imgPath='assets/images/logo-lg.jpeg' alt='logo hydra analytic' />

      <div className='sidebar-inner slimscrollleft' id='sidebar-main'>
        <div id='sidebar-menu'>
          <NavigationItems />
        </div>
        <div className='clearfix'></div>
      </div>
    </div>
  );
}
