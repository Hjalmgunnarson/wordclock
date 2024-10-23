#pragma once

#include <IotWebConf.h>
#include <NeoPixelAnimator.h>
#include <NeoPixelBrightnessBus.h>

#include "BrightnessController.h"
#include "ClockFace.h"
#include "nodo.h"
#include "Font.h"

// The pin to control the matrix
#ifndef NEOPIXEL_PIN
#define NEOPIXEL_PIN 32
#endif
//
#define TIME_CHANGE_ANIMATION_SPEED 400

class Display
{
public:
  Display(ClockFace* clockFace, uint8_t pin = NEOPIXEL_PIN);

  void setup();
  void loop();
  void setColor(const RgbColor &color);
  RgbColor getColor() { return _brightnessController.getOriginalColor(); };
  void setClockFace(ClockFace* clockface);
  void runBootAnimation();

  // Sets the sensor sentivity of the brightness controller.
  void setSensorSentivity(int value) { _brightnessController.setSensorSensitivity(value); }

  // Sets the sensor sentivity of the brightness controller.
  float getRawSensorValue() { return _brightnessController.readSensor(); }

  // Sets whether to show AM/PM information on the display.
  void setShowAmPm(bool show_ampm) { _show_ampm = show_ampm; }

  // Starts an animation to update the clock to a new time if necessary.
  void updateForTime(int hour, int minute, int second, int animationSpeed = TIME_CHANGE_ANIMATION_SPEED);

  // Used by MQTT Client to switch off and back on without overriding the color.
  void setOff();
  void setOn();
  bool isOn() { return !_off; }
  
  void setMatrix(std::vector<RgbColor> colorValues);
  void clearMatrix();
  void scrollText(IotWebConf &iwc, String text, RgbColor textColor, int speed = 200, bool rightToLeft = false);
  enum Mode {
    BOOT,
    CLOCK,
    MATRIX,
    TICKER
  };
  Mode getMode() { return _mode; }

private:
  // Updates pixel color on the display.
  void _update(int animationSpeed = TIME_CHANGE_ANIMATION_SPEED);

  // To know which pixels to turn on and off, one needs to know which letter
  // matches which LED, and the orientation of the display. This is the job
  // of the clockFace.
  ClockFace* _clockFace;

  // Overrides color to #000000 when true.
  bool _off;

  Mode _mode;
  std::vector<RgbColor> _matrix_buf;
  
  // Whether the display should show AM/PM information.
  bool _show_ampm = 0;

  // Addressable bus to control the LEDs.
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> _pixels;

  // Reacts to change in ambient light to adapt the power of the LEDs
  BrightnessController _brightnessController;

  //
  // Animation time management object.
  // Uses centiseconds as precision, so an animation can range from 1/100 of a
  // second to a little bit more than 10 minutes.
  //
  NeoPixelAnimator _animations;

  // Reserved for boot animation.
  NeoPixelAnimator _bootAnimations;

  static double _distance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
  }

  uint16_t _circleCount = 0;
  RgbColor _circleColor;
  uint16_t _circleCenterX = 5;
  uint16_t _circleCenterY = 11;
  void _circle(uint16_t x, uint16_t y, int radius, RgbColor color);
  void _colorCornerPixels(RgbColor color);
  void _circleAnimUpdate(const AnimationParam &param);
  void _fadeAll(uint8_t darkenBy);
  void _fadeAnimUpdate(const AnimationParam &param);
  void _displayCharacter(FontTable fontTable, char character, int scrollPosition, RgbColor color);
};
