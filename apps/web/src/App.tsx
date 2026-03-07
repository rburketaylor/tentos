import { useEffect, useState } from "react";

type Device = {
  id: string;
  name: string;
  status: "online" | "offline" | "degraded";
  temperature_c: number;
  humidity_rh: number;
};

type Camera = {
  id: string;
  name: string;
  status: "online" | "offline" | "degraded";
  stream_url: string;
  last_snapshot_url?: string | null;
};

type Alert = {
  id: string;
  level: "info" | "warning" | "critical";
  message: string;
};

type Schedule = {
  id: string;
  name: string;
  start_time: string;
  duration_seconds: number;
};

const API_BASE_URL = import.meta.env.VITE_API_BASE_URL ?? "/api/v1";

const fallbackData = {
  devices: [
    {
      id: "tent-controller-01",
      name: "Tent Controller",
      status: "online" as const,
      temperature_c: 24.3,
      humidity_rh: 59.1,
    },
  ],
  cameras: [
    {
      id: "pi-camera-01",
      name: "Tent Canopy Cam",
      status: "online" as const,
      stream_url: "http://pi-camera.local:8100/stream",
      last_snapshot_url: "http://pi-camera.local:8100/snapshots/latest.jpg",
    },
  ],
  alerts: [
    {
      id: "camera-signal-lag",
      level: "warning" as const,
      message: "Camera node reported delayed snapshot upload.",
    },
  ],
  schedules: [
    {
      id: "feed-veg-am",
      name: "Morning Feed",
      start_time: "08:00",
      duration_seconds: 18,
    },
  ],
};

async function fetchCollection<T>(path: string, fallback: T): Promise<T> {
  try {
    const response = await fetch(`${API_BASE_URL}/${path}`);
    if (!response.ok) {
      throw new Error(`Request failed: ${response.status}`);
    }
    return (await response.json()) as T;
  } catch {
    return fallback;
  }
}

function formatStatus(status: Device["status"] | Camera["status"]) {
  return status.charAt(0).toUpperCase() + status.slice(1);
}

export function App() {
  const [devices, setDevices] = useState<Device[]>(fallbackData.devices);
  const [cameras, setCameras] = useState<Camera[]>(fallbackData.cameras);
  const [alerts, setAlerts] = useState<Alert[]>(fallbackData.alerts);
  const [schedules, setSchedules] = useState<Schedule[]>(fallbackData.schedules);
  const [dataSource, setDataSource] = useState<"live" | "fallback">("fallback");

  useEffect(() => {
    async function load() {
      const [nextDevices, nextCameras, nextAlerts, nextSchedules] = await Promise.all([
        fetchCollection("devices", fallbackData.devices),
        fetchCollection("cameras", fallbackData.cameras),
        fetchCollection("alerts", fallbackData.alerts),
        fetchCollection("schedules", fallbackData.schedules),
      ]);

      setDevices(nextDevices);
      setCameras(nextCameras);
      setAlerts(nextAlerts);
      setSchedules(nextSchedules);

      const usingFallback = JSON.stringify(nextDevices) === JSON.stringify(fallbackData.devices);
      setDataSource(usingFallback ? "fallback" : "live");
    }

    void load();
  }, []);

  return (
    <main className="app-shell">
      <section className="hero">
        <div>
          <p className="eyebrow">Grow Control</p>
          <h1>Home server, ESP32, and Pi camera in one repo.</h1>
          <p className="hero-copy">
            This dashboard is intentionally lean: quick health visibility, a schedule anchor,
            and camera access without dragging in a heavy monitoring stack.
          </p>
        </div>
        <div className="hero-status">
          <span className={`badge badge-${dataSource}`}>{dataSource === "live" ? "Live API" : "Fallback Seed"}</span>
          <button type="button" className="action-button">
            Trigger Manual Watering
          </button>
        </div>
      </section>

      <section className="grid">
        <article className="panel">
          <div className="panel-header">
            <h2>Devices</h2>
            <span>{devices.length} node</span>
          </div>
          {devices.map((device) => (
            <div className="row-card" key={device.id}>
              <div>
                <strong>{device.name}</strong>
                <p>{device.id}</p>
              </div>
              <div>
                <span className={`status status-${device.status}`}>{formatStatus(device.status)}</span>
                <p>
                  {device.temperature_c} C / {device.humidity_rh}% RH
                </p>
              </div>
            </div>
          ))}
        </article>

        <article className="panel">
          <div className="panel-header">
            <h2>Cameras</h2>
            <span>{cameras.length} feed</span>
          </div>
          {cameras.map((camera) => (
            <div className="row-card" key={camera.id}>
              <div>
                <strong>{camera.name}</strong>
                <p>{camera.id}</p>
              </div>
              <div>
                <span className={`status status-${camera.status}`}>{formatStatus(camera.status)}</span>
                <a href={camera.stream_url} target="_blank" rel="noreferrer">
                  Open Stream
                </a>
              </div>
            </div>
          ))}
        </article>

        <article className="panel">
          <div className="panel-header">
            <h2>Schedules</h2>
            <span>{schedules.length} active</span>
          </div>
          {schedules.map((schedule) => (
            <div className="row-card" key={schedule.id}>
              <div>
                <strong>{schedule.name}</strong>
                <p>{schedule.id}</p>
              </div>
              <div>
                <p>{schedule.start_time}</p>
                <p>{schedule.duration_seconds}s pulse</p>
              </div>
            </div>
          ))}
        </article>

        <article className="panel panel-alerts">
          <div className="panel-header">
            <h2>Alerts</h2>
            <span>{alerts.length} open</span>
          </div>
          {alerts.map((alert) => (
            <div className="alert-card" key={alert.id}>
              <span className={`pill pill-${alert.level}`}>{alert.level}</span>
              <p>{alert.message}</p>
            </div>
          ))}
        </article>
      </section>
    </main>
  );
}
