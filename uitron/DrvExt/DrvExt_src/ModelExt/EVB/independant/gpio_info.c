#include "modelext_info.h"
#include "gpio_info.h"
#include "IOCfg.h"

GPIO_INIT_OBJ ind_gpio_info_data[] __attribute__((section("modelext_data.gpio_info"))) = {
#if 0
	{  GPIO_LENS_RESET,        GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST   },
	{  GPIO_LENS_ZOOM_INTR,    GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_LENS_ZOOM_INTR  },
	{  GPIO_LENS_FOCUS_INTR,   GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_LENS_FOCUS_INTR },
	{  GPIO_LENS_IR_CTRL0,     GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST   },
	{  GPIO_LENS_IR_CTRL1,     GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST   },

	{  GPIO_KEY_LEFT,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_LEFT        },
	{  GPIO_KEY_ENTER,         GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_ENTER       },
	{  GPIO_KEY_UP,            GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_UP          },
	{  GPIO_KEY_RIGHT,         GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_RIGHT       },
	{  GPIO_KEY_PLAYBACK,      GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_PLAYBACK    },
	{  GPIO_KEY_DOWN,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_DOWN        },
	{  GPIO_KEY_MODE,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_MODE        },
#endif
	{  GPIO_KEY_MENU,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_MENU        },
	//coverity[undefined_identifier]
	{  GPIO_CARD2_DETECT,      GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_CARD2_DETECT   },
};

MODELEXT_HEADER ind_gpio_info_header __attribute__((section("modelext_header.gpio_info"))) = {
	.size = sizeof(ind_gpio_info_data) + sizeof(MODELEXT_HEADER),
	.type = MODELEXT_TYPE_GPIO_INFO,
	.number = sizeof(ind_gpio_info_data) / sizeof(GPIO_INIT_OBJ),
	.version = GPIO_INFO_VER,
};
