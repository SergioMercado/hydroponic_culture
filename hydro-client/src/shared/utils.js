import axios from './axios';
import { useEffect, useState } from 'react';

export function useFetch(url) {
  const [state, setState] = useState(null);
  const [error, setError] = useState('');

  useEffect(() => {
    const source = axios.CancelToken.source();

    async function fetchPosts() {
      try {
        const { data } = await axios.get(url, {
          cancelToken: source.token,
        });
        setState(data);
      } catch (error) {
        if (axios.isCancel(error)) {
          return;
        } else {
          setError(error.message);
        }
      }
    }

    fetchPosts();

    return () => {
      source.cancel();
    };
  }, [url]);

  return { state, error };
}
