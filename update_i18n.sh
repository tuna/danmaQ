#!/bin/sh
export PATH=/usr/local/opt/qt/bin:$PATH
lupdate -recursive . -ts src/translations/zh_CN.ts
lupdate -recursive . -ts src/translations/zh_TW.ts
lupdate -recursive . -ts src/translations/ja_JP.ts
