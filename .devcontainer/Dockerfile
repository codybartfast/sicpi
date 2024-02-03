FROM debian:stable
ARG user=alyssa
ARG userHome=/home/$user
RUN echo "root:apply" | chpasswd
RUN apt update
RUN apt install -y sudo vim git build-essential
RUN apt install -y python3
RUN useradd -m -s /bin/bash --uid 25441 $user
RUN echo "$user:eval" | chpasswd
RUN sudo usermod -a -G sudo $user
USER $user
RUN echo set nocompatible > ${userHome}/.exrc
# RUN git clone -b jd-dev --recurse-submodules https://github.com/codybartfast/sicpi.git ${userHome}/sicp
RUN /bin/bash -c "${userHome}/sicp/wasm/emsdk/emsdk install latest"
RUN /bin/bash -c "${userHome}/sicp/wasm/emsdk/emsdk activate latest"
# personal
COPY autoexec.bat ${userHome}/bin/autoexec.bat
RUN echo source ${userHome}/bin/autoexec.bat >> ${userHome}/.bashrc
RUN git clone https://github.com/lyze/posh-git-sh.git ${userHome}/posh-git-sh
