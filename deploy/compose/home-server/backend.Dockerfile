FROM python:3.12-slim

WORKDIR /app

COPY apps/backend/pyproject.toml apps/backend/pyproject.toml
COPY apps/backend/src apps/backend/src
COPY shared/fixtures shared/fixtures

RUN pip install --no-cache-dir -e /app/apps/backend

CMD ["grow-backend"]
