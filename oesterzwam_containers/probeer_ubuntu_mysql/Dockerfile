
FROM ubuntu:focal

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update -y && apt upgrade -y
RUN apt install -y wget lsb-release gnupg

# Add MySQL APT repository
RUN wget https://dev.mysql.com/get/mysql-apt-config_0.8.22-1_all.deb && \
    dpkg -i mysql-apt-config_0.8.22-1_all.deb && \
    rm mysql-apt-config_0.8.22-1_all.deb

RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys B7B3B788A8D3785C

# Update package list and install MySQL Workbench
RUN apt-get update


