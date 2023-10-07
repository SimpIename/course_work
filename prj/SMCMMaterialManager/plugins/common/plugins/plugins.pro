TEMPLATE = subdirs

SUBDIRS += \
           ../../plugins/template/template.pro \
#           ../../plugins/Elast_mod/Elast_mod.pro \
#           ../../plugins/Density/Density.pro \
           ../../plugins/Plasticity/Plasticity.pro \
           ../../plugins/Density_new/Density2.pro \
           ../../plugins/Elast_mod_new/Elast_mod2.pro \
            ../../base/base.pro
DEFINES += QT_NO_DEBUG_OUTPUT

