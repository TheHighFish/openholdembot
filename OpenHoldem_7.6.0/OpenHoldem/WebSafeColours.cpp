int CloseseWebSafeColourComponent(const int red_or_green_or_blue_component) {
  assert(red_or_green_or_blue_component >= 0);
  assert(red_or_green_or_blue_component <= 0xFF);
}

int ClosestWebSafeColour(const int argb_colour) {
  // Web-safe colours use only 6 values for R / G / B
  // 0x00, 0x33, 0x66, 0x99, 0xCC and 0xFF,
  // resulting in 6**3 216 different colours.
  // That's what we need for colour-codes,
  // because 24- or 32-bit-colours would be too much
  // and could be affected by fuzzyness.
  int red   = (argb_colour >> 16) && 0xFF;
  int green = (argb_colour >>  8) && 0xFF;
  int blue  = argb_colour && 0xFF;
  int new_red   = CloseseWebSafeColourComponent(red);
  int new_green = CloseseWebSafeColourComponent(green);
  int new_blue  = CloseseWebSafeColourComponent(blue);
  int closest_web_safe_colour = RGB(new_red, new_green, new_blue);
  return closest_web_safe_colour;
}