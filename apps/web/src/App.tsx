import { useEffect, useState } from "react";

import {
  emptyDashboardData,
  loadDashboardData,
  type Alert,
  type Camera,
  type DataSource,
  type Device,
} from "./dashboardData";

function formatStatus(status: Device["status"] | Camera["status"]) {
  return status.charAt(0).toUpperCase() + status.slice(1);
}

function formatLabel(value: string) {
  return value.charAt(0).toUpperCase() + value.slice(1);
}

function formatDuration(durationSeconds: number) {
  if (durationSeconds < 60) {
    return `${durationSeconds}s pulse`;
  }

  const minutes = Math.round(durationSeconds / 60);
  return `${minutes}m run`;
}

function alertPriority(level: Alert["level"]) {
  switch (level) {
    case "critical":
      return 3;
    case "warning":
      return 2;
    default:
      return 1;
  }
}

export function App() {
  const [dashboardData, setDashboardData] = useState(emptyDashboardData);
  const [dataSource, setDataSource] = useState<DataSource>("loading");
  const [dataError, setDataError] = useState<string | null>(null);

  useEffect(() => {
    async function load() {
      const result = await loadDashboardData();
      setDashboardData(result.data);
      setDataSource(result.source);
      setDataError(result.error ?? null);
    }

    void load();
  }, []);

  const { devices, cameras, alerts, schedules } = dashboardData;
  const primaryAlert = [...alerts].sort((left, right) => alertPriority(right.level) - alertPriority(left.level))[0];
  const nextSchedule = schedules[0];
  const primaryCamera = cameras[0];
  const connectedNodes = [...devices, ...cameras].filter((item) => item.status === "online").length;
  const systemTone =
    primaryAlert?.level === "critical"
      ? "Intervention required"
      : primaryAlert?.level === "warning"
        ? "Monitor closely"
        : "Stable canopy";
  const systemMessage = primaryAlert
    ? primaryAlert.message
    : dataSource === "error"
      ? dataError ?? "Live dashboard data could not be loaded."
    : "No active alerts. Sensors, schedules, and cameras are reporting normally.";
  const sourceLabel =
    dataSource === "live"
      ? "Live API"
      : dataSource === "seeded"
        ? "Seeded Mode"
        : dataSource === "error"
          ? "API Error"
          : "Loading";
  const sourceMessage =
    dataSource === "live"
      ? "Validated backend responses are flowing into the dashboard."
      : dataSource === "seeded"
        ? "Shared repo fixtures are in use because VITE_USE_SEEDED_DATA=true."
        : dataSource === "error"
          ? `Live API data could not be loaded, and the dashboard is not masking that failure with seed data.${dataError ? ` Details: ${dataError}` : ""}`
          : "Loading dashboard data from the API.";

  return (
    <main className="app-shell">
      <section className="command-deck">
        <div className="command-copy">
          <p className="eyebrow">Grow Control</p>
          <h1>Tent operations at a glance.</h1>
          <p className="hero-copy">
            Put the next decision above the fold: surface system health, watering timing, and
            camera access before the operator has to scan through rows.
          </p>

          <div className="source-strip">
            <span className={`badge badge-${dataSource}`}>{sourceLabel}</span>
            <p>{sourceMessage}</p>
          </div>
        </div>

        <div className="command-side">
          <div className="summary-grid">
            <article className="summary-card">
              <span className="summary-label">Connected Nodes</span>
              <strong>{connectedNodes}</strong>
              <p>{devices.length} controller + {cameras.length} camera feed</p>
            </article>
            <article className="summary-card">
              <span className="summary-label">Attention</span>
              <strong>{alerts.length}</strong>
              <p>{primaryAlert ? `${formatLabel(primaryAlert.level)} alert open` : "All clear"}</p>
            </article>
            <article className="summary-card">
              <span className="summary-label">Next Irrigation</span>
              <strong>{nextSchedule?.start_time ?? "Unscheduled"}</strong>
              <p>{nextSchedule ? formatDuration(nextSchedule.duration_seconds) : "No active run"}</p>
            </article>
            <article className="summary-card">
              <span className="summary-label">Camera Access</span>
              <strong>{primaryCamera ? formatStatus(primaryCamera.status) : "Offline"}</strong>
              <p>{primaryCamera ? primaryCamera.name : "No camera discovered"}</p>
            </article>
          </div>

          <div className="action-cluster">
            <button type="button" className="action-button">
              Trigger Manual Watering
            </button>
            {primaryCamera?.stream_url ? (
              <a className="secondary-action" href={primaryCamera.stream_url} target="_blank" rel="noreferrer">
                Open Camera Stream
              </a>
            ) : null}
          </div>
        </div>
      </section>

      <section className="feature-grid">
        <article className="feature-panel priority-panel">
          <div className="feature-header">
            <p className="panel-kicker">Priority</p>
            <span className={`pill pill-${primaryAlert?.level ?? "info"}`}>
              {primaryAlert?.level ?? "info"}
            </span>
          </div>
          <h2>{systemTone}</h2>
          <p className="feature-copy">{systemMessage}</p>
          <div className="priority-footer">
            <div>
              <span className="metric-caption">Next schedule</span>
              <strong>{nextSchedule?.name ?? "No schedule loaded"}</strong>
              <p>{nextSchedule ? `${nextSchedule.start_time} · ${formatDuration(nextSchedule.duration_seconds)}` : "Review irrigation automation."}</p>
            </div>
            <div>
              <span className="metric-caption">Data source</span>
              <strong>{sourceLabel}</strong>
              <p>
                {dataSource === "live"
                  ? "Using validated backend responses."
                  : dataSource === "seeded"
                    ? "Using shared repo fixtures by explicit opt-in."
                    : dataSource === "error"
                      ? "Partial or missing live data is being surfaced honestly."
                      : "Waiting for the API response."}
              </p>
            </div>
          </div>
        </article>

        <article className="feature-panel camera-panel">
          <div className="feature-header">
            <p className="panel-kicker">Cameras</p>
            <span className={`status status-${primaryCamera?.status ?? "offline"}`}>
              {primaryCamera ? formatStatus(primaryCamera.status) : "Offline"}
            </span>
          </div>
          <div className="camera-title-block">
            <h2>Cameras</h2>
            <p>{primaryCamera?.name ?? "Camera feed unavailable"}</p>
          </div>
          <div className="camera-frame">
            <div className="camera-overlay" />
            <div className="camera-copy">
              <span className="camera-chip">Canopy View</span>
              <p>
                {primaryCamera
                  ? "Launch the Pi camera stream when you need a visual confirmation before watering or after an alert."
                  : "Bring a Pi camera online to unlock visual verification from the dashboard."}
              </p>
            </div>
          </div>
          {primaryCamera?.stream_url ? (
            <a className="camera-link" href={primaryCamera.stream_url} target="_blank" rel="noreferrer">
              Open Stream
            </a>
          ) : null}
        </article>
      </section>

      <section className="operations-grid">
        <article className="panel">
          <div className="panel-header">
            <h2>Devices</h2>
            <span>{devices.length} node</span>
          </div>
          {devices.length === 0 ? (
            <p className="panel-empty">No device data loaded.</p>
          ) : (
            devices.map((device) => (
              <div className="row-card" key={device.id}>
                <div>
                  <strong>{device.name}</strong>
                  <p>{device.id}</p>
                </div>
                <div className="row-metrics">
                  <span className={`status status-${device.status}`}>{formatStatus(device.status)}</span>
                  <p>{device.temperature_c} C</p>
                  <p>{device.humidity_rh}% RH</p>
                </div>
              </div>
            ))
          )}
        </article>

        <article className="panel">
          <div className="panel-header">
            <h2>Schedules</h2>
            <span>{schedules.length} active</span>
          </div>
          {schedules.length === 0 ? (
            <p className="panel-empty">No schedule data loaded.</p>
          ) : (
            schedules.map((schedule) => (
              <div className="row-card" key={schedule.id}>
                <div>
                  <strong>{schedule.name}</strong>
                  <p>{schedule.id}</p>
                </div>
                <div className="row-metrics">
                  <p>{schedule.start_time}</p>
                  <p>{formatDuration(schedule.duration_seconds)}</p>
                </div>
              </div>
            ))
          )}
        </article>

        <article className="panel panel-wide">
          <div className="panel-header">
            <h2>Alerts</h2>
            <span>{alerts.length} open</span>
          </div>
          {alerts.length === 0 ? (
            <p className="panel-empty">No alert data loaded.</p>
          ) : (
            <div className="alert-stack">
              {alerts.map((alert) => (
                <div className="alert-card" key={alert.id}>
                  <span className={`pill pill-${alert.level}`}>{alert.level}</span>
                  <p>{alert.message}</p>
                </div>
              ))}
            </div>
          )}
        </article>
      </section>
    </main>
  );
}
