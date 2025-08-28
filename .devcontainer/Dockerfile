FROM gcc:14.2.0

USER root
WORKDIR /root

RUN apt update; \
    apt install -y python3-pip python3.11-venv cmake gdb

RUN pip install conan --break-system-packages
