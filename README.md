# Smart Device Firmware

智能硬件固件，支持电量监测、分组控制、屏幕显示、Home Assistant 集成。

## 功能特性

- 🔋 **电量监测** - 实时监测电池电压、电流、容量
- 🎛️ **分组控制** - 多路继电器/可控硅分组控制
- 📺 **屏幕显示** - OLED/LCD 屏幕显示状态信息
- 🏠 **Home Assistant** - MQTT/ESPHome 集成
- 📡 **多种通信** - WiFi/Bluetooth/IR/RF遥控
- ⚡ **低功耗设计** - 深度睡眠+定时唤醒

## 硬件支持

- ESP32 / ESP32-S3 / ESP32-C3
- 0.96" OLED 显示屏 (SSD1306)
- INA219 / INA3221 电量监测芯片
- 继电器模块 (1-8路)
- IR/ RF 接收模块

## 快速开始

### 环境要求

- [PlatformIO](https://platformio.org/) 
- Python 3.8+

### 编译烧录

```bash
# 克隆项目
git clone https://github.com/binglingfeng/smart-device-firmware.git
cd smart-device-firmware

# 安装依赖
pip install platformio

# 编译
pio run

# 烧录
pio run --target upload

# 串口监视器
pio device monitor
```

### 配置

编辑 `src/config.h` 设置 WiFi、HA 参数：

```cpp
// WiFi 配置
#define WIFI_SSID "YourSSID"
#define WIFI_PASSWORD "YourPassword"

// MQTT / HA 配置
#define MQTT_SERVER "192.168.1.100"
#define MQTT_PORT 1883
#define MQTT_USERNAME "mqtt_user"
#define MQTT_PASSWORD "mqtt_password"
#define HA_DISCOVERY_PREFIX "homeassistant"
```

## 项目结构

```
smart-device-firmware/
├── src/
│   ├── main.cpp           # 主程序入口
│   ├── config.h           # 配置文件
│   ├── power.cpp/h        # 电量监测
│   ├── control.cpp/h      # 分组控制
│   ├── display.cpp/h      # 屏幕显示
│   ├── mqtt.cpp/h         # MQTT 通信
│   ├── wifi.cpp/h         # WiFi 管理
│   └── sensor.cpp/h       # 传感器数据
├── include/
│   └── .h                 # 头文件
├── lib/                   # 本地库
├── test/                  # 单元测试
├── platformio.ini         # PlatformIO 配置
└── README.md
```

## 功能模块

### 电量监测 (Power)

- 电压采样 (ADC / INA219)
- 电流计算
- 容量估算 (mAh)
- 充电状态检测

### 分组控制 (Control)

- 继电器控制 (1-8路)
- 定时任务
- 场景联动
- 手动/自动模式

### 屏幕显示 (Display)

- 系统状态
- 电量信息
- 控制界面
- 动画效果

### HA 集成

- MQTT 自动发现
- 实体状态上报
- 命令接收
- 传感器/开关/按钮

## 协议说明

### MQTT 主题

```
home/+/status          # 状态发布
home/+/set             # 命令接收
home/+/sensor          # 传感器数据
```

### HA 自动发现

- switch.* - 开关实体
- sensor.* - 传感器
- binary_sensor.* - 二进制传感器

## 许可证

Private - 仅供个人使用
