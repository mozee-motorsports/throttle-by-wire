#![no_std]
#![no_main]

use panic_halt as _;
use pwm_pca9685::{Address, Channel, Pca9685};

const SERVO_MIN: u16 = 120;
const SERVO_MAX: u16 = 700;

#[arduino_hal::entry]
fn main() -> ! {
    let dp = arduino_hal::Peripherals::take().unwrap();
    let pins = arduino_hal::pins!(dp);

    let mut i2c = arduino_hal::I2c::new(
        dp.TWI,
        pins.a4.into_pull_up_input(),
        pins.a5.into_pull_up_input(),
        100000,
    );

    let mut pwm = Pca9685::new(i2c, Address::from(0x40)).unwrap();
    pwm.set_prescale(100).unwrap();
    pwm.enable().unwrap();
    pwm.set_channel_on(Channel::All, 0).unwrap();

    let mut current = SERVO_MIN;
    let mut factor = 1i16;

    loop {
        pwm.set_channel_off(Channel::C0, current).unwrap();
        pwm.set_channel_off(Channel::C1, SERVO_MIN + (SERVO_MAX - current)).unwrap();

        if current == SERVO_MAX { 
            factor -= 1; 
        } else if current == SERVO_MIN {
            factor = 1;
        }
        current = ( current as i16 + factor) as u16;
    }

}
