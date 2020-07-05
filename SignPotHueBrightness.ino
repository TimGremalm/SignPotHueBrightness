//Lighting Mode
float level = 0;
float hue = 0;

int potLightLevelLast = -10;
int potHoueLast = -10;

int pinRed = 5;
int pinGreen = 6;
int pinBlue = 9;

void setup() {
	Serial.begin(115200);
	Serial.println("Start SignPotHueBrightness");
	pinMode(pinRed, OUTPUT);
	pinMode(pinGreen, OUTPUT);
	pinMode(pinBlue, OUTPUT);
}

void checkLightLevelPot() {
	int potLightLevel = analogRead(A1);
	// Scale inverted pot 0-3.7V (0-780)
	potLightLevel = map(potLightLevel, 780, 0, 0, 255);
	// Avoid negative nambers due to voltage differences when scaling
	potLightLevel = max(potLightLevel, 0);
	// To avaoid flicker, only change level if steps exceeds a value
	if ((potLightLevelLast-potLightLevel > 3) || (potLightLevelLast-potLightLevel < -3)) {
		Serial.print("Light level changed to ");
		Serial.println(potLightLevel);
		level = potLightLevel/255.0f;
		potLightLevelLast = potLightLevel;
	}

	int potHue = analogRead(A2);
	// Scale inverted pot 0-3.7V (0-780)
	potHue = map(potHue, 780, 0, 0, 255);
	// Avoid negative nambers due to voltage differences when scaling
	potHue = max(potHue, 0);
	// To avaoid flicker, only change level if steps exceeds a value
	if ((potHoueLast-potHue > 3) || (potHoueLast-potHue < -3)) {
		Serial.print("Hue changed to ");
		Serial.println(potHue);
		hue = potHue/255.0f;
		potHoueLast = potHue;
	}
}

/** Helper method that converts hue to rgb */
float hueToRgb(float p, float q, float t) {
	if (t < (float)0)
			t += (float)1;
	if (t > (float)1)
			t -= (float)1;
	if (t < (float)1/(float)6)
			return p + (q - p) * (float)6 * t;
	if (t < (float)1/(float)2)
			return q;
	if (t < (float)2/(float)3)
			return p + (q - p) * ((float)2.0/ (float)3.0 - t) * (float)6;
	return p;
}

/* Converts an HSL color value to RGB. Conversion formula
-- adapted from http://en.wikipedia.org/wiki/HSL_color_space.
-- Assumes h, s, and l are contained in the set [0, 1] and
-- returns r, g, and b in the set [0, 1].
--
-- @param	 Number	h			 The hue
-- @param	 Number	s			 The saturation
-- @param	 Number	l			 The lightness
-- @return	Array					 The RGB representation*/
void hslToRgb(float h, float s, float l, int rgb[]) {
	float r, g, b;

	if (s == (float)0) {
			r = g = b = l; // achromatic
	} else {
			float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
			float p = 2 * l - q;
			r = hueToRgb(p, q, h + (float)1/(float)3);
			g = hueToRgb(p, q, h);
			b = hueToRgb(p, q, h - (float)1/(float)3);
	}
	rgb[0] = (int) (r * 255);
	rgb[1] = (int) (g * 255);
	rgb[2] = (int) (b * 255);
}

void tick() {
	int RGB[3] = {0, 0, 0};
	hslToRgb(hue, 1.0f, level, RGB);
	analogWrite(pinRed, RGB[0]);
	analogWrite(pinGreen, RGB[1]);
	analogWrite(pinBlue, RGB[2]);
}

void loop() {
	checkLightLevelPot();
	tick();
	delay(20);
}

