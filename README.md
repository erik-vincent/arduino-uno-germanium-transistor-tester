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
  <img width="820" src="https://i.postimg.cc/pX3rzXSc/Image3.jpg" alt="Arduino Uno Germanium Transistor Tester schematic">
</p>

<p>&nbsp;</p>
<h2>Tiny X-Mas Tree BOM</h2>
<p>
1x 10K ohm 1206 SMD resistor: <a href="https://www.digikey.com/en/products/detail/vishay-dale/CRCW120610K0FKEA/1176758">https://www.digikey.com/en/products/detail/vishay-dale/CRCW120610K0FKEA/1176758</a><br />
8x 261 ohm 1206 SMD resistors: <a href="https://www.digikey.com/en/products/detail/vishay-dale/CRCW1206261RFKEA/1176575"></a>https://www.digikey.com/en/products/detail/vishay-dale/CRCW1206261RFKEA/1176575<br />
1x 2032 Coincell Battery Holder: <a href="https://www.digikey.com/en/products/detail/mpd-memory-protection-devices/BA2032SM/257743">https://www.digikey.com/en/products/detail/mpd-memory-protection-devices/BA2032SM/257743</a><br />
1x 2032 3V lithium battery: <a href="https://www.digikey.com/en/products/detail/fdk-america-inc-a-member-of-fujitsu-group/CR2032/1202936">https://www.digikey.com/en/products/detail/fdk-america-inc-a-member-of-fujitsu-group/CR2032/1202936</a><br />
1x Two-pin header: <a href="https://www.digikey.com/en/products/detail/sullins-connector-solutions/PREC002SAAN-RC/2774852">https://www.digikey.com/en/products/detail/sullins-connector-solutions/PREC002SAAN-RC/2774852</a><br />
1x ISP header: <a href="https://www.digikey.com/en/products/detail/w%C3%BCrth-elektronik/61200621621/4846913">https://www.digikey.com/en/products/detail/w%C3%BCrth-elektronik/61200621621/4846913</a><br />
1x 0.1uF 0805 SMD capacitor: <a href="https://www.digikey.com/en/products/detail/samsung-electro-mechanics/CL21B104KBCNNNC/3886661"></a>https://www.digikey.com/en/products/detail/samsung-electro-mechanics/CL21B104KBCNNNC/3886661<br />
1x USB Tiny Programmer: <a href="https://www.amazon.com/KOOBOOK-USBtinyISP-Programmer-Bootloader-Interface/dp/B07S9BX98K/">https://www.amazon.com/KOOBOOK-USBtinyISP-Programmer-Bootloader-Interface/dp/B07S9BX98K/</a><br />
1x 0805 SMD LED variety pack: <a href="https://www.amazon.com/Assorted-Lighting-Electronics-Components-Emitting/dp/B01CUGA8JO/">https://www.amazon.com/Assorted-Lighting-Electronics-Components-Emitting/dp/B01CUGA8JO/</a><br />
</p>
<p>Have fun!</p>
