## ����ƽ̨��RM������

## FreeRTOS: V9.00

## HAL��汾��1.13.0

## ���ߣ�Satori

# 2019/2/13
������App��Bsp�����ļ��У��ֱ��RTOS��task��bsp��Ķ�����Ŀǰ�����Ȱ�ң�������ݵĽ���������Ȼ���can�ķ������������ʵ��һ��ң�����ص��

��Ҫ������RemoteMsgTask.c��bsp_uart.c�����ļ�

# 2019/2/14

����һ���ӵ�Ҫ���ĵط�......

CUBEMX���ɵĹ������źͰ�����ʵ���õ����Ų�ͬ���кü������ͷǳ���

ң�ؽ�����ɣ�����������ģ�ʵ���ϱ���DJI�Ĵ���Ҳû���������������һ���Ƚ�����˼�ģ�ר���и���������ִ��ģʽ�л����Ҿ�������Ҳ���Գ���һ�����

# 2019/2/15

���ź���Ū��һ��ģʽ�л�
```
void RemoteMsg_Receive_ModeSw_TaskStart(void * argument)
{
    osEvent Event;
    while(1)
    {
        Event = osSignalWait(RC_MODE_SIGNAL | KM_MODE_SIGNAL | ST_MODE_SIGNAL , osWaitForever);

        //Ȼ����Ƿ�һ��ģʽ��
        if(Event.status == osEventSignal)
        {
            if(Event.value.signals & RC_MODE_SIGNAL)
            {
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,RESET);
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,SET);
            }
            if(Event.value.signals & KM_MODE_SIGNAL)
            {
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,RESET);
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,SET);
            }
            if(Event.value.signals & ST_MODE_SIGNAL)
            {
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,RESET);
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,RESET);
            }
        }
    }
}
```
����������һ���������⣬�о����������һ������ĳ�����ȥŪ�ȽϺã���Ȼ��ʱ��д�����߰���Ĺ��̳�����̫����

�������һ�³��򣬺��������������ȥ�ټӸ��İ�
Ŀǰ����ģʽ�����֣�
```
typedef enum
{
chassis_follow_gimbal_remote_control��
chassis_follow_gimbal_keymouse_control��
chassis_free_keymouse_control��
chassis_sway��
chassis_stop��
}chassis_mode;

typedef enum
{
gimbal_remote_control��
gimbal_keymouse_control��
gimbal_auto��
gimbal_stop��
}gimbal_mode; 

typedef enum
{
shoot_auto��
shoot_remote_control��
shoot_keymouse_control��
shoot_stop��
}shoot_mode��
```
���У������ң��������ģʽ������
```
chassis_mode = CHASSIS_FOLLOW_GIMBAL_REMOTE_CONTROL

gimbal_mode = GIMBAL_REMOTE_CONTROL

shoot_mode = SHOOT_REMOTE_CONTROL/SHOOT_STOP
```

�����ֹͣģʽ������
```
chassis_mode = CHASSIS_STOP

gimabl_mode = GIMBAL_STOP

shoot_mode = SHOOT_STOP
```
����Ǽ��̿���ģʽ������
```
chassis_mode = CHASSIS_FOLLOW_GIMBAL_KEYMOUSE_CONTROL/CHASSIS_FREE_KEYMOUSE_CONTROL/CHASSIS_SWAY/CHASSIS_STOP

gimbal_mode = GIMBAL_KEYMOUSE_CONTROL/GIMBAL_AUTO/GIMBAL_STOP

shoot_mode = SHOOT_KEYMOUSE_CONTROL/SHOOT_STOP
```
# 2019/2/16
����������ܻ����ϴ��ϣ�������������ͼҲ����һ���֣�������Ǹ�������ľ���ʵ�����ˣ�����Ӧ���ܹ��ѵ��̵�task����ɵ�
��ͷ����Ҫ��һ��bsp_can����canͨ�ŵ���ز���
```
static void chassis_twist_handler(void)
{
  twist_count++;
  
  if (twist_side > 0)
  {
    if (gim.sensor.yaw_relative_angle >= 2*twist_angle)
    {
      twist_count = 0;
      twist_sign  = -1;
    }
    
    if(gim.sensor.yaw_relative_angle <= 0)
    {
      twist_count = 0;
      twist_sign  = 1;
    }
    
  }
  else
  {
    if (gim.sensor.yaw_relative_angle >= 0)
    {
      twist_count = 0;
      twist_sign  = -1;
    }
    
    if(gim.sensor.yaw_relative_angle <= -2*twist_angle)
    {
      twist_count = 0;
      twist_sign  = 1;
    }
    
  }
  chassis.position_ref = -twist_sign*twist_angle*cos(2*PI/twist_period*twist_count) + twist_side*twist_angle;
  
  chassis.vw = -pid_calc(&pid_chassis_angle, gim.sensor.yaw_relative_angle, chassis.position_ref);
  
}
```
ICRA�ĳ���chassis_twist_handler

# 2019/2/17

�������ʱ�����ˣ����������ʱ�жϻص���̨�Ŀ�������͵��̵Ŀ�������

Ȼ��ղ�����һ�³����ּ�����ֵ����飬�����ʱ��û�����ɹ����������Ľ����osError���ǳ��ķ�ŭ��֪�������Ǹ�ʲô��

Ȼ����һ������Ϊ��FreeRTOSConfig.h��������þͲ��ԣ������һ���ǳ���Ȥ�Ŀ��ˣ���������ô�ô���������û���뵽��ȥ��FreeRTOSConfig.h�ȸ��޸�һ�£�����

Ȼ����ܳɹ��������ʱ���ˣ���ס�����ʱ����Ĭ�����ȼ���belowNormalPriority,�������������������������̬���оͶ���������Ĵ����

����һ���ǳ��ӵĶ�����������ͻȻ����һ��ʼ�Ҵ������̵�ʱ��ò���Ǵ���ֻд��demo����ɶ�ģ�����һ��ʼ��ʼ���Ĵ����ж�ֻ�д���6һ������ͷǳ��Ŀ��ˣ���Ϊʵ���õĹ������Ǻü��������ж϶���Ҫ���ϣ���ʱһ����֪����ķ�լ�������Ա�״̬�����ҵ����Ҷ�ʱ��������ͳ�ʼ����һ��6�Ŷ�ʱ������jb����

����һ�����ص���������ICRA�ĳ���Ū��һ�Ѷ�ʱ�������Ҳ���֪��������ô�ඨʱ����������һЩʲô������ҷǳ��ǳ���

# 2019/2/18

��ѧû�����������ӣ��Ȱ�can���շ�д����˵������Ŀǰû��mpu�����߰�����������ϣ�����������Ҳ��̫�����ϸ���ҵĳ�����ͼ��

������д����bsp_can�����ֲ�����һ���Լ�д��can�����

��д�����أ�����������

�ǳ��ǳ��ı������˼���֮��ͻᱨcan_error����Ҳ���±ƺܲ�ˬ��ܳ

׼ȷ��˵��������һ��״̬

����CAN���ͻ������η��ʹ��ڳɹ��ģ�ȷʵ������HAL_CAN_STATE_BUSY_TX����ȥ����������֮��ͻ���������error��������Һ��±�

����ò��can�Ľ����ж�Ҳû�н�ȥ

��Ҫ�漰����������`hcan1.Instance.TSR`��һλ�������������͵������

0x1C000003

���ҵ������

0x1C000000

0x19000008

0x52000008

0x82000008

emmmm�õİɣ����ҾͿ����������ı߻�ʹ�����ֵ�仯����ҵ��ǣ���û�ҵ�

�ð���tm���Ǹ�ɵ�ƣ��Ұ���ICRA���������ý��еĳ�ʼ��������һ�����ö�û�У�Ȼ���ҾͰ���A�Ͱ�Ľ�����һ�����ã�Ȼ������
```
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SJW = CAN_SJW_1TQ;
  hcan1.Init.BS1 = CAN_BS1_9TQ;
  hcan1.Init.BS2 = CAN_BS2_4TQ;
  hcan1.Init.TTCM = DISABLE;
  hcan1.Init.ABOM = DISABLE;
  hcan1.Init.AWUM = DISABLE;
  hcan1.Init.NART = DISABLE;
  hcan1.Init.RFLM = DISABLE;
  hcan1.Init.TXFP = ENABLE;
```
��һ��'��ȷ'�����ú�'����'������
```
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 5;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SJW = CAN_SJW_1TQ;
  hcan1.Init.BS1 = CAN_BS1_3TQ;
  hcan1.Init.BS2 = CAN_BS2_5TQ;
  hcan1.Init.TTCM = DISABLE;
  hcan1.Init.ABOM = ENABLE;
  hcan1.Init.AWUM = DISABLE;
  hcan1.Init.NART = DISABLE;
  hcan1.Init.RFLM = DISABLE;
  hcan1.Init.TXFP = DISABLE;
```
��Ȼң����������Ҳ�������ˣ����Ҳ�ܶ��ˣ���ô��������ֻʣ��һ�������ˣ��Ǿ���......����дһ���ܶ��ĵ��̳������ٺٺ�

# 2019/2/19

Ȼ���Ҿ͹��ˣ�û���ļ�д�꣨Ŷ���군.jpg��

��������ѵ����̵���Ľ����д���ˣ����ǿ��Լ�д���̲���ʵ�ĸ��ܵ�FreeRTOS��ǿ��֮��

��һ�����Խ�ʡӲ����Դ�����������ʱ�����ܣ������������ʱ�ж�Ч����Ӳ����ʱ�ж�û�и��ܵ�����

�ڶ�����������������֪ͨ�ĸ��ߵĳ���˼·������ʹ��̵�˼·��ø������������е����񶼿��Գ�����̣߳�ֻҪ�����߳�֮��Ĺ�ϵ�Ϳ��ԣ�������֮ǰ������ѯ-�жϵĴ���Ҫд�����ͱȽ�����˼·���ң���Ϊ�����ڸ����жϻص������Ҷ�����

������������������osSignalSet()��osSignalWait()����������������˵�������ǳ���ˬ�����Ե���һ�����ܵ�'�ⲿ�ж�'���ã�ĳ�̶ֳ�����������˵OS��ʵ��չ��32���ж�ϵͳ

�ܵ���˵����Ŀǰ����os��ʹ����ʵҲ�Ÿո��Ǳ�ɽһ�ǣ���Ҳ��ʵ�ĸ��ܵ�os�Ĺ�����ķǳ��ǳ�ǿ���ú��˿��Էǳ��õİ�������Ӵ�Ĺ���

ϲ����bug�����յ�ʱ��������жϻص�������Ȼ��ͱղ��˻����ǳ��������˿���˵
�������յ����ҵķ�������ȫ�����ģ�����CAN��ͨѶЭ�飬ÿһ֡��Ӧ������Ack�ģ���ͷǳ��ǳ��ǳ����

��֮��������ȫû���κ�����ģ����ǽ��վʹ�ʼ���ն���һ��Ϊ0����ʹ�Ҽ�������

�㶨�ˣ���ʼ���Ĺ�����Ū���е����⣬����Ȼ�о��Լ�Ū�ú��ܶ������������𣬵��������Ƕ�����������������

��һ��������õ�
```
void Can_Device_Init(CAN_HandleTypeDef* hcan)
{
  CAN_FilterConfTypeDef canfilter;
  
  static CanTxMsgTypeDef  Tx1Message;
  static CanRxMsgTypeDef  Rx1Message;
  static CanTxMsgTypeDef  Tx2Message;
  static CanRxMsgTypeDef  Rx2Message;
  
  canfilter.FilterMode              = CAN_FILTERMODE_IDMASK;
  canfilter.FilterScale             = CAN_FILTERSCALE_32BIT;
  
  canfilter.FilterIdHigh            = 0x0000;
  canfilter.FilterIdLow             = 0x0000;
  canfilter.FilterMaskIdHigh        = 0x0000;
  canfilter.FilterMaskIdLow         = 0x0000;
  
  canfilter.FilterFIFOAssignment    = CAN_FilterFIFO0;
  canfilter.FilterActivation        = ENABLE;
  canfilter.BankNumber              = 14;
  
  if(hcan == &hcan1)
  {
    canfilter.FilterNumber = 0;
    hcan->pTxMsg = &Tx1Message;
    hcan->pRxMsg = &Rx1Message;
  }
  if(hcan == &hcan2)
  {
    canfilter.FilterNumber = 14;
    hcan->pTxMsg = &Tx2Message;
    hcan->pRxMsg = &Rx2Message;
  }
  
  HAL_CAN_ConfigFilter(hcan, &canfilter);
  
}
```
# 2019/2/20
������ֱ�ӳ�ʼ����ʱ��ֵһ��pid�������������ֲ����ף���Ϊ��������һ�����ƣ�
```
#include <stdio.h>

typedef struct
{
    int a;
    int b;
} B;

typedef struct
{
    int c;
    B d;
} A;

int main()
{
    A C.d={1,2};
    printf("%d",C.c);
}

�����Ľṹ�帳ֵ���޷��ɹ���

#include <stdio.h>

typedef struct
{
    int a;
    int b;
} B;

typedef struct
{
    int c;
    B d;
} A;

int main()
{
    A C = {1,{2,3}};
    printf("%d",C.d.a);
}
```
������Ƕ�׾Ϳ�����

����һ�����̣����ĺ����һ��������ʹ��������Ȼֻ��˵����Ϸ���鲻�Ǻܼѣ��Ͼ�pid�������ıȽ��ã���ͷ���ܻ��ٵ�����˵��ͷ��ô��ľ����Ǵ��������ľ͸���Һ��±�

����һ������������飬RM�������can2�ӿ��Ǹ�tm4pin�ģ���c610�����������2pin�ģ���Ҫ�Լ�ħ��һ����ͷ�ˣ�nmd��wsm

������Ҫ��ʼ�����д��̨�ˣ���������֮ǰ�����и�С���⣬��������������̨����֮��Ĺ�ϵ������δ���

����һ����AHRS�����涮�ˣ�ͦ����˼�ģ��ʼǶ�����onenote������

# 2019/2/21

������һ�����⣬��Ϊһ������jy901ʹ���ߣ�Ϊʲô��Ҫ����AHRS�����أ�

�ðɹ���һ�����Ǻõģ��Ͼ�ICRA�ĳ����ð���9������ȵ����費��

Ȼ��������ҵ������㷨ͼ����gimbal��shoot��ң�ز��ֵĴ���д����˵���ҵ��ǲ�ָ���ܹ���������ҵ�һ����������������̨��

# 2019/3/23

�������ˣ���Ҫ��ʼ���ˣ���Ȼ�Һ����������д���ĳɷ���unix/linux���ģ���������ʵ����̫����ʱ���ˣ������һ��Ǿ�����д�ɣ��������Ҳ��Ҫ�������ַ��ͻ��е����

Ȼ�������������������������Ƶ֮��Ϳ����Ʒ���д�ˣ���Ϊ��ͷ�¹���+�����İ汾��Ҫ˫������Ū���G�ٺ�

# 2019/3/24

��ҹ�� mpu�Ĵ������ֲ��һ�£����ǻ�û�������о�������������Ҿ��ñȽϹ���Ǻ�����Ҫ�Լ����ƫ�ã�������ô6p����

����������һ���ܲ�������

����һ�����ã����ҷ·�Ч����������ӣ������廹�ǵ��ø���������һ�����ݣ����һ�˵����ȫûȡ����ƫ�ð�

Ч���������ͷ�����ݲ��ᶯ������һ�·��������ǽ��ٶȾͲ��ᶯ�����е������о��о�

���۾���spi��ȡ�˸��ٵģ��Ĵ������ݸ���û�������������������һ��������о����ı߳�������

Ȼ���ֹ�Ȼ��device_initû���ã��������ʱ�򱨵�һ�������Һ��±�
```
../Src/main.c(129): warning:  #223-D: function "mpu_device_init" declared implicitly
```
������Ӧ��������Include��ͷ�ļ�

extern��һ�£������bug����ˣ�Ȼ����������Ҳ����mpu�������Ĵ�����������Ȼû��������е㻳���ǲ�����IO�ڳ�ʼ����������

����IO��ʼ�������⣬���ݳɹ��������ˣ����Ҿ������ԣ�pitch������ݶ�ȡ�ǳ��ĺã�Ȼ��yaw��ȴһֱ��Ʈ�������һ�·����������ڴ����Ƶ�����û�б�������ȡ������Ȼ�����ر���һ���£�����ֲ������������ȫȫû�Ѵ����Ƶ����ݴ����ںϽ������棬������Ҹо��ǳ��ǳ���Ȥ������Ϊ�˹����Կ��ǣ���ͷ�ҿ϶���Ҫ������ֲһ��ICRA�Ǹ���������Ľ���ģ��Ǹ�����Ѵ��������������yaw��������棬����������ʱ������ˣ������õ�������ĽǶ�ֵ���ջ��ɣ�����������������ֲ��漰��ͷģʽ������Ҫ�߾��ȵ�yaw������

# 2019/3/25

��Ҫ����ˣ����ɻᣬ�����Ŀ���ǰ���̨�Ŀ��ƴ�����ɣ����ǵ�bsp�����Ѿ�д���ˣ�mpu��������Ҳ�Ѿ��ܶ��ˣ���ôʣ�µ������޷Ǿ��Ƿֳ�pitch���yaw����ԵĿ���ʵ����

# 2019/3/28

��ʼдɵ�����룬IMU��pitch��������gimbal_motor_pitch���ȰѼ�����������ȥ��˵�ɣ��÷���������������Լ�һ�������CAN�����жϰѷ����Ķ�����д���ˣ��������ң�����Ч����jb�ߣ� 

����һ�����ݣ�����������������������ٶȷ�����0-16-��-32������ֵ֮�����䣬������ᣬ��Ҫ����һ���˲����߻��������������𣿷���ֱ���ô���ʳ���

����һ��ICRA�Ĵ��룬������Ȼ�����õ�IMU���Ǿ�ûʲô��˵����

�򵥲�����һ�£�pitch����ٶȷ����ܺÿ���yaw�����-4��ƫ�ã������ֶ���ƫ�ø��������ˣ�����һ�¾���һ�仰�����ȵ�һƥ

����һ������ִ��ʱ�䣬imu_task��ű�gimbal_task����һ��ʱ�䵥λ�����ǵ�1��ʱ�䵥λͦС���ˣ����Ի����Ͽ��Ե�����ͬ�����µģ��G�ٺ�
Ȼ�����ICRA���ӵ�������˵һ����̨�Ŀ����㷨�ɣ�����˵������������
```
while(1)
{
  pid_calc(&pid_yaw, gim.pid.yaw_angle_fdb, gim.pid.yaw_angle_ref);
  pid_calc(&pid_pit, gim.pid.pit_angle_fdb, gim.pid.pit_angle_ref);
  
  gim.pid.yaw_spd_ref = pid_yaw.out;
  gim.pid.pit_spd_ref = pid_pit.out;
  
  gim.pid.yaw_spd_fdb = gim.sensor.yaw_palstance;
  gim.pid.pit_spd_fdb = gim.sensor.pit_palstance;
  
  pid_calc(&pid_yaw_spd, gim.pid.yaw_spd_fdb, gim.pid.yaw_spd_ref);
  pid_calc(&pid_pit_spd, gim.pid.pit_spd_fdb, gim.pid.pit_spd_ref);

  if (gimbal_is_controllable())
  {
    glb_cur.gimbal_cur[0] = YAW_MOTO_POSITIVE_DIR*pid_yaw_spd.out;
    glb_cur.gimbal_cur[1] = PIT_MOTO_POSITIVE_DIR*pid_pit_spd.out;
    glb_cur.gimbal_cur[2] = pid_trigger_spd.out;
  }
  else
  {
    memset(glb_cur.gimbal_cur, 0, sizeof(glb_cur.gimbal_cur));
    gim.ctrl_mode = GIMBAL_RELAX;
    //pid_trigger.iout = 0;
  } 
}

//ICRA���Ƴ�����ţ��֮�����ڰ�ȫ��Ū���ر��ر�ã��ܱ�ը�ĵط�����������
```
Pitch�Ŀ��ƴ���д���ˣ�֮��Ҫ���������ֻ��������һ������ΰѼ��������pidת����ʵ�ʸ��������ֵ������һ������ʲô��ʽ����ң����������
Ȼ�����yaw����˵���鷳��΢��һ�㣬��Ҫͨ��ĳ�ּ��㷽ʽ�ѵ����ʵ�ʽǶȻ��㵽��imu������ͬһ���������Ķ�����Ȼ�����ظ�pitch��Ĺ������÷���

ת����p��ֱ�ӵ��ξͺ���

���Ĳ�����

λ�û�kp:8 ki:0 kd:0

�ٶȻ�kp:21 ki:1.3e-06 kd:0

���־���Ƚϴ�
```
Motor_t Gimbal_Motor_Pitch  = {{{13.0f,0.000001f,0.0f},{10.0f,0.00019f,0.0f}}};
```
һ���ȽϺõĲ���

yaw��Ƕ����˼򵥵���ѧ��ʽ�궨��һ�£��ѱ�����ת������ʵ�ʽǶ�ֵ��Ȼ�����������ȥд����+���Ծ�ok�ˣ�������ô�ෳ�˵Ķ���������

yaw��Ū����

����������

��Ϊ�ҷ�������һ����������飬yaw�ᵥ�������ܺã�������pitch�ᵥ�������ܺã�������������һ�𡪡�boom  

���ˣ���û�뵽ϵͳ֮������໥���棬�����ˣ�ܳ

# 2019/3/29

�������������ĳ�������Ļ���ң���������Ǽ�ⲻ�˵ģ��������Ŵ�����������������ø���ֵ�ͽ������ģ��������ֲ��ԣ�����ң������;���˾ͻ���Ŷ���군

������������ͷ����Ҫ�ú���һ����ô������÷���

���˰�����������ֻ��Ǻ���������ᣬpitch��ĽǶȻ��ܵ�yaw������Եĸ��ţ���˼����һ�¾������б�Ҫ����ù��pitch��Ƕ�����һ���˲��������Ļ��ᱬը����ô����ʲô�˲���ʽ�أ�

1.��ֲ�����icra�Ŀ������˲�
2.��ֵ�˲��������
��˼������֮���Ҿ�������һ����ֵ�˲�Ūһ�¿����ܲ��ܺ�һ�㣨pitch����Ŷ������̫��̫�����ˣ�

# 2019/3/30

�����

�ҷ���

�Ұ�

pitch��ķ�����yaw��ķ���Ū����

��ɱ���Լ�

Ȼ����IMU�ĽǶȷ����ǳ���ʺ�������ӳٳ̶ȷǳ����أ��ܵ���˵���Ǻܱ�ը��������ֱ�ӻ������ñ����������������������̨Ч��ok��

Ȼ���µ����ⷢ�֣�����ң�������ݽ����任������£����̻��ܷɣ��������о�һ�����bug

�������ֻ�ǵ���pid����̫���˶��ѣ�С����

��һ������ң����ȥ����һ����̨������pitch�������ң�������ˣ�yaw������ʱû���ر�����Ȥ

# 2019/3/31

��������õĻ��Ͱѷ��������Ū���

�����Ժ�������ͷ��Ū������ӣ����и��Ӿ����Ľӿڣ�

����������Ǻ�Ū����s1����һ��ģʽ�����ˣ������߼��õ��Ǵ�0��Ħ���֣���2�����֣��ٴ�0��Ħ���֣���2�ز��֣���jb�鷳����

��������Ħ�����ٶȱջ����ַ��˵�����Ū��Ūȡ�����ҵ����飨

Ȼ���ֱȽ���������飬��Ҫ����һ�´���ṹ��֮ǰ�ǰ�Ħ���ֹ���CAN1����ģ��������ڵ�Ӳ���ṹ����Һ�����ֻ����CAN2��Ū�ˣ��е㷳��

����һ���Լ���ɵ�ƴ��룬����ȷʵ��û��ô����Ū

����������˸������˵����⣬���ɵ��2006������������ҵĻ�������Ħ���ֵ��Ƕ��õúܣ������m2006�������ᣬ����Ū�ſ���������ɶ�����

���������C610�������,mdzz����ͷ����Ҫ��Ҫ����ˢ���̼�ɶ�ģ���ɵ�������˷����ҿ�һ��Сʱ����������ķ���

# 2019/4/1

��ʼͷ������������ƴ��룬�о���ʵû��ʲô�ر��˵�ģ���ң��ģʽ��Ҫʵ�ֵĹ���Ҳ�ܼ򵥣���s1����Ϊ�����缫�Ŀ��ƾ�ok�ˣ�ûɶ�ر������⣬��������ȿ�һ��ICRA�Ŀ����߼�

�ޣ��������ICRA����ô�򵥵Ķ���

��һ�ε����棬Ħ���ֿ���������Ħ���ֹ�

��һ�ε����棬���ֿ��������β��ֹ�

Ȼ���һ��˲���ʱ���ڰ��ҵ�developing diaryת��markdown��markdown����棬�G�ٺ٣�ʵ����������

��������Ѹ��Ӿ��Ľӿڸ�Ū����Ȼ�����һ�µģ���������Ѿ������������ȹ�����

# 2019/4/2

�����ӣ������ٰѼ����Ūһ�º���͵ðѳ��Ӹ������͹�ȥ��_(:3_|��)_

����֮��ӿ�ɶ�Ķ������ˣ�ֱ��д��ok����

������˵��������һ��ʵ�ʵĿ��Ƴ�������̨�Ǳ߷����Ǻ��񻹻��дһ�㣬ֱ�ӽ����ľ���λ��ֵ����̨�Ƕ�֮����һ��ӳ��Ϳ����ˣ���������Ǳ�Ҳ����̫�鷳��һ���Ҽ���Ħ���֣�һ���Ҽ���Ħ���֣�һ����������֣�һ������ز��֡����Ҫ˵�Ļ���ʵӦ��дһ�µ���ģʽ������ģʽ�ģ�������߹�����ô�������������֮�ȹ�������

Ȼ���ǹ��ڵ��̵Ŀ��Ʋ������⣬��֮���������Ŀ����߼����򵥹�Ħһ�£�

���ԣ������ȵĲ����Ҹ��������һ��ң�������ͼ�������Ȼ��궨��

Ц�����ˣ��������Ǹ���ù�������ǵ�����ʽ���ƣ���ͷ�����дһ����̨���ŵ��̶����߼���Ŀǰ��ʵ�ֻ����Ĵ��Ŷ�������

����˦����꣬����һ�·�ֵ�ܵ�500�ͺ��˲����ˣ��ҿ��Խ�˻����������������궨һ��

�㶨����̨�������ȿ�����sys_config.h�����

Ȼ���ұ�һ��������C�������ⴸ���ˣ��Ǿ�����������ô�õ����⣬��Ϊ�ҵ�������remote_data��ʱ������һ����������װ����

������̿���ģʽ�Ǳ߻��и����⣬��ͷ��һ��

����д��һ�´��룬��������˷ǳ��ǳ��յ�����

����������Ӧ����һ�ζ��˵Ĵ��뵫�Ǻ�������ɾ�ˣ�

�������ң���д����ôɵ�ƵĴ���

���������������ڱ�������������»��ܷɣ���֪�����ǲ���úÿ�developing diary��������Ҫ������������λ����ȵ������ܷ�������ʱ������ĳ������ǹ�����������������

Ȼ����Ҫͣ�����Ļ���