# IoT-The-automatic-fish-tank
Mô hình hệ thống hỗ trợ nuôi cá tự động có các tính năng sau:
- Tự động và bán tự động cho cá ăn.
- Kiểm tra nhiệt độ nước.
- Điều chỉnh ánh sáng.
- Màn hình LCD hiển thị thông tin.
- Kết nối mạng và IoT 

Hệ thống phần cứng:
- ESP8266 NodeMCU Lua V3 CH340
- Arduino Uno R3
- Đèn Led RGB 5050
- Động Cơ Servo SG90
- Biến trở chiết áp WH148 3 chân 1K
- Module thời gian thực DS3231 + ROM AT24C02
- Màn hình LCD 1602
- Cảm biến nhiệt độ DS18B20

![image](https://github.com/huedo2102/IoT-The-automatic-fish-tank/assets/118194834/53ab29ee-49f0-405e-a9af-431d961b5eb8)

![image](https://github.com/huedo2102/IoT-The-automatic-fish-tank/assets/118194834/6f92c0e0-d7a8-45c6-9230-9ee44bb14e59)

Chức năng các khối:
- Cảm biến: có nhiệm vụ đọc giá trị của các cảm biến đưa vào khối điều khiển. Linh kiện của khối này có cảm biến nhiệt độ DS18B20.
- Arduino là trung tâm điều khiển của hệ thống. Nó nhận dữ liệu từ khối cảm biến, xử lý dữ liệu đó và sau đó ra lệnh cho khối chấp hành điều khiển (như servo, led) để thực hiện các hành động cần thiết, như bật/tắt đèn, vv
- ESP8266 có chức năng truyền nhận tín hiệu từ IoT Cloud (Firebase) tới hệ thống. Kết nối hệ thống với Internet, cho phép giám sát và kiểm soát hệ thống từ xa thông qua ứng dụng di động. Nó cũng cho phép hệ thống gửi dữ liệu lên Firebase để lưu trữ và phân tích.
- Khối chấp hành điều khiển: Khối này chứa các thiết bị hành động như servo, Led. Dựa trên lệnh từ khối điều khiển, khối này thực hiện các hành động cụ thể, như bật/tắt đèn, vv..

![image](https://github.com/huedo2102/IoT-The-automatic-fish-tank/assets/118194834/fdc9ac9e-7c8f-441f-9c90-99904af6be42)

![image](https://github.com/huedo2102/IoT-The-automatic-fish-tank/assets/118194834/c61d40fc-267a-4336-b635-ee8394f86085)

![image](https://github.com/huedo2102/IoT-The-automatic-fish-tank/assets/118194834/3e58b847-d270-400f-9eac-abb162baa43e)

![image](https://github.com/huedo2102/IoT-The-automatic-fish-tank/assets/118194834/d1ae3d32-a503-415b-bdb0-e7d16cc791ac)

![image](https://github.com/huedo2102/IoT-The-automatic-fish-tank/assets/118194834/fce30506-22d8-4857-8cb6-7b3c354646e2)

