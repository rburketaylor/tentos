import alertsFixture from "../../../shared/fixtures/alerts.json";
import camerasFixture from "../../../shared/fixtures/cameras.json";
import devicesFixture from "../../../shared/fixtures/devices.json";
import schedulesFixture from "../../../shared/fixtures/schedules.json";

export type Status = "online" | "offline" | "degraded";
export type AlertLevel = "info" | "warning" | "critical";
export type DataSource = "loading" | "live" | "seeded" | "error";

export type Device = {
  id: string;
  name: string;
  status: Status;
  temperature_c: number;
  humidity_rh: number;
  moisture_pct?: number | null;
  active_schedule_id?: string | null;
  last_seen: string;
};

export type Camera = {
  id: string;
  name: string;
  status: Status;
  stream_url: string;
  last_snapshot_url?: string | null;
  last_seen: string;
};

export type Alert = {
  id: string;
  source: string;
  level: AlertLevel;
  message: string;
  active: boolean;
  created_at: string;
};

export type Schedule = {
  id: string;
  device_id: string;
  name: string;
  enabled: boolean;
  start_time: string;
  duration_seconds: number;
  description: string;
};

export type DashboardData = {
  devices: Device[];
  cameras: Camera[];
  alerts: Alert[];
  schedules: Schedule[];
};

type DashboardLoadResult = {
  data: DashboardData;
  source: Exclude<DataSource, "loading">;
  error?: string;
};

type Validator<T> = (value: unknown) => value is T;

const API_BASE_URL = import.meta.env.VITE_API_BASE_URL ?? "/api/v1";
const USE_SEEDED_DATA = import.meta.env.VITE_USE_SEEDED_DATA === "true";

export const emptyDashboardData: DashboardData = {
  devices: [],
  cameras: [],
  alerts: [],
  schedules: [],
};

function isRecord(value: unknown): value is Record<string, unknown> {
  return typeof value === "object" && value !== null;
}

function isStatus(value: unknown): value is Status {
  return value === "online" || value === "offline" || value === "degraded";
}

function isAlertLevel(value: unknown): value is AlertLevel {
  return value === "info" || value === "warning" || value === "critical";
}

function isOptionalString(value: unknown): value is string | null | undefined {
  return value === undefined || value === null || typeof value === "string";
}

function isOptionalNumber(value: unknown): value is number | null | undefined {
  return value === undefined || value === null || typeof value === "number";
}

function isDevice(value: unknown): value is Device {
  return (
    isRecord(value) &&
    typeof value.id === "string" &&
    typeof value.name === "string" &&
    isStatus(value.status) &&
    typeof value.temperature_c === "number" &&
    typeof value.humidity_rh === "number" &&
    isOptionalNumber(value.moisture_pct) &&
    isOptionalString(value.active_schedule_id) &&
    typeof value.last_seen === "string"
  );
}

function isCamera(value: unknown): value is Camera {
  return (
    isRecord(value) &&
    typeof value.id === "string" &&
    typeof value.name === "string" &&
    isStatus(value.status) &&
    typeof value.stream_url === "string" &&
    isOptionalString(value.last_snapshot_url) &&
    typeof value.last_seen === "string"
  );
}

function isAlert(value: unknown): value is Alert {
  return (
    isRecord(value) &&
    typeof value.id === "string" &&
    typeof value.source === "string" &&
    isAlertLevel(value.level) &&
    typeof value.message === "string" &&
    typeof value.active === "boolean" &&
    typeof value.created_at === "string"
  );
}

function isSchedule(value: unknown): value is Schedule {
  return (
    isRecord(value) &&
    typeof value.id === "string" &&
    typeof value.device_id === "string" &&
    typeof value.name === "string" &&
    typeof value.enabled === "boolean" &&
    typeof value.start_time === "string" &&
    typeof value.duration_seconds === "number" &&
    typeof value.description === "string"
  );
}

function validateCollection<T>(name: string, payload: unknown, validator: Validator<T>): T[] {
  if (!Array.isArray(payload) || payload.some((item) => !validator(item))) {
    throw new Error(`Invalid ${name} response payload`);
  }

  return payload;
}

function cloneDashboardData(data: DashboardData): DashboardData {
  return {
    devices: [...data.devices],
    cameras: [...data.cameras],
    alerts: [...data.alerts],
    schedules: [...data.schedules],
  };
}

const seededDashboardData: DashboardData = {
  devices: validateCollection("shared fixtures devices", devicesFixture, isDevice),
  cameras: validateCollection("shared fixtures cameras", camerasFixture, isCamera),
  alerts: validateCollection("shared fixtures alerts", alertsFixture, isAlert),
  schedules: validateCollection("shared fixtures schedules", schedulesFixture, isSchedule),
};

async function fetchCollection<T>(
  path: string,
  validator: Validator<T>,
): Promise<T[]> {
  const response = await fetch(`${API_BASE_URL}/${path}`);
  if (!response.ok) {
    throw new Error(`${path} request failed with status ${response.status}`);
  }

  return validateCollection(path, await response.json(), validator);
}

export async function loadDashboardData(): Promise<DashboardLoadResult> {
  if (USE_SEEDED_DATA) {
    return {
      data: cloneDashboardData(seededDashboardData),
      source: "seeded",
    };
  }

  const result = cloneDashboardData(emptyDashboardData);
  const failures: string[] = [];

  const [devices, cameras, alerts, schedules] = await Promise.allSettled([
    fetchCollection("devices", isDevice),
    fetchCollection("cameras", isCamera),
    fetchCollection("alerts", isAlert),
    fetchCollection("schedules", isSchedule),
  ]);

  if (devices.status === "fulfilled") {
    result.devices = devices.value;
  } else {
    failures.push(devices.reason instanceof Error ? devices.reason.message : "devices request failed");
  }

  if (cameras.status === "fulfilled") {
    result.cameras = cameras.value;
  } else {
    failures.push(cameras.reason instanceof Error ? cameras.reason.message : "cameras request failed");
  }

  if (alerts.status === "fulfilled") {
    result.alerts = alerts.value;
  } else {
    failures.push(alerts.reason instanceof Error ? alerts.reason.message : "alerts request failed");
  }

  if (schedules.status === "fulfilled") {
    result.schedules = schedules.value;
  } else {
    failures.push(schedules.reason instanceof Error ? schedules.reason.message : "schedules request failed");
  }

  if (failures.length > 0) {
    return {
      data: result,
      source: "error",
      error: failures.join(" | "),
    };
  }

  return {
    data: result,
    source: "live",
  };
}
