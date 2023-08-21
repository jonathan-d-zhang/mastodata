ARG DEBIAN_VERSION=bullseye
ARG DEBIAN_VERSION_NUMBER=11
ARG PROJECT=sink
ARG RUST_VERSION=1.71

# ====================================================================================================
# Base
FROM rust:${RUST_VERSION}-${DEBIAN_VERSION} AS build-base
ARG PROJECT

RUN <<EOT
#!/usr/bin/env bash
set -e

apt -q update
apt -qy --no-install-recommends install libzmq3-dev
rm -rf /var/lib/apt/lists/*
EOT

WORKDIR /app
COPY Cargo.toml Cargo.toml
COPY Cargo.lock Cargo.lock

# ===================================================================================================
# Debug
FROM build-base AS build-debug
ARG PROJECT

RUN --mount=type=cache,id=cargo-registry,target=/usr/local/cargo/registry \
    --mount=type=cache,id=rust-target-debug,target=/app/target \
    <<EOT
#!/usr/bin/env bash
set -eu

mkdir src
echo 'fn main() {}' > src/main.rs
cargo build --locked
rm src/main.rs target/debug/deps/${PROJECT//-/_}*
EOT

COPY src src
RUN --mount=type=cache,id=cargo-registry,target=/usr/local/cargo/registry \
    --mount=type=cache,id=rust-target-debug,target=/app/target \
    cargo build --locked && cp /app/target/debug/${PROJECT} /app/${PROJECT}

# =================================================
#FROM gcr.io/distroless/cc-debian${DEBIAN_VERSION_NUMBER}:debug-nonroot AS debug
FROM debian:${DEBIAN_VERSION}-slim AS debug
ARG PROJECT

RUN <<EOT
#!/usr/bin/env bash
set -e

apt -q update
apt -qy --no-install-recommends install libzmq3-dev
rm -rf /var/lib/apt/lists/*
EOT

WORKDIR /app

COPY --from=build-debug /app/${PROJECT} ./${PROJECT}

ENTRYPOINT ["./sink"]

# ===================================================================================================
# Release
FROM build-base AS build-release
ARG PROJECT

RUN --mount=type=cache,id=cargo-registry,target=/usr/local/cargo/registry \
    --mount=type=cache,id=rust-target-release,target=/app/target \
    <<EOT
#!/usr/bin/env bash
set -eu

mkdir src
echo 'fn main() {}' > src/main.rs
cargo build --locked --release
rm src/main.rs target/release/deps/${PROJECT//-/_}*
EOT

COPY src src
RUN --mount=type=cache,id=cargo-registry,target=/usr/local/cargo/registry \
    --mount=type=cache,id=rust-target-release,target=/app/target \
    cargo build --locked --release && cp /app/target/release/${PROJECT} /app/${PROJECT}

# =================================================
#FROM gcr.io/distroless/cc-debian${DEBIAN_VERSION_NUMBER}:nonroot AS release
FROM debian:${DEBIAN_VERSION}-slim AS release
ARG PROJECT

RUN <<EOT
#!/usr/bin/env bash
set -e

apt -q update
apt -qy --no-install-recommends install libzmq3-dev
rm -rf /var/lib/apt/lists/*
EOT

WORKDIR /app

COPY --from=build-release /app/${PROJECT} ./${PROJECT}

ENTRYPOINT ["./sink"]
