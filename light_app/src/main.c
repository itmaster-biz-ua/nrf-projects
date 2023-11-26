#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
    !DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
    ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

#define PWM_MAX 253
#define TIMER_INTERVAL_MSEC 200
#define BUFFER_SIZE 1
#define ADC_MAX 1023

static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led1));
/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
                DT_SPEC_AND_COMMA)
};
struct k_timer my_timer;
static int16_t m_sample_buffer[BUFFER_SIZE];

static int adc_sample(void)
{
    int status;
    struct adc_sequence sequence = {
        .buffer = &m_sample_buffer,
        /* buffer size in bytes, not number of samples */
        .buffer_size = sizeof(m_sample_buffer),
    };

    (void)adc_sequence_init_dt(&adc_channels[0], &sequence);

    status = adc_read(adc_channels[0].dev, &sequence);
    if (status < 0) {
        printk("Error %d: failed to read ADC\n", status);
    }

    return status;
}

void adc_sample_event(struct k_timer *timer_id){
    int status;
    int adc_value;

    // Read ADC value
    status = adc_sample();
    if (status) {
        printk("adc_read() failed with code %d\n", status);
        return;
    }

    // Check the ranges
    if (m_sample_buffer[0] < 0) {
        adc_value = 0;
    }
    else {
        adc_value = m_sample_buffer[0]; 
    }
    printk("ADC value %d\n", adc_value);

    // Calculate the PWM width
    uint32_t pwm_val = (uint32_t)(((float)PWM_MAX/ADC_MAX)*adc_value);
    if (pwm_val > PWM_MAX) {
        printk("Error: failed to set pulse width %d\n", pwm_val);
        return;
    }

    // Set the PWM width
    printk("Setting pulse to %d\n", pwm_val);
    status = pwm_set_dt(&pwm_led, PWM_MAX, pwm_val);
    if (status) {
        printk("Error %d: failed to set pulse width\n", status);
    }
}

void main(void)
{
    printk("PWM Application has started!\r\n");
    int status;

    // ADC setup
    if (!device_is_ready(adc_channels[0].dev)) {
        printk("Error %d: failed to ADC\n", status);
        return;
    }

    status = adc_channel_setup_dt(&adc_channels[0]);
    if (status < 0) {
        printk("Error %d: failed to ADC\n", status);
        return;
    }

    // Timer setup
    k_timer_init(&my_timer, adc_sample_event, NULL);
    k_timer_start(&my_timer, K_MSEC(TIMER_INTERVAL_MSEC), K_MSEC(TIMER_INTERVAL_MSEC));
}
