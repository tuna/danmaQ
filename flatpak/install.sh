flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
flatpak install flathub org.kde.Sdk//5.15
flatpak install flathub io.qt.qtwebengine.BaseApp//5.15
flatpak-builder --repo=repo --force-clean build-dir org.tuna.danmaQ.local.json
flatpak build-bundle repo danmaQ.flatpak org.tuna.danmaQ
