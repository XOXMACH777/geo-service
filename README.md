# Geo Service

Geo Service is an example of a **gRPC-based microservice** developed for educational purposes to demonstrate backend development skills in C++.
It provides functionality for searching cities and regions based on various criteria, leveraging geographical data and APIs.

## Features

The Geo Service offers the following capabilities:

1. **City Search**:
   - Search for cities by name (e.g., "New York").
   - Search for cities near a specific geographical point (latitude/longitude).
   - Retrieve detailed information about cities, including their names, countries, and geographical features.

2. **Region Search**:
   - Search for regions within a square box defined by a central point and a distance in kilometers.
   - Filter regions by specific geographical features, such as international airports, mountain peaks, sea beaches, or salt lakes.
   - Stream regions as they are found, enabling real-time results.

3. **Geographical Data**:
   - Retrieve metadata about geographical entities, including their names, countries, and tagged features (e.g., airports, peaks).
   - Access detailed information about geographical features, such as their positions and associated metadata tags.

TODO Extend with weather.

## Protobuf API

The service is built on a **gRPC API** defined using Protocol Buffers (Protobuf). Key messages and methods include:

- **Point**: Represents a geographical coordinate (latitude/longitude).
- **Place**: Represents a geographical entity (e.g., city or region) with metadata and tagged features.
- **CitiesRequest/CitiesResponse**: Used to search for cities and retrieve results.
- **RegionsRequest/RegionsResponse**: Used to search for regions and stream results.
- **Geo Service**: Provides two main methods:
  - `GetCities`: Returns a list of cities based on search criteria.
  - `GetRegionsStream`: Streams regions within a specified area.

TODO Extend with weather.

## Data Sources

The Geo Service uses two external APIs to collect geographical data:

1. **Nominatim API**:
   - A search engine for OpenStreetMap data, used to resolve place names and retrieve metadata.
   - Learn more: [Nominatim API Documentation](https://nominatim.org/release-docs/latest/api/Overview/).

2. **Overpass API**:
   - A read-only API for querying OpenStreetMap data using a specialized query language called **Overpass QL**.
   - Overpass QL allows for complex queries to filter and retrieve geographical features (e.g., airports, peaks, beaches).
   - The service uses the **VK Maps Overpass API instance** by default, which is one of the fastest publicly available endpoints.
   - Overpass queries can also be tested and visualized using **Overpass Turbo**, a web-based interface for Overpass QL.
   - Try it out: [VK Maps Overpass Turbo](https://maps.mail.ru/osm/tools/overpass/).
   - **VK Maps Overpass instance** is fast, but is sometimes unstable. See list of other endpoints [here](https://wiki.openstreetmap.org/wiki/Overpass_API).

3. **Open Meteo API**
   - A read-only API for querying historical weather at specific location.

All endpoints are configured in geo-config.json.

## Overpass API Overview

- **Overpass API**: A powerful tool for querying OpenStreetMap data.
It allows users to extract specific geographical features (e.g., roads, buildings, natural landmarks) using Overpass QL.
- **Overpass QL**: A query language designed for Overpass API.
It enables users to write precise queries to filter and retrieve OpenStreetMap data.
- **Overpass Turbo**: A web-based interface for testing and visualizing Overpass QL queries.
It provides an interactive map to explore query results in real-time.


## Development

It is proposed to use WSL for Geo Service development.
However, it is not obligatory, and any other approach can be chosen.

### Prepare WSL

1. Enable WSL on your system.
2. Create a WSL VM with Ubuntu. On the host, run these commands:
```
> wsl --install -d Ubuntu-22.04
> wsl --update
> wsl --shutdown
```
3. Install Docker Desktop (if Docker Desktop is unavailable, use Rancher Desktop).
4. Enable Ubuntu WSL integration in Docker/Rancher Desktop settings.
5. Start WSL. On the host, run:
```
> wsl -d Ubuntu-22.04
```
6. Run these commands inside WSL:
```
$ cd ~
$ mkdir –p geo/.conan2
$ mkdir –p geo/code
$ cd geo/code
$ git clone https://github.com/cqginternship/geo-start geo
$ cd geo
```

For detailed setup instructions, refer to the [WSL Containers Tutorial](https://learn.microsoft.com/en-us/windows/wsl/tutorials/wsl-containers).

---

### Work with code from WSL

1. Install VSCode with WSL and Dev Containers extensions on the host.
2. Start WSL and run these commands:
```
$ cd ~/geo/code/geo
$ code .
```

In VSCode:

1. VSCode will start in WSL mode. To develop, switch to "Dev Container" mode.
   Press Ctrl+Shift+P and select "Dev Containers: Reopen in Container".
   Note that the first time this may take a few minutes.
2. In the CMake menu, select Configure and then choose the "conan-debug" configuration.
3. In the CMake menu, select Build.
4. Use the Run and Debug menu to debug the service.

Working with Python code:

1. Press Ctrl+Shift+P and select "Tasks: Run Task". Run the following tasks in sequence:
2. "Python: create venv for tests"
3. "Python: install requirements for tests" (NOTE: You will need to run this command every time you change the content of the `tests/requirements.txt` file)
4. After building and running the geo service, in order to run the tests, run the "Python: run tests" task.

---

## Deployment

Geo Service can be built and launched from a scratch on any environment where Docker is installed.

Run:

```
$ docker compose up --build
```

Note that the Dockerfile has four stages: Build Dependencies, Build Service, Start Service, Start Service Tests.
The first stage collects dependencies and may take several minutes to complete.
This stage is executed only once unless project dependencies change.

**Important**: If VSCode is running in Dev Container mode, docker-compose will fail to run the service because it attempts to use the same port.
Close VSCode or switch it back to WSL mode before running docker-compose.

---

## Sample Coordinates for Testing (Latitude/Longitude)

- Guatemala: 14.594582, -90.517661
- Zelenograd: 55.991893, 37.214390
- Toledo: 39.858014, -4.029030
- Pyongyang: 39.019368, 125.754257
- Phnom Penh: 11.552898, 104.865913
- Cairo: 30.050755, 31.246909
- Kolkata: 22.563887, 88.345477
- Kiev: 50.450441, 30.523550
- Denver: 39.739253, -104.989117
- Tarragona: 41.116525, 1.257839
- Yerevan: 40.1777112, 44.5126233
- Aral Sea: 45.20842335, 58.52356612752623
