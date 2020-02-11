import React from 'react';
import { HydraCard } from '.';

export default function UserForm({ action = 'Create' }) {
  return (
    <HydraCard>
      <h4 class='mt-0 header-title'>{action} User</h4>
      <p class='text-muted  font-14'>
        Similar to the contextual text color classes, easily set the background
        of an element to any contextual class. Anchor components will darken on
        hover, just like the text classes.
      </p>
      <div class='general-label'>
        <form class='mb-0'>
          <div class='form-group'>
            <label
              for='exampleInputEmail1'
              class='bmd-label-floating text-dark'
            >
              Email address
            </label>
            <input type='email' class='form-control' id='exampleInputEmail1' />
            <span class='bmd-help'>
              We'll never share your email with anyone else.
            </span>
          </div>
          <div class='form-group'>
            <label
              for='exampleInputPassword1'
              class='bmd-label-floating text-dark'
            >
              Password
            </label>
            <input
              type='password'
              class='form-control'
              id='exampleInputPassword1'
            />
          </div>

          <div class='form-group'>
            <label for='exampleTextarea' class='bmd-label-floating text-dark'>
              Example textarea
            </label>
            <textarea
              class='form-control'
              id='exampleTextarea'
              rows='3'
            ></textarea>
          </div>

          <div class='checkbox mb-2'>
            <label className='text-dark'>
              <input type='checkbox' />
              <span class='checkbox-decorator'>
                <span class='check'></span>
                <div class='ripple-container'></div>
              </span>{' '}
              Check me out
            </label>
          </div>
          <button type='submit' class='btn btn-primary btn-raised mb-0'>
            Submit
          </button>
        </form>
      </div>
    </HydraCard>
  );
}
