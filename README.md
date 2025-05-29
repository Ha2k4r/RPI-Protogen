# 🎨 **PROTOGEN VECTOR DRAWER FOR RGB MATRIX PANELS** 🚀  
`An open-source project for rendering vector graphics on RGB LED matrix panels, optimized for use in protogens in fursuits. Built for Raspberry Pi with a focus on performance and real-time rendering.`

[What is a protogen you ask?](https://knowyourmeme.com/memes/protogen)
---

### 📜 **FEATURES**
- **Real-Time Rendering:** Converts vector graphics into pixel-perfect displays on an RGB matrix.  
- **Open Source:** Licensed under the **MIT License WITH ATTRIBUTION CLAUSE** for freedom to use, copy, modify, merge, publish, and distribute.  
- **Raspberry Pi Basis:** Designed for the Raspberry Pi, with plans to fully utilize this Arguably overkill hardware.

---

## 🔧 **DEPENDENCIES**
This project leverages powerful libraries and frameworks to handle image processing, matrix control, and asynchronous communication:

- [OpenCV - Computer Vision Library](https://github.com/opencv/opencv)  
- [rpi-rgb-led-matrix - Control for RGB LED panels on Raspberry Pi](https://github.com/hzeller/rpi-rgb-led-matrix)  
- [Boost.Asio - Asynchronous I/O Library](https://github.com/boostorg/asio)  
- [Boost.Asio Official Documentation](https://www.boost.org/doc/libs/1_86_0/doc/html/boost_asio.html)

---

## 🛠️ **INSTALLATION**

1. Clone the repository:  
   ```bash
   git clone https://github.com/Ha2k4r/RPI-Protogen
   cd ~/RPI-Protogen/prootmk3/
   ```

2. Install dependencies:  
   ```bash
   sudo apt update && sudo apt upgrade
   sudo apt install libopencv-dev python3-opencv  
   sudo apt-get install libboost-all-dev
   ```

3. Build the project using `make`:  
   - Make the hzeller/rpi-rgb-led-matrix Library
   ```bash
   cd ~/RPI-Protogen/prootmk3/lib
   make 
   ```
   ```bash
   cd ~/RPI-Protogen/prootmk3/
   mkdir build
   make
   ```

5. Run the application:  
   ```bash
   sudo ./proot
   ```
---

## Hardware And Wireing 
- This program will work with any raspberry pi with standard 3.3v gpio - *EXCEPT THE RASPBERRY PI 5*.

- The screen is updated using Hzellers library, which can be modified for alternative wireing however the *Recomended* wireing is with a [Bonnet](https://www.adafruit.com/product/3211) with pins 4 and 8 shorted using a (preferably soldered) jumper wire to make use of pwm.

- Each Matrix must be on a different Power line than the *RPI*'s gpio 5v as it cannot provide enough power. I recomend cutting a charger cables end and soldering the wire into the bonnets provided power input lines, and the USBA side should plug into the *RPI*'s Native USB. *Images provided in : [Wiring](https://github.com/Ha2k4r/RPI-Protogen/tree/main/Wiring)
- Often these boards come with cables that have connectors crimped on. Cutting the ends on them and soldering them to a power source may be your best bet as it depends on your protogens screens power draw and desire for reliablity as the screens can easily draw 3.4 Amps of power. 

## 🧬 **HOW IT WORKS**
1. **Vectors:** Accepts pre-made geometry for sprites.  
2. **Color Overlays:** Layers geometry over images or GIFs.  
3. **Matrix Control:** Continuously updates the display with smooth transitions and animations.

---

## 🛑 **LICENSE**  
This project is licensed under the **MIT License WITH ATTRIBUTION CLAUSE**.  
You are free to use, modify, and distribute this project under the terms of the license.

#### **Key Points of the License:**
- Attribution is required for all distributions, modifications, and public uses.
- The software is provided "as is," without warranty of any kind.

For more details, see the [Full License Text](https://opensource.org/licenses/MIT).

---

## 🤝 **CONTRIBUTING**  
As the sole developer currently maintaining this project, contributions are welcome!  
- Submit issues or feature requests via GitHub Issues.  
- Pull requests with well-documented code and comments are appreciated.

---

## 🌐 **RESOURCES & ACKNOWLEDGEMENTS**  
This project wouldn't be possible without these incredible libraries and tools:  
- [OpenCV Repository](https://github.com/opencv/opencv)  
- [rpi-rgb-led-matrix Repository](https://github.com/hzeller/rpi-rgb-led-matrix)  
- [Boost.Asio Repository](https://github.com/boostorg/asio)  
- [Boost.Asio Documentation](https://www.boost.org/doc/libs/1_86_0/doc/html/boost_asio.html)

---

## 🎩 **SPECIAL THANKS**  
- To the **furry community** and **individual fursuit makers** for inspiring me so much to persue all of this.
  special thanks to :
  * [@Jting-F](https://www.youtube.com/@jting-f3237) for the *buetifull* and incredibly complicated Protogen design that inspired me to make my own!
  * [@WafflesDoesArt](https://linktr.ee/wafflesdoesart) for their amazing discord community and Fantastic work on their own designs with a toutorial that sent me on this spiral!
  * My friends who contributed in small but meaningfull ways!
  * **YOU** a potential user of *this* software! thank you, keep being great and have a *great* day!
  
