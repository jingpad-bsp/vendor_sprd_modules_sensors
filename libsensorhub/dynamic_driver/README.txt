    ������̬�������������dynamic_driver.h��sensorhub_interface.h��sensorhub_interface.c�ļ���
(ƽ̨��)_dynamic_driver�ļ����µ�platform_config.h��Ȼ�������Ŀ���󣬲ο�dynamc_driver_(xxxx).c�ļ�
ʵ�ֶ�̬����������
1. dynamic_driver.h���嶯̬����������Ҫʵ�ֵĽӿ�ԭ�ͣ�
2. sensorhub_interface.h��sensorhub_interface.c�����ļ������˶�̬���������У�
   ��Ҫʹ�õ�I2C��д�ӿڣ�sleep��ʱ�ӿڣ�log��ӡ�ӿ�ʵ�֣�
3. (ƽ̨��)_dynamic_driver�ļ����µ�platform_config.hΪ��Ӧƽ̨������ã�������ƽ̨����������Ҫ������
   dynamic_driver_(ƽ̨��).ldΪ������Ӧƽ̨��̬����������Ҫʹ�õ����ӽű���
   ���������ļ���ͬƽ̨���ɻ��á�
   dynamc_driver_(xxxx).cΪ��̬���������ӿڵľ���ʵ�֣������߿ɲο�������������޸Ŀ��ƻ���
4. ��̬����������ϸ���ܣ��ο��ĵ���SensorHub��̬��������ָ��������EmBitz����Sensorhub��̬�����������ܡ�pressure_driver
   �ĵ����Դ�iSupport��վ��ȡ��
5. akm_cali�ļ��а�������akm09918 sensor��̬���ؽӿ�ʵ�֣��ڹ���������sensor�Ķ�̬���ع���ʱ�������ļ��м��빤�̣�
6. ltr578_driver_interface�ļ���Ϊltr578 sensor��̬���������ӿ�ʵ�֣�����ʹ�ã������ļ����빤�̱��룻
7. pressure_driver �ļ���Ϊbmp280/380/390 pressure sensor��̬���������ӿ�ʵ�֣�����ʹ�ã������ļ����빤�̱��룻
8. sc7a20_driver_interface �ļ���Ϊʿ��΢sc7a20��̬���������ӿ�ʵ�֣�����ʹ�ã������ļ��м��빤�̱��룻