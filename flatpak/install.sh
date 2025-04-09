flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
flatpak install flathub org.kde.Sdk//6.8
flatpak install flathub io.qt.qtwebengine.BaseApp//6.8
flatpak-builder --repo=repo --force-clean build-dir org.tuna.danmaQ.local.json
flatpak build-bundle repo danmaQ.flatpak org.tuna.danmaQ
