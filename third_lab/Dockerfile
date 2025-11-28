FROM fedora:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN dnf -y update && \
    dnf -y install gcc gcc-c++ make && \
    dnf clean all

WORKDIR /app
COPY . /app

CMD ["/bin/bash"]