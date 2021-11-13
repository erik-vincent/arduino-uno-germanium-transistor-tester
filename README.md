# arduino-uno-germanium-transistor-tester
<p align="center">
  Arduino IDE C code for the Arduino Uno, using a SSD1306 OLED display and an ADS1115 16-Bit ADC<br />
  <img width="572" src="https://i.postimg.cc/q75xjL0X/20211112-223037.jpg" alt="Breadboard">
</p>

<p>I saw a post on DIY Stompboxes where someone else was attempting to do this, but I never saw them complete it</p>
<p>I knew one of the problems was that the ADC on the ATMEGA328P is kind of limited and prone to too much noise</p>
<p>So I found a breakout board for the ADS1115 16-Bit ADC was available and cheap, so I opted to use that to overcome</p>
<p>the native hardware limitations</p>

<h2>Arduino Uno Germanium Transistor Tester</h2>

<p align="center">
  <img width="820" src="https://i.postimg.cc/G2SdRXwg/Image4.jpg" alt="Arduino Uno Germanium Transistor Tester schematic">
</p>

<p>&nbsp;</p>
<h2>Arduino Uno Germanium Transistor Tester BOM</h2>
<p>
1x 1K ohm Metal Film resistor: <a href="https://www.digikey.com/en/products/detail/stackpole-electronics-inc/RNF14FTD1K00/1706678">https://www.digikey.com/en/products/detail/stackpole-electronics-inc/RNF14FTD1K00/1706678</a><br />
1x 1.2M ohm Metal Film resistor: <a href="https://www.digikey.com/en/products/detail/stackpole-electronics-inc/RNF14FTD1M21/1750283"></a>https://www.digikey.com/en/products/detail/stackpole-electronics-inc/RNF14FTD1M21/1750283<br />
1x 100uF Electrolytic Capacitor: <a href="https://www.digikey.com/en/products/detail/nichicon/UVR1H101MPD1TD/3438480">https://www.digikey.com/en/products/detail/nichicon/UVR1H101MPD1TD/3438480</a><br />
1x 100nF Ceramic Capacitor: <a href="https://www.digikey.com/en/products/detail/vishay-beyschlag-draloric-bc-components/K104K15X7RF5TL2/286538">https://www.digikey.com/en/products/detail/vishay-beyschlag-draloric-bc-components/K104K15X7RF5TL2/286538</a><br />
1x Arduino Nano: <a href="https://www.amazon.com/ALMOCN-Compatible-ATmega328P-Controller-Arduino/dp/B08HVPMLKG/">https://www.amazon.com/ALMOCN-Compatible-ATmega328P-Controller-Arduino/dp/B08HVPMLKG/</a><br />
1x I2C SSD1306 OLED Display Module: <a href="https://www.amazon.com/ALMOCN-Module-Serial-Display-SSD1306/dp/B092C8LB7B/">https://www.amazon.com/ALMOCN-Module-Serial-Display-SSD1306/dp/B092C8LB7B/</a><br />
1x I2C ADS1115 16-Bit ADC Module: <a href="https://www.amazon.com/TeOhk-Converter-Programmable-Amplifier-Development/dp/B081CJWGHZ/">https://www.amazon.com/TeOhk-Converter-Programmable-Amplifier-Development/dp/B081CJWGHZ/</a><br />
</p>
<p>Have fun!</p>
