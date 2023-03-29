# ชื่อโครงงาน : เครื่องให้อาหารสัตว์เลี้ยงแบบจำเพาะ
โครงงานนี้เป็นส่วนหนึ่งของรายวิชา 01204322 Embedded Systems ภาคปลาย ปีการศึกษา 2565 หมู่ 1

## สมาชิกผู้จัดทำ:  
> :shipit:นิสิตภาควิชาวิศวกรรมคอมพิวเตอร์ คณะวิศวกรรมศาสตร์ มหาวิทยาลัยเกษตรศาสตร์ วิทยาเขตบางเขน:shipit:  

6310500261 ชยพล สาสนรักกิจ<br>
6310500287 ตะวันฉาย ฉัตรกุล ณ อยุธยา<br>
6310503456 พฤกษ์ อ่วมเทศ<br>
6310503499 ภราดร วัชรเสมากุล<br>
6310503545 รัฐธรรม เอิบอิ่มฤทธิ<br>



<script src="https://kit.fontawesome.com/a302d730af.js" crossorigin="anonymous"></script>

<strong>
    <h2> 
         ที่มาและความสำคัญ <i class="fa-solid fa-circle-info fa-bounce"></i>
    </h2> 
</strong>
--- 

<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
สัตว์เลี้ยงของเราต้องการอาหารเพื่อสร้างพลังงานและเพื่อพัฒนาการของมัน โดยสัตว์แต่ละชนิดก็ต่างมีข้อจำกัดของอาหารที่รับประทานที่แตกต่างกัน ดังนั้นสัตว์เลี้ยงแต่ละตัวจึงไม่สามารถรับประทานอาหารประเภทเดียวกันหรือปริมาณที่เท่ากันได้ ซึ่งเป็นปัญหาสำหรับผู้เลี้ยงที่มีสัตว์เลี้ยงหลายตัวภายในบ้าน พวกเราจึงมีแนวคิดที่จะประดิษฐ์อุปกรณ์เครื่องให้อาหารสัตว์อัตโนมัติที่สามารถแยกแยะชนิดของสัตว์เลี้ยงได้โดยจำแนกจากสีของเครื่องแต่งกายที่สัตว์เลี้ยงของเราสวมใส่อยู่ รวมถึงระบบแจ้งเตือนปริมาณอาหารภายในถังเก็บอาหาร และยังสามารถปรับปริมาณอาหารได้ตามต้องการ อีกทั้งยังมีระบบป้องกันการให้อาหารเกินความจำเป็น โดยการกำหนดระยะเวลาที่จะให้อาหารในครั้งถัดไปจากการให้อาหารครั้งก่อนหน้าได้
</p>

<strong>
    <h2> 
         ฟีเจอร์ที่สำคัญ <i class="fa-solid fa-star fa-bounce"></i>
    </h2> 
</strong>
---


- แผงควบคุมประกอบด้วยจอ OLED และปุ่มควบคุมเพื่อตั้งค่า
    - สีของเครื่องแต่งกายของสัตว์เลี้ยงของเรา
    - ปริมาณอาหารที่ปล่อยให้ในแต่ละครั้ง
    - ระยะเวลาการให้อาหารในรอบถัดไป จะต้องห่างจากรอบก่อนหน้าเท่าไร
- การจดจำการตั้งค่า โดยเมื่อเปิดเครื่องขึ้นมาใหม่ค่าที่เคยตั้งไว้จะยังคงอยู่
- Node-Red Dashboard แสดงค่า parameter ต่างของเครื่องให้อาหาร และแสดงระดับอาหารภายในถังเก็บอาหารแบบ Realtime โดยสามารถเข้าได้ <a href="https://red.tawanchai.com/ui/#!/0?socketid=bEyyCUDPzm6VrG3NAAAd" target="_blank">ที่นี่</a>
- การป้องกันการให้อาหารที่มากเกินไป โดยเครื่องให้อาหารจะไม่ให้อาหารจนกว่าจะเลยเวลาที่กำหนดไว้


<strong>
    <h2> 
         แนวคิดและหลักการ <i class="fa-solid fa-book fa-bounce"></i>
    </h2> 
</strong>
---

<div style="text-align: center; margin: 5% 0%;">
    <img src="https://files.tawanchai.com/embedded/flow.png" alt="Diagram" border="0" style="border-radius: 15px; height: 75%; width: 85%">
        <div style="width: 100%; font-weight: bold; padding-top: 10px;">
            แผนภาพแสดงการทำงานของทั้งระบบ
  </div>
    
</div>

<div style="text-align: center; margin: 5% 0%;">
    <img src="https://files.tawanchai.com/embedded/nodered.png" alt="Diagram" border="0" style="border-radius: 15px; height: 75%; width: 85%">
        <div style="width: 100%; font-weight: bold; padding-top: 10px;">
            ภาพแสดงองค์ประกอบของ Node-Red Dashboard
  </div>
    
</div>

<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
เครื่องให้อาหารสัตว์เลี้ยง ประกอบด้วย Microcontroller NodeMCU ESP-WROOM-32S ทั้งหมด 5 ตัว โดยแต่ละตัวทำหน้าที่ที่แตกต่างกัน ดังนี้
</p></br>
<strong>
    <p> 
         บอร์ดที่ 1 สำหรับการ configuration ค่า parameter ต่างๆ
    </p> 
</strong>

<div style="text-align: center; margin: 5% 0% 3% 0%;">
    <img src="https://files.tawanchai.com/embedded/board1_des.png" alt="Diagram" border="0" style="border-radius: 15px; height: 75%; width: 75%">
        <div style="width: 100%; font-weight: bold; padding-top: 10px;">
            ภาพองค์ประกอบของบอร์ดที่ 1
  </div>
    
</div>


- ประกอบด้วย OLED 1 ตัวสำหรับแสดงผล และ ปุ่มกดทั้งหมด 3 ปุ่ม แทนการเลื่อนขึ้น เลื่อนลง และปุ่ม OK สำหรับเลือกเมนู
- เริ่มด้วยเมื่อเปิดเครื่อง microcontroller จะทำการดึง parameter ที่ถูกเก็บไว้ในฐานข้อมูลและนำมาแสดงหากหากสามารถดึงข้อมูลจากฐานได้สำเร็จ
- โดย parameter ที่สามารถแก้ไขได้มีดังนี้
    - **Color** คือสีของเสื้อที่เครื่องให้อาหารอนุญาติให้สามารถรับประทานได้
    - **Feed time** คือระยะเวลาที่บานควบคุมอาหารเปิด โดยยิ่งระยะเวลามาก ก็จะทำให้ได้ปริมาณอาหารที่มากตาม
    - **Time interval** คือระยะเวลาการให้อาหารในรอบถัดไป จะต้องห่างจากรอบก่อนหน้าเท่าไร เพื่อไม่ให้ระบบมให้อาหารมากเกินไป
- โดยเราสามารถเปลี่ยนค่า parameter ได้โดยการกดเลือกเมนูที่แสดงผ่านจอ OLED โดยหากทำการแก้ไขแล้ว ตัว microcontroller จะทำการ publish ค่าผ่าน topic ของ MQTT เพื่อบันทึกค่าลงฐานข้อมูล เพื่อที่หากเปิดเครื่องรอบใหม่ระบบก็จะสามารถดึงค่าที่เคยตั้งค่าไว้แล้วได้ และ publish ค่าไปยังบอร์ดที่จำเป็นที่ใช้ parameter ในการควบคุม ได้แก่ บอร์ดที่ 3,4 และ 5
- และทุกๆครั้งที่มีการเปลี่ยนค่า parameter หน้า node-red ก็จะแสดงผลการเปลี่ยนแปลงของข้อมูลอยู่ตลอดเวลา  

</br>
<strong>
    <p> 
         บอร์ดที่ 2 สำหรับวัดระดับ ปริมาณอาหารภายในถังเก็บอาหาร
    </p> 
</strong>



- ประกอบด้วย Ultrasonic Module จำนวน 1 ตัว โดยทำหน้าที่ในการวัดระดับอาหารที่อยู่ภายในถังอาหาร
- หาก Ultrasonic ตรวจจับระยะได้ใกล้ แสดงว่าระดับอาหารมีอยู่มาก
- โดย microcontroller จะทำการ publish ระดับอาหารผ่าน topic ของ MQTT ทุกๆ ครั้งที่ระดับอาหารมีการเปลี่ยนแปลง โดยจะไปแสดงผลที่หน้า node-red 



</br>
<strong>
    <p> 
         บอร์ดที่ 3 สำหรับตรวจจับสัตว์เลี้ยง และสีเครื่องแต่งกายของสัตว์เลี้ยง
    </p> 
</strong>


- ประกอบด้วย RGB Color Sensor Module 1 ตัว และ Ultrasonic Module 1 ตัว
- บอร์ดนี้จะทำการตรวจจับสัตว์เลี้ยงที่จะเข้ามากินอาหาร เพื่อส่ง publish message ด้วย topic ของ MQTT เพื่อให้บอร์ดที่ 4 ทำงานต่อ
- โดยบอร์ดนี้จะ publish message ไปยังบอร์ดที่ 4 ก็ต่อเมื่อ
    - Ultrasonic สามารถตรวจจับวัตถุได้
    - หาก Ultrasonic สามารถตรวจจับวัตถุได้นั้น บอร์ดจะทำการตรวจสอบสีของวัตถุ หากวัตถุมีสีที่ตรงกับสีที่ได้ตั้งค่าไว้จากบอร์ดที่ 1 หมายความว่า อนุญาติให้ระบบ publish ไปที่บอร์ดที่ 4 เพื่อเปิดที่ให้อาหารได้
      
</br>
<strong>
    <p> 
         บอร์ดที่ 4 สำหรับควบคุมบานเปิดปิดจานอาหารเพื่อป้องกันการกินอาหารที่มากเกินไป
    </p> 
</strong>


- ประกอบด้วย Servo 1 ตัว, Laser 1 ตัว และ LDR 1 ตัว
- โดยบอร์ดนี้จะมีหน้าที่ดังนี้
    - subscribe mqtt คอยรับสัญญาณจากบอร์ด 3 ที่ทำการตรวจสอบสัตว์เลี้ยงเพื่อเปิดและปิดฝาให้อาหารสัตว์เลี้ยงโดยใช้ Servo 
    - ตรวจสอบระดับอาหารในถาดด้วย LDR และ Laser ซึ่งถ้าตรวจสอบพบว่าอาหารในถาดหมดจะทำการ publish คำสั่งไปให้บอร์ด 5 เพื่อทำการปล่อยอาหารจาก tank ให้ไหลลงมาในถาด 
      
</br>
<strong>
    <p> 
         บอร์ดที่ 5 สำหรับควบคุมบานเปิดปิดอาหารที่ออกมาจากถังเก็บอาหาร
    </p> 
</strong>


- ประกอบด้วย Servo 1 ตัว
- คอยรับคำสั่งจากบอร์ดที่ 4 โดยการ subscriber topic MQTT ที่สามารถรอข้อความที่บอร์ดที่ 4 publish มาได้ โดย Servo จะเปิดตามจำนวนเวลาที่ได้ตั้งค่าไว้จากบอร์ดที่ 1



<strong>
    <h2> 
         อุปกรณ์ที่ใช้ <i class="fa-sharp fa-solid fa-screwdriver-wrench fa-bounce"></i>
    </h2> 
</strong>
---

- NodeMCU ESP-WROOM-32S [5 ตัว]
- Push Button ขนาด 6 mm [3 ตัว]
- 0.96" I2C 128x64 OLED Display [1 ตัว]
- Ultrasonic Module HC-SR04 [2 ตัว]
- RGB Color Sensor Module GY-31 (TCS230/TCS3200) [1 ตัว]
- Servo MG90S [2 ตัว]
- Laser Head Transmitter Sensor Module KY-008 [1 ตัว]

---

<strong>
    <h2> 
         Development Tools <i class="fa-solid fa-code fa-bounce"></i>
    </h2> 
</strong>
---
- **Hardware**
    - พัฒนาโดยใช้ <a href="https://platformio.org/" target="_blank"> PlatformIO</a> โดยใช้ในรูปแบบของ VSCode Extension
- **Backend**
    - <a href="https://mosquitto.org/" target="_blank">Mosquitto</a> สำหรับเปิด MQTT broker เข้าถึงได้ที่ mqtt://mqtt.tawanchai.com 
    - <a href="https://expressjs.com/" target="_blank">ExpressJS</a> สำหรับ Backend HTTP Server เข้าถึงได้ที่ <a href="https://foodfeeder.tawanchai.com/parameter" target="_blank"> foodfeeder.tawanchai.com</a>
    - <a href="https://redis.com/" target="_blank">Redis</a> เป็นฐานข้อมูลไว้เก็บข้อมูล parameter ได้แก่ color,time_interval,feed_time
- **Frontend**
    - พัฒนาบน Node-Red เข้าถึงได้ที่ <a href="https://red.tawanchai.com/ui/#!/0?socketid=jDoJNDb7RZkaeEbmAACK" target="_blank"> red.tawanchai.com</a>


Source Code <i class="fa-brands fa-github fa-bounce fa-lg"></i> : <a href="https://github.com/tawanchaiii/Embedded_Foodfeeder">github.com/tawanchaiii/Embedded_Foodfeeder</a>