import React, { Suspense } from 'react';
import { BrowserRouter as Router, Switch, Route } from 'react-router-dom';
import { library } from '@fortawesome/fontawesome-svg-core';
import { fab } from '@fortawesome/free-brands-svg-icons';
import {
  faSignOutAlt,
  faChartArea,
  faUsers,
  faAssistiveListeningSystems,
  faSeedling,
  faShoppingBasket,
} from '@fortawesome/free-solid-svg-icons';
import { Preloader } from '../components';
import { DefaultContainer, Dashboard, Users, Culture } from './';

library.add(
  fab,
  faSignOutAlt,
  faChartArea,
  faUsers,
  faAssistiveListeningSystems,
  faSeedling,
  faShoppingBasket,
);

export default function RootContainer(props) {
  return (
    <Router>
      <Suspense fallback={<Preloader />}>
        <Switch>
          {/* <Route exact path='/' component={Login} />*/}

          <Route
            exact
            path='/'
            render={() => (
              <DefaultContainer title='Dashboard Metrics'>
                <Dashboard />
              </DefaultContainer>
            )}
          />

          <Route
            exact
            path='/dashboard'
            render={() => (
              <DefaultContainer title='Dashboard Metrics'>
                <Dashboard />
              </DefaultContainer>
            )}
          />

          <Route
            exact
            path='/users'
            render={() => (
              <DefaultContainer title='Users'>
                <Users />
              </DefaultContainer>
            )}
          />

          <Route
            exact
            path='/sensors'
            render={() => (
              <DefaultContainer title='Sensor'>
                Hello World Sensor List
              </DefaultContainer>
            )}
          />

          <Route
            exact
            path='/cultures'
            render={() => (
              <DefaultContainer title='Culture'>
                <Culture />
              </DefaultContainer>
            )}
          />
        </Switch>
      </Suspense>
    </Router>
  );
}
