
目标一：引脚配置

MCU:
GD32A712AVT3
外部高速时钟使用8MHz无源晶振
系统时钟配置为160MHz

LED:
LED0--PC2
LED1--PC3
LED2--PA0
高电平点亮

ADC:
ADC0_IN0--PI9
ADC0_IN1--PI10
ADC0_IN6--PI11
标定试验使用 ADC0_IN0--PI9
ADC 标定采样频率 3kHz
滑动平均窗口长度 32 点
电压换算基准 3300mV
UART1 标定数据输出保持 115200，按 10:1 降采样输出约 300Hz
ADC 标定触发源使用 TIM0_CH0，PJ7 复用为 TIM0_CH0，AF6，可作为 3kHz 触发波形观察输出；TRIGSEL 片内选择 TIM0_CH0 作为 ADC 触发源

KEY:
KEY1--PE13
KEY2--PE15
KEY3--PB11
KEY4--PJ5
高电平有效

NRST--PB7
SWDIO--PA13
SWCLK--PA14

UART；
UART1_TX--PB4
UART1_RX-PB5
波特率115200
8位数据位
无校验
1位停止位

普通PWM输出;
TIM7_CH1--PK1
TIM7_CH2--PG2
H桥SD--PK2
SD低电平关断，高电平使能
占空比参数使用0~10000表示0.00%~100.00%

OLED:
芯片：SSD1315
PJ10--I2C0_SCL
PJ11--I2C0_SDA
注意：使用软件IIC，不要用硬件；当前只生成引脚/端口配置，OLED显示驱动由用户移植



目标二：基于 GD32A7 定时器配置两路普通 PWM 控制 H 桥电机驱动。

硬件连接：
PWM1 → 左半桥驱动芯片 IN
PWM2 → 右半桥驱动芯片 IN
SD   → 两个半桥驱动芯片 SD#，作为使能/关断脚，MCU 引脚 PK2

注意：
1. 不使用互补 PWM 输出。
2. 不使用 TIMERx_MCHx 互补通道。
3. 半桥驱动芯片内部已根据 IN 自动控制 HO/LO。
4. 同一时刻只允许 PWM1 或 PWM2 一路输出 PWM，另一方向必须为 0。
5. SD# 为低有效关断，正常运行时 SD=1，停止时 SD=0。
6. 占空比不要长期 100%，最大限制 90%~95%，避免自举电容失效。

PWM 参数：
PWM 频率：20 kHz
初始占空比：0%
启动占空比：30%~40%
正常运行占空比：40%~80%
最大占空比：95%

控制逻辑：
正转：
SD = 1
PWM1 = duty
PWM2 = 0

反转：
SD = 1
PWM1 = 0
PWM2 = duty

停止/滑行：
SD = 0
PWM1 = 0
PWM2 = 0

防夹处理：
检测到过流或纹波异常后：
1. SD = 0
2. PWM1 = 0
3. PWM2 = 0
4. 延时 50~100 ms
5. 反向输出 PWM，执行短距离反转

定时器配置建议：
- 边沿对齐 PWM 模式
- 两个普通 PWM 通道
- 频率 20 kHz
- 占空比可软件动态修改
- SD 使用普通 GPIO 输出控制

调试注意：
第一次上电必须用示波器确认：
MCU PWM 高电平时，驱动芯片 IN 实际是否为高电平。
因为光耦可能会反相。
