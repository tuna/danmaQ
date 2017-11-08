#!/bin/sh
export PATH=/usr/local/opt/qt/bin:$PATH
lupdate -recursive . -ts src/translations/i18n_zh.ts
lupdate -recursive . -ts src/translations/i18n_ja.ts
