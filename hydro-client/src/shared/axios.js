import axios from 'axios';

const instance = axios.create({
  baseURL: 'https://hydro-udc.herokuapp.com',
});

instance.CancelToken = axios.CancelToken;
instance.isCancel = axios.isCancel;
instance.Cancel = axios.Cancel;

export default instance;
