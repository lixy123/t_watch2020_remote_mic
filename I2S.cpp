#include "I2S.h"

//https://github.com/RafigRzayev/Audio_Recording_to_WAV/blob/main/pdm_mic/pdm_mic.ino

//https://esp32.com/viewtopic.php?t=6062
void I2S_Init(i2s_mode_t MODE, int SAMPLE_RATE, i2s_bits_per_sample_t BPS) {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | MODE | I2S_MODE_PDM),  //LILYGO_WATCH_2020_V3 要加上I2S_MODE_PDM
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = BPS,   
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,  //左声道
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1 ,  // default interrupt priority
    .dma_buf_count = 2,
    .dma_buf_len = 128 
  };
  i2s_pin_config_t pin_config;

  pin_config.bck_io_num = TWATCH_DAC_IIS_BCK;  //必须设置
  pin_config.ws_io_num = MIC_CLOCK;
  pin_config.data_out_num = TWATCH_DAC_IIS_DOUT;   //必须设置
  pin_config.data_in_num   = MIC_DATA     ;


  /*
    if (MODE == I2S_MODE_RX) {
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num = MIC_DATA;
    }
    else if (MODE == I2S_MODE_TX) {
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num = I2S_PIN_NO_CHANGE;
    }
  */
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  //最终设置: 16k, 16位，单声道
  //下句不要有，否则录音初始化异常
 //i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, BPS, I2S_CHANNEL_MONO);
}


int I2S_Read(char* data, int numData) {
  return i2s_read_bytes(I2S_NUM_0, (char *)data, numData, portMAX_DELAY);
}

void I2S_Write(char* data, int numData) {
  i2s_write_bytes(I2S_NUM_0, (const char *)data, numData, portMAX_DELAY);
}
