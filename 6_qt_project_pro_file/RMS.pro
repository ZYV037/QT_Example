TEMPLATE = subdirs

SUBDIRS += \
    Inc     \
    Src     \
    App

Inc.subdir = Inc
Src.subdir = Src
App.subdir = App

App.depends = Inc Src
