from pathlib import Path
import sys

# add proto folder
proto_folder = Path(__file__).resolve().parent.parent / "proto"
sys.path.append(str(proto_folder))

import grpc
import geo_pb2_grpc
import geo_pb2

import logging
import os

LOGGER = logging.getLogger("main")
LOGGER.addHandler(logging.StreamHandler())
LOGGER.setLevel(logging.DEBUG)

class Client(object):
    """
    Client for gRPC functionality
    """

    def __init__(self):
        # instantiate a channel
        channel = grpc.insecure_channel(target=os.environ["SERVER_ADDRESS"])

        # Wait for the channel to be ready with a timeout
        ready_future = grpc.channel_ready_future(channel)
        ready_future.result(timeout=5)

        # bind the client and the server
        self.stub = geo_pb2_grpc.GeoStub(channel)

    def get_cities_by_name(self, name: str):
        request = geo_pb2.CitiesRequest(name=name)
        return self.stub.GetCities(request)

    def get_cities_by_position(self, latitude: float, longitude: float):
        request = geo_pb2.CitiesRequest(position=geo_pb2.Point(latitude=latitude, longitude=longitude))
        return self.stub.GetCities(request)

def main():
    client = Client()

    # GetCitiesByName
    request_params = {
        "name": "Moscow"
    }
    LOGGER.info(f"GetCitiesByName: {request_params}")
    response = client.get_cities_by_name(**request_params)
    LOGGER.info("Response:")
    LOGGER.info(response)

    # GetCitiesByPosition
    request_params = {
        "latitude": 55.991893,
        "longitude": 37.214390
    }
    LOGGER.info(f"GetCitiesByPosition: {request_params}")
    response = client.get_cities_by_position(**request_params)
    LOGGER.info("Response:")
    LOGGER.info(response)

if __name__ == '__main__':
    main()
