import { lazy } from 'react';

const Dashboard = lazy(() => import('./dashboard'));
const DefaultContainer = lazy(() => import('./default_container'));
const Users = lazy(() => import('./users'));
const Culture = lazy(() => import('./cultures'));
const Login = lazy(() => import('./login/login'));

export { DefaultContainer, Dashboard, Users, Culture, Login };
