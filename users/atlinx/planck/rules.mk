SRC += planck/atlinx_planck.c

UNICODE_ENABLE = yes
BOOTMAGIC_ENABLE = yes
MOUSEKEY_ENABLE = yes
TAP_DANCE_ENABLE = no

ifeq ($(strip $(RGB_MATRIX_ENABLE)),yes)
  GLOW_TYPE = glow
else
  GLOW_TYPE = base
endif

include $(USER_PATH)/planck/$(strip $(GLOW_TYPE))/rules.mk
