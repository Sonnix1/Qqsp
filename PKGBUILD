pkgname=qqsp
_pkgname=Qqsp
pkgver=1.9
pkgrel=1
pkgdesc="Qt Quest Soft Player"
arch=(i686 x86_64)
url="http://git.tfgamessite.com/Sonnix/Qqsp"
license=('MIT')
depends=('qt5-base' 'qt5-webengine' 'qt5-multimedia' 'gst-plugins-good' 'gst-plugins-ugly')
makedepends=('git' 'librsvg')
source=("${_pkgname}::git+https://gitlab.com/Sonnix1/Qqsp.git#tag=v$pkgver")
sha256sums=('SKIP')

prepare() {
  install -dm755 "$srcdir/hicolor/scalable/apps/"
  install -m644 "${_pkgname}/icons/qsp-logo-vector.svg" "$srcdir/hicolor/scalable/apps/qsp.svg"
  for i in 16x16 24x24 32x32 48x48 64x64 96x96 128x128 256x256 512x512; do
    msg2 "Generating icon: $i raster from vector..."
    install -dm755 "$srcdir/hicolor/$i/apps/"
    rsvg-convert -w "${i%%x*}" -h "${i##*x}" -f png \
        -o "$srcdir/hicolor/$i/apps/qsp.png" "$srcdir/hicolor/scalable/apps/qsp.svg"
  done
  cd "${_pkgname}"
  mkdir -p build
}

build() {
  cd "${_pkgname}/build"
  qmake-qt5 .. PREFIX=/usr
  make
}

package() {
  install -dm0755 "$pkgdir"/usr/{bin,share/{applications,icons,licenses/qqsp,mime/packages}}/

  install -m0755 "$_pkgname/build/Qqsp" "$pkgdir/usr/bin/"
  install -m0644 "$_pkgname/LICENSE" "$pkgdir/usr/share/licenses/qqsp/"

  install -m0644 "$_pkgname/Qqsp.desktop" "$pkgdir/usr/share/applications/"
  install -m0644 "$_pkgname/qsp.mime" "$pkgdir/usr/share/mime/packages/qsp.xml"
  
  bsdtar -cf - "hicolor" | bsdtar -xf - -C "$pkgdir/usr/share/icons"
}
