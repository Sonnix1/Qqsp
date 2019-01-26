FROM base/archlinux

COPY qqsp-1.8-1-x86_64.pkg.tar.xz /tmp/
RUN pacman -Sy --noconfirm archlinux-keyring
RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm xorg-server-xvfb xorg-fonts-misc ttf-bitstream-vera 
RUN pacman --noconfirm -U /tmp/qqsp-1.8-1-x86_64.pkg.tar.xz
RUN rm /tmp/qqsp-1.8-1-x86_64.pkg.tar.xz
ENV QTWEBENGINE_DISABLE_SANDBOX=1
CMD ["/usr/bin/Qqsp"]
