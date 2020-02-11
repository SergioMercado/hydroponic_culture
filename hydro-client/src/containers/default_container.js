import React from 'react';
import { Footer, LeftMenuSidebar, TopBarMenu } from '../components';

export default function Default({ title, children }) {
  return (
    <div id='wrapper'>
      <LeftMenuSidebar />

      <div className='content-page'>
        <div className='content'>
          <TopBarMenu />

          <div className='page-content-wrapper '>
            <div className='container-fluid'>
              <div className='row'>
                <div className='col-sm-12'>
                  <div className='page-title-box'>
                    {/* <Breadcrumb /> */}
                    <h4 className='page-title'>{title}</h4>
                  </div>
                </div>
                <div className='clearfix'></div>
              </div>
              {children}
            </div>
          </div>
        </div>

        <Footer />
      </div>
    </div>
  );
}
