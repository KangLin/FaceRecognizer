#!/bin/bash

# Use to install appimage in linux
# $1: install or remove
# $2: run

case "$1" in
    remove)
        echo "remove ..."
        rm -f /usr/share/applications/FaceRecognizer.desktop
        rm -f ~/.config/autostart/FaceRecognizer.desktop
        rm -f /usr/share/pixmaps/FaceRecognizer.png
    ;;

    install|*)
        echo "install ..."
        # Install destop
        if [ -f /usr/share/applications/FaceRecognizer.desktop ]; then
            rm /usr/share/applications/FaceRecognizer.desktop
        fi
        ln -s `pwd`/share/applications/FaceRecognizer.desktop /usr/share/applications/FaceRecognizer.desktop

        # Install auto run on boot
        if [ ! -d ~/.config/autostart ]; then
            mkdir -p ~/.config/autostart
            chmod a+wr ~/.config/autostart
        fi
        if [ -f ~/.config/autostart/FaceRecognizer.desktop ]; then
            rm ~/.config/autostart/FaceRecognizer.desktop
        fi
        ln -s `pwd`/share/applications/FaceRecognizer.desktop ~/.config/autostart/FaceRecognizer.desktop

        # Reset exec to desktop
        sed -i "s/Exec=.*//g" `pwd`/share/applications/FaceRecognizer.desktop
        echo "Exec=`pwd`/FaceRecognizer-x86_64.AppImage" >> `pwd`/share/applications/FaceRecognizer.desktop

        # Install applications icon
        if [ -f /usr/share/pixmaps/FaceRecognizer.png ]; then
            rm /usr/share/pixmaps/FaceRecognizer.png
        fi
        if [ ! -d /usr/share/pixmaps ]; then
            mkdir -p /usr/share/pixmaps
        fi
        ln -s `pwd`/share/pixmaps/FaceRecognizer.png /usr/share/pixmaps/FaceRecognizer.png
        
        # Whether run after install
        if [ "$2" = "run" ]; then
            ./FaceRecognizer-x86_64.AppImage
        fi
        ;;
esac
