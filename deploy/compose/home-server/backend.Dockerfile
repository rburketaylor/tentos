FROM python:3.12-slim

WORKDIR /app

COPY apps/backend/pyproject.toml apps/backend/pyproject.toml
COPY apps/backend/src apps/backend/src

RUN pip install --no-cache-dir -e /app/apps/backend

CMD ["uvicorn", "grow_backend.main:app", "--app-dir", "apps/backend/src", "--host", "0.0.0.0", "--port", "8000"]
