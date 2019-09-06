FROM archlinux/base as builder

RUN pacman -Sy --noconfirm archlinux-keyring
RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm xorg-server-xvfb xorg-fonts-misc ttf-bitstream-vera base-devel git x11-ssh-askpass
COPY PKGBUILD /tmp/build/
RUN chmod 777 /tmp/build
RUN echo "nobody ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
WORKDIR /tmp/build
RUN su nobody -s /bin/bash -c "makepkg -s --noconfirm"

FROM archlinux/base

COPY --from=builder /tmp/build/qqsp-1.9-1-x86_64.pkg.tar.xz /tmp/
RUN pacman -Sy --noconfirm archlinux-keyring
RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm xorg-server-xvfb xorg-fonts-misc ttf-bitstream-vera
RUN pacman --noconfirm -U /tmp/qqsp-1.9-1-x86_64.pkg.tar.xz
RUN rm /tmp/qqsp-1.9-1-x86_64.pkg.tar.xz
ENV QTWEBENGINE_DISABLE_SANDBOX=1
ENTRYPOINT ["/usr/bin/Qqsp"]
CMD []
