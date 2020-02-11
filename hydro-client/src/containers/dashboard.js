import React, { useState, useEffect, useRef } from 'react';
import { HydraCard, Widget, Table, ToggleStation } from '../components';
import { Bar, Polar } from 'react-chartjs-2';
import { Spinner } from 'reactstrap';
import axios from '../shared/axios';
import { ToastContainer, toast } from 'react-toastify';
import { userHeader, userRows, plantHeader, plantRows } from '../shared/data';
import 'react-toastify/dist/ReactToastify.css';
import uuid from 'uuid';

const TIME = 10000;

const showNotification = (notification, sensor) => {
  switch (notification) {
    case 'stable':
      toast(`The ${sensor} levels are stable`, { type: 'info' });
      break;

    case 'low':
      toast(`The ${sensor} levels are low`, { type: 'warning' });
      break;

    case 'high':
      toast(`The ${sensor} levels are high`, {
        type: 'success',
      });
      break;

    default:
      return;
  }
};

const isDifferent = (sensor, saved) => {
  return sensor !== saved;
};

export default function Dashboard() {
  const [status, setStatus] = useState(null);
  const [metrics, setMetrics] = useState(null);
  let waterLevelColor = 'progress-bar';

  if (metrics) {
    if (metrics.notifications.waterLevel === 'low') {
      waterLevelColor += ' bg-warning';
    } else if (metrics.notifications.waterLevel === 'high') {
      waterLevelColor += ' bg-success';
    }
  }

  useEffect(() => {
    axios
      .get('/actuator/1', {
        params: { agentCode: 'mcu-h1', type: 'pump' },
      })
      .then(({ data }) => {
        setStatus(data.status);
      });
  }, []);

  useEffect(() => {
    function fetchMetrics() {
      axios
        .get('/sensor/1', {
          params: { agentCode: 'mcu-h1', type: 'pump' },
        })
        .then(({ data }) => {
          const datasets = data.reduce(
            (instance, { value, variable, notification }) => {
              instance[variable] = instance[variable]
                ? [...instance[variable], +value]
                : [+value];

              instance['notifications'] = {
                ...instance['notifications'],
                [variable]: notification,
              };
              return instance;
            },
            {},
          );
          localStorage.setItem(
            'notifications',
            JSON.stringify(datasets.notifications),
          );

          setMetrics(datasets);
        });
    }

    fetchMetrics();

    const interval = setInterval(fetchMetrics, TIME);

    return () => clearInterval(interval);
  }, []);
  
  const changeStatusOfStation = () => {
    axios
      .post('/actuator', {
        value: !status,
        agentCode: 'mcu-h1',
        agentId: 1,
      })
      .then(({ data }) => {
        if (data.message === 'Changed to off') {
          setStatus(false);
        } else {
          setStatus(true);
        }
      });
  };

  //showNotification(temperature, 'temperature');
  //showNotification(humidity, 'humidity');
  //showNotification(waterLevel, 'water level');

  return (
    <>
      <ToastContainer
        position='top-right'
        autoClose={5000}
        hideProgressBar={false}
        newestOnTop={false}
        closeOnClick
        rtl={false}
        pauseOnVisibilityChange
        draggable
        pauseOnHover
      />

      <div className='row'>
        <Widget icon='seedling' title='Plants' total={5} />
        <Widget
          icon='assistive-listening-systems'
          title='Sensors'
          bgColor='bg-info'
          total={3}
        />
        <Widget icon='users' title='Users' bgColor='bg-success' total={5} />
        <Widget
          icon='shopping-basket'
          title='Cultures'
          bgColor='bg-warning'
          total={1}
        />
      </div>

      <HydraCard title='Switch Station'>
        {status === null ? (
          <Spinner color='primary' />
        ) : (
          <ToggleStation
            changeStatusHandler={changeStatusOfStation}
            status={status}
          />
        )}
      </HydraCard>

      <HydraCard title='Water Level Sensors'>
        {metrics === null ? (
          <Spinner color='primary' />
        ) : (
          <div className='progress'>
            <div
              className={waterLevelColor}
              role='progressbar'
              style={{ width: `${metrics ? metrics.waterLevel : 0}%` }}
              aria-valuenow={metrics ? metrics.waterLevel : 0}
              aria-valuemin='0'
              aria-valuemax='100'
            >
              {metrics ? metrics.waterLevel : 0}%
            </div>
          </div>
        )}
      </HydraCard>

      <HydraCard title='Temperature Sensors'>
        {metrics === null ? (
          <Spinner color='primary' />
        ) : (
          <Bar
            data={{
              datasets: [
                {
                  label: 'temperature sensors',
                  data: metrics ? metrics.temperature : [],
                  backgroundColor: ['rgba(255, 159, 64, 0.5)'],
                  borderColor: ['rgba(255, 159, 64, 0.5)'],
                  borderWidth: 1,
                },
              ],
            }}
          />
        )}
      </HydraCard>

      <div className='row'>
        <div className='col-md-6 col-xs-12'>
          <HydraCard title='User List'>
            <Table header={userHeader}>
              {userRows.map(({ name, position, office, age }) => (
                <tr key={uuid()}>
                  <td>{name}</td>
                  <td>{position}</td>
                  <td>{office}</td>
                  <td>{age}</td>
                </tr>
              ))}
            </Table>
          </HydraCard>
        </div>

        <div className='col-md-6 col-xs-12'>
          <HydraCard title='Plants List'>
            <Table header={plantHeader}>
              {plantRows.map(({ name, description, type, quantity }) => (
                <tr key={uuid()}>
                  <td>{name}</td>
                  <td>description</td>
                  <td>{type}</td>
                  <td>{quantity}</td>
                </tr>
              ))}
            </Table>
          </HydraCard>
        </div>
      </div>

      <HydraCard title='Humidity Sensors'>
        <Polar
          data={{
            datasets: [
              {
                label: 'humidity sensors',
                data: metrics ? metrics.humidity : [],
                backgroundColor: ['rgba(54, 162, 235, 0.5)'],
                borderColor: ['rgba(54, 162, 235, 0.5)'],
                borderWidth: 1,
              },
            ],
          }}
        />
      </HydraCard>
    </>
  );
}
