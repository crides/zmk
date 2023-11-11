#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include <zmk/hid.h>
#include <zmk/endpoints.h>
#include <zmk/keymap.h>

#define SCROLL_DIV_FACTOR 35

LOG_MODULE_REGISTER(mouse, 3);

const struct device *mouse = DEVICE_DT_GET(DT_INST(0, pixart_pmw3610));

static void handle_mouse(const struct device *dev, const struct sensor_trigger *trig) {
    int ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        LOG_ERR("fetch: %d", ret);
        return;
    }
    struct sensor_value dx, dy;//, btn;
    ret = sensor_channel_get(dev, SENSOR_CHAN_POS_DX, &dx);
    if (ret < 0) {
        LOG_ERR("get dx: %d", ret);
        return;
    }
    ret = sensor_channel_get(dev, SENSOR_CHAN_POS_DY, &dy);
    if (ret < 0) {
        LOG_ERR("get dy: %d", ret);
        return;
    }
    const int16_t x = -dy.val1, y = dx.val1;
    LOG_DBG("mouse %d %d", x, y);
    const uint8_t layer = zmk_keymap_highest_layer_active();
    /* static uint8_t last_layer = 0; */
    static int8_t scroll_ver_rem = 0, scroll_hor_rem = 0;
    /* static bool reached_zero = false; */
    /* bool update = true; */
    /* if (x == 0 && y == 0) { */
    /*     if (reached_zero) { */
    /*         update = false; */
    /*     } else { */
    /*         reached_zero = true; */
    /*     } */
    /* } */
    /* if (update) { */
        /* if (layer != last_layer) { */
        zmk_hid_mouse_movement_set(0, 0);
        zmk_hid_mouse_scroll_set(0, 0);
        /* } */
        if (layer == 2) {   // raise
            zmk_hid_mouse_movement_set(x, y);
        } else {
            const int16_t total_hor = x + scroll_hor_rem, total_ver = -y + scroll_ver_rem;
            scroll_hor_rem = total_hor % SCROLL_DIV_FACTOR;
            scroll_ver_rem = total_ver % SCROLL_DIV_FACTOR;
            zmk_hid_mouse_scroll_set(total_hor / SCROLL_DIV_FACTOR, total_ver / SCROLL_DIV_FACTOR);
        }
        zmk_endpoints_send_mouse_report();
    /* } */
    /* last_layer = layer; */
}

static int mouse_init() {
    struct sensor_trigger trigger = {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_ALL,
    };
    int ret = sensor_trigger_set(mouse, &trigger, handle_mouse);
    if (ret < 0) {
        LOG_ERR("can't set trigger: %d", ret);
        return -EIO;
    };
    return 0;
}

SYS_INIT(mouse_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
